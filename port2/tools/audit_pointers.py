#!/usr/bin/env python3
"""Static audit of the retail code pointers the native port would jump to (docs/PORT.md "M3", open point 1).

The port relocates every overlay image at load (port2/runtime/ovl.c port_relocate_overlay) and MAIN0's own data
at build time (gen2.py emit_data): each aligned data word that holds a retail address becomes the native function,
a dispatch thunk, or a pointer into the data image.  A word the relocator cannot name stays a retail address
("code labels kept"), and the game crashes the first time it jumps through it.  This tool walks every image the
game loads exactly as the relocator does and classifies every word that lands inside code:

  ok        function start with native code (MAIN0/X000 function, an area or SYSTEM function with C, or a
            thunk that resolves for this overlay)
  nonative  function start of a known unit (notes/functions_*.csv) that has no native code in this overlay:
            asm-only, library (Psy-Q) or dropped; a call logs `no native code` / `stub:` and returns 0
  label     address inside a function: a switch jump table (`jtbl_*` symbol, or a run of words into one
            function) is harmless (the native C has its own switch); a *handler* label (one word pointing into
            the middle of a function, used as an entry point) crashes when called
  unknown   address the relocator takes for data (outside every code range of this overlay) but which is code
            in another overlay, or a thunk whose address is no function start of this overlay (the code there
            is the middle of another function here)
  intable   a run of pointers inside a code range of this overlay: a table the csv counts as code, so the
            relocator never touches it (tools/units.py DATA lists the known ones)

Images: MAIN0's data (the build-time image), X000 and every area X0nn (game/AREAnn/X0nn.BIN), the SYSTEM
overlays (tools/sysovl.py), and the load lists SYS/LD0SYS.BIN, LD0ARnn.BIN (read into MAIN0's data, relocated
without code ranges).  MAIN0's table words that point into the overlay region are classified per overlay they
could be used with.  Also listed: the address literals gen2 skipped as constants that fall inside a code unit
(build/port2/gen/literals.txt).

Usage: python3 port2/tools/audit_pointers.py [--gen build/port2/gen] [--out FILE] [--all] [PROG...]
  --all   lists the ok words too; PROG restricts the images (MAIN0, X000, X004, D0SPSUB, LD0AR04, ...)
Exit status 1 when a gap pointer, a data pointer turned thunk or a pointer table inside code is found (the labels
are listed for review: most are entries of per-area tables that the resident area never indexes)."""
import bisect, csv, os, re, sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import units as UN
import sysovl

MAIN0_LOAD, OVL_BASE, OVL_END = 0x80010000, 0x800E8028, 0x80140000
JR_RA = 0x03E00008


def read_int_pairs(path):
    return [tuple(int(x, 16) for x in l.split()) for l in open(path) if l.strip()]


class Tables:
    """The generated run-time tables of build/port2/gen (what the relocator and the dispatcher know)."""
    def __init__(self, gen):
        self.gen = gen
        t = open(os.path.join(gen, "funcmap.c")).read()
        self.funcmap = {int(a, 16): n for a, n in re.findall(r"^\s*\{0x([0-9A-F]{8}), \(void \*\)(\w+), ", t, re.M)}
        self.ovlmap = {(int(i, 16), int(a, 16)): n for i, a, n in re.findall(r"^\s*\{0x([0-9A-F]{2}), 0x([0-9A-F]{8}), \(void \*\)(\w+), ", t, re.M)}
        self.ovls = {int(i, 16): n for i, n in re.findall(r"^\s*\{0x([0-9A-F]{2}), \"(\w+)\"\},", t, re.M)}
        self.areamap = {(int(i), int(a, 16)): n for i, a, n in re.findall(r"^\s*\{(\d+), 0x([0-9A-F]{8}), \(void \*\)(\w+)\},", t, re.M)}
        self.areas = {n: int(i) for i, n in re.findall(r"^\s*\{(\d+), \"(X\d{3})\", \{", t, re.M)}
        t = open(os.path.join(gen, "addrmap.c")).read()
        am = re.search(r"port_addrmap\[\] = \{(.*?)\};", t, re.S).group(1)
        self.addrmap = {int(a, 16): n for a, n in re.findall(r"\{0x([0-9A-F]{8}), \(void \*\)(\w+)\}", am)}
        tm = re.search(r"port_thunkmap\[\] = \{(.*?)\};", t, re.S).group(1)
        self.thunkmap = set(int(a, 16) for a in re.findall(r"\{0x([0-9A-F]{8}), ", tm))
        self.ranges = {}
        for p, body in re.findall(r"static const unsigned int ranges_(\w+)\[\] = \{(.*?)\};", t):
            ws = [int(x, 16) for x in re.findall(r"0x([0-9A-F]{8})", body)]
            self.ranges[p] = list(zip(ws[::2], ws[1::2]))
        self.starts = {}                                 # overlay -> its function starts (the relocator's starts_<prog>)
        for p, body in re.findall(r"static const unsigned int starts_(\w+)\[\] = \{(.*?)\};", t):
            self.starts[p] = set(int(x, 16) for x in re.findall(r"0x([0-9A-F]{8})", body))
        self.vary = set(int(l, 16) for l in open(os.path.join(gen, "vary.txt")) if l.strip())
        self.unit = dict((a, n) for a, n in (l.split() for l in open(os.path.join(gen, "unitnames.txt")) if l.strip()))
        self.unit = {int(a, 16): n for a, n in self.unit.items()}
        self.code = sorted(read_int_pairs(os.path.join(gen, "code_ranges.txt")))      # MAIN0 + X000 units
        self.code_starts = [c[0] for c in self.code]
        self.syms = {}
        for l in open(os.path.join(gen, "symaddr.txt")):
            n, a = l.split(); self.syms.setdefault(int(a, 16), []).append(n)
        self.sym_addrs = sorted(self.syms)

    def is_code(self, a):
        i = bisect.bisect_right(self.code_starts, a) - 1
        return i >= 0 and self.code[i][0] <= a < self.code[i][1]

    def in_ranges(self, p, a):
        for s, e in self.ranges.get(p, ()):
            if s <= a < e: return True
        return False


class Prog:
    """One program's code: its units, binary, data names, jump tables."""
    def __init__(self, name, units):
        self.name = name
        self.units = units
        self.starts = [u.addr for u in units]
        self.by_start = {u.addr: u for u in units}
        self.unit_names = {u.name for u in units}
        path, base = UN.BINS[name]
        self.base = base
        fn = os.path.join(ROOT, path)
        self.blob = open(fn, "rb").read() if os.path.exists(fn) else b""
        if name == "MAIN0": self.blob, self.base = self.blob[0x800:], MAIN0_LOAD    # the PS-X EXE header
        self.data_names = []                                           # (addr, size, name)
        p = os.path.join(ROOT, "notes", "data_names_%s.csv" % name.lower())
        if os.path.exists(p):
            for r in csv.DictReader(open(p)):
                try: self.data_names.append((int(r["address"], 16), int(r.get("size") or "0", 0), r["name"]))
                except ValueError: pass
        self.data_names.sort()
        self.dn_addrs = [d[0] for d in self.data_names]
        self.symbols = {}                                              # name -> (addr, size) of config/symbol_addrs
        p = os.path.join(ROOT, "config", "symbol_addrs_%s.txt" % name.lower())
        for l in (open(p) if os.path.exists(p) else ()):
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);(?:.*size:0x([0-9A-Fa-f]+))?", l)
            if m: self.symbols[m.group(1)] = (int(m.group(2), 16), int(m.group(3) or "0", 16))
        self.jtbl = {}                                                 # addr -> (size, name)
        for n, (a, s) in self.symbols.items():
            if n.startswith("jtbl_"): self.jtbl[a] = (s, n)
        self.jtbl_addrs = sorted(self.jtbl)
        self.switch_files = {}

    def word(self, a):
        o = a - self.base
        return int.from_bytes(self.blob[o:o + 4], "little") if 0 <= o and o + 4 <= len(self.blob) else None

    def unit_at(self, a):
        i = bisect.bisect_right(self.starts, a) - 1
        if i >= 0 and self.units[i].addr <= a < self.units[i].addr + self.units[i].size: return self.units[i]
        return None

    def in_jtbl(self, a, T=None):
        """the jump-table symbol (config/symbol_addrs `jtbl_*`, or the matching build's `jtbl_*` PROVIDE that is
        the nearest symbol before a) holding the word at a"""
        i = bisect.bisect_right(self.jtbl_addrs, a) - 1
        if i >= 0:
            ja, (sz, n) = self.jtbl_addrs[i], self.jtbl[self.jtbl_addrs[i]]
            if ja <= a < ja + max(sz, 4): return n
        if T:
            j = bisect.bisect_right(T.sym_addrs, a) - 1
            if j >= 0 and a - T.sym_addrs[j] < 0x400:
                for n in T.syms[T.sym_addrs[j]]:
                    if n.startswith("jtbl_"): return n
        return None

    def table_name(self, a, T):
        """which named datum holds address a: the data_names row covering it, else the nearest symbol before
        (a name carrying another address, `D_800772BC` PROVIDEd elsewhere, is skipped)"""
        i = bisect.bisect_right(self.dn_addrs, a) - 1
        if i >= 0:
            da, ds, dn = self.data_names[i]
            if da <= a < da + max(ds, 4): return "%s+0x%X" % (dn, a - da)
        j = bisect.bisect_right(T.sym_addrs, a) - 1
        while j >= 0 and a - T.sym_addrs[j] < 0x2000:
            sa = T.sym_addrs[j]
            ns = []
            for n in T.syms[sa]:
                m = re.match(r"(?:D|DAT|jtbl|PTR|PTR_DAT|P)_([0-9A-Fa-f]{8})", n)
                if m and int(m.group(1), 16) != sa: continue
                if n in self.unit_names or re.match(r"X0\d\d_|(?:%s)_" % sysovl.PROG_RE, n): continue
                ns.append(n)
            if ns: return "%s+0x%X" % (sorted(ns, key=len)[0], a - sa)
            j -= 1
        return "@%08X" % a

    def code_start_like(self, v):
        """the bytes of this program at v look like a function start: `jr $ra` two words before it, or
        `addiu $sp, $sp, -N` at it"""
        w, p = self.word(v), self.word(v - 8)
        if not w: return False
        if not (p == JR_RA or ((w >> 16) == 0x27BD and (w & 0x8000))): return False
        for x in range(v, v + 0x1000, 4):             # and a function end follows: not the data after the last function
            if self.word(x) == JR_RA: return True
        return False

    def has_switch(self, u):
        if not u or not u.file: return False
        if u.file not in self.switch_files:
            t = open(u.file, errors="replace").read()
            self.switch_files[u.file] = bool(re.search(r"\bswitch\s*\(", t))
        return self.switch_files[u.file]


class Audit:
    def __init__(self, gen, show_all=False):
        self.T = Tables(gen)
        self.U = UN.units()
        self.P = {p: Prog(p, us) for p, us in self.U.items()}
        self.show_all = show_all
        self.ids = {}                                                  # SYSTEM prog -> header id
        for p, (path, sha, code) in sysovl.SYSOVL.items():
            fn = os.path.join(ROOT, "game", path)
            if os.path.exists(fn): self.ids[p] = int.from_bytes(open(fn, "rb").read(4), "little")
        # every function start over all programs: address -> [progs]
        self.starts_all = defaultdict(list)
        for p, us in self.U.items():
            for u in us: self.starts_all[u.addr].append(p)
        self.results = {}                                              # image -> {kind: [rows]}

    # ---- where does a retail code address lead, for a given overlay P (None = MAIN0/X000 region only)
    def native_for(self, p, v):
        """native name of the function starting at v while overlay p is resident (p = None: MAIN0 region)"""
        T = self.T
        if v < OVL_BASE or v >= OVL_END:
            return T.addrmap.get(v) or T.funcmap.get(v)
        if p in self.ids:
            return T.ovlmap.get((self.ids[p], v))
        if p in T.areas:
            idx = T.areas[p]
            n = T.areamap.get((idx, v))
            if n: return n
            if v in T.addrmap and v not in T.vary: return T.addrmap[v]
            if v in T.funcmap and v not in T.vary: return T.funcmap[v]
        return None

    def classify_target(self, p, v, where, P_img, run_same_unit):
        """classify the retail address v found at word `where` of image P_img, for overlay p resident
        (p = "MAIN0": v is in MAIN0's own code)"""
        T = self.T
        in_region = OVL_BASE <= v < OVL_END
        P = self.P.get(p) if in_region else self.P["MAIN0"]
        others = [q for q in self.starts_all.get(v, []) if q != p]
        n = self.native_for(p, v) if p != "MAIN0" else None
        if n: return "ok", n
        u = P.unit_at(v) if P else None
        if P and v in P.by_start:
            why = "library" if u.lib else ("asm-only" if not u.file else "dropped")
            return "nonative", "%s (%s, %s)" % (u.name, why, "twin" if u.twin else "own")
        if P and u:                                                     # inside a function of this overlay
            jt = P_img.in_jtbl(where, T)
            if jt: return "label-jtbl", "%s+0x%X in %s" % (u.name, v - u.addr, jt)
            if run_same_unit and P.has_switch(u): return "label-jtbl", "%s+0x%X (run of %d words into one switch function)" % (u.name, v - u.addr, run_same_unit)
            note = ""
            if others: note += "; function start in " + ",".join(others)
            if P.code_start_like(v): note += "; looks like a function start the csv lacks (jr $ra before / addiu $sp)"
            return "label", "%s+0x%X (%s)%s" % (u.name, v - u.addr, "C" if u.file else "asm", note)
        if in_region:
            # outside every code range of this overlay: the relocator makes it a data pointer.  Fine unless the
            # bytes here are code the csv does not list
            if P and P.code_start_like(v):
                return "gap", "no unit of %s here but the bytes look like a function start%s" % (p, "; start in " + ",".join(others) if others else "")
            return "data", ""
        return "data", ""

    # ---- the images
    def walk_overlay(self, p):
        """an area X0nn (kind 0) or SYSTEM overlay (kind 1), as port_relocate_overlay does it"""
        T, P = self.T, self.P[p]
        out = defaultdict(list)
        words = [(OVL_BASE + 4 * i, int.from_bytes(P.blob[4 * i:4 * i + 4], "little")) for i in range(len(P.blob) // 4)]
        self.scan_code_tables(p, P, words, out)
        sysv = p in self.ids
        rows = []
        for a, v in words:
            if T.in_ranges(p, a): continue
            if 0x1F800000 <= v < 0x1F800400: continue
            if v < MAIN0_LOAD or v >= 0x80200000 or (v & 3): continue
            if (v & 0xFFFF) == 0:                      # a round value: a flag word to emit_data, a data pointer to the relocator
                u = P.unit_at(v) if OVL_BASE <= v < OVL_END else self.P["MAIN0"].unit_at(v)
                rows.append((a, v, "round", "relocated to a data pointer (emit_data keeps such words as constants)%s" % ("; inside %s" % u.name if u else ""))); continue
            if sysv and OVL_BASE <= v < OVL_END:
                n = T.ovlmap.get((self.ids[p], v))
                if n: rows.append((a, v, "ok", n)); continue
                if T.in_ranges(p, v): rows.append((a, v, None, "raw")); continue
                rows.append((a, v, None, "data")); continue
            if OVL_BASE <= v < OVL_END and p in T.starts and v not in T.starts[p]:
                # no function start of this area here (its units and the X000 units it shares): a label or data,
                # never a thunk (the rule since the thunk-data fix; "thunk" marks what the old relocator did)
                rows.append((a, v, None, "raw" if T.in_ranges(p, v) else ("thunk" if v in T.thunkmap else "data"))); continue
            if v in T.addrmap: rows.append((a, v, "ok", T.addrmap[v])); continue
            if v in T.thunkmap: rows.append((a, v, None, "thunk")); continue
            if T.in_ranges(p, v): rows.append((a, v, None, "raw")); continue
            rows.append((a, v, None, "data"))
        self.finish(p, P, rows, out)
        self.results[p] = out

    def walk_main0(self):
        """MAIN0's data image as gen2.emit_data relocates it: words inside MAIN0/X000 code ranges are skipped, round
        values are constants; a word into the overlay region is checked against every overlay"""
        T, P = self.T, self.P["MAIN0"]
        out = defaultdict(list)
        words = [(MAIN0_LOAD + 4 * i, int.from_bytes(P.blob[4 * i:4 * i + 4], "little")) for i in range(len(P.blob) // 4)]
        self.scan_code_tables("MAIN0", P, words, out)
        rows = []
        for a, v in words:
            if T.is_code(a): continue
            if v < MAIN0_LOAD or v >= 0x80200000 or (v & 3) or (v & 0xFFFF) == 0: continue
            if v in T.thunkmap: rows.append((a, v, None, "thunk")); continue
            if v in T.addrmap: rows.append((a, v, "ok", T.addrmap[v])); continue
            if v in T.unit and v not in T.addrmap:                      # a function start with no native code: a stub
                rows.append((a, v, "nonative", "%s (stub)" % T.unit[v])); continue
            if T.is_code(v): rows.append((a, v, None, "raw")); continue
            rows.append((a, v, None, "data"))
        self.finish("MAIN0", P, rows, out)
        self.results["MAIN0"] = out

    def walk_loadlist(self, name):
        """SYS/LD0*.BIN: read into MAIN0's data, relocated without code ranges (addrmap, thunkmap, else data)"""
        T, P = self.T, self.P["MAIN0"]
        fn = os.path.join(ROOT, "game", "disc", "fs", "SYS", name + ".BIN")
        out = defaultdict(list)
        if not os.path.exists(fn): self.results[name] = out; return
        blob = open(fn, "rb").read()
        rows = []
        for i in range(len(blob) // 4):
            a, v = 4 * i, int.from_bytes(blob[4 * i:4 * i + 4], "little")
            if v < MAIN0_LOAD or v >= 0x80200000 or (v & 3): continue
            if v in T.addrmap: rows.append((a, v, "ok", T.addrmap[v])); continue
            if v in T.thunkmap: rows.append((a, v, None, "thunk")); continue
            if T.is_code(v) or (OVL_BASE <= v < OVL_END and any(Q.unit_at(v) for Q in self.P.values())):
                rows.append((a, v, None, "raw")); continue
        for a, v, k, how in rows:
            if k == "ok": out["ok"].append(("+%04X" % a, v, how)); continue
            out["unknown"].append(("+%04X" % a, v, "a code address in a load list (%s)" % how))
        self.results[name] = out

    def finish(self, p, P, rows, out):
        """resolve the thunk / raw / data rows for overlay p (MAIN0: for every overlay that could be resident)"""
        T = self.T
        # runs of consecutive words into the same unit (jump tables)
        def unit_of(v):                # the unit holding v for this image's own region (MAIN0's words: MAIN0 or X000)
            Q = self.P.get(p) if p != "MAIN0" else (self.P["MAIN0"] if v < OVL_BASE else self.P["X000"])
            return Q.unit_at(v) if Q else None
        run = {}
        i = 0
        while i < len(rows):
            a, v, k, how = rows[i]
            u = unit_of(v) if how == "raw" else None
            j = i + 1
            while u and j < len(rows) and rows[j][0] == rows[j - 1][0] + 4 and rows[j][3] == "raw" and unit_of(rows[j][1]) is u: j += 1
            if u and j - i >= 2:
                for k2 in range(i, j): run[rows[k2][0]] = j - i
            i = max(j, i + 1)
        for a, v, k, how in rows:
            where = "%s @%08X" % (P.table_name(a, T), a)
            if k == "ok": out["ok"].append((where, v, how)); continue
            if k == "nonative": out["nonative"].append((where, v, how)); continue
            if k == "round": out["round"].append((where, v, how)); continue
            if p != "MAIN0" or v < OVL_BASE or v >= OVL_END:
                kind, what = self.classify_target(p, v, a, P, run.get(a, 0))
                if kind == "data" and how == "thunk":
                    if p in T.starts: kind, what = "data-at-start", "a data pointer that is another overlay's function start (relocated as data since the per-overlay starts)"
                    else: kind, what = "thunk-data", "a data pointer the relocator turns into the dispatch thunk of another overlay's function"
                if kind == "label" and p != "MAIN0":
                    # the same word in the other area files: a table every overlay carries (per-area anim sets,
                    # the dispatcher cases of other areas), whose entry for another area is dead here
                    same = [q for q in ["X000"] + UN.OVERLAYS if q != p and q in self.P and self.P[q].word(a) == v]
                    if same: what += "; same word in %d other area files" % len(same)
                if kind != "data": out[kind].append((where, v, what))
                continue
            # MAIN0's word into the overlay region: which overlay the table entry is used with is unknown
            # (object-type and per-area tables); report what every overlay has at that address
            per = defaultdict(list)
            for q in ["X000"] + [x for x in UN.OVERLAYS] + list(sysovl.PROGS):
                if q not in self.P or not self.P[q].blob: continue
                kind, what = self.classify_target(q, v, a, P, run.get(a, 0))
                per[kind].append(q)
            bad = {k: v2 for k, v2 in per.items() if k in ("label", "gap")}
            if bad:
                det = "; ".join("%s in %s" % (k, ",".join(v2)) for k, v2 in sorted(bad.items()))
                u0 = self.P["X000"].unit_at(v)
                det += " [X000: %s]" % ("%s+0x%X" % (u0.name, v - u0.addr) if u0 else "no code")
                if per.get("ok"): det += "; start in " + ",".join(per["ok"])
                out["label" if "label" in bad and "gap" not in bad else "gap"].append((where, v, det))
            elif "nonative" in per: out["nonative"].append((where, v, "ok in %d overlays; no native in %s" % (len(per.get("ok", [])), ",".join(per["nonative"]))))
            elif "label-jtbl" in per: out["label-jtbl"].append((where, v, "jump table of %s" % ",".join(per["label-jtbl"])))
            else: out["ok"].append((where, v, "ok in %s" % ",".join(per.get("ok", [])) if per.get("ok") else "data in every overlay"))

    def scan_code_tables(self, p, P, words, out):
        """pointer tables inside a code range: runs of >= 3 words that are function starts (any program) or named data"""
        T = self.T
        ranges = T.ranges.get(p) if p != "MAIN0" else T.code
        if not ranges: return
        def ptr_like(v):
            if v in self.starts_all: return True
            if v in T.syms and not (v & 0xFFFF) == 0: return True
            return False
        i, n = 0, len(words)
        rs = iter(sorted(ranges))
        for s, e in sorted(ranges):
            lo = bisect.bisect_left(words, (s, 0)); hi = bisect.bisect_left(words, (e, 0))
            k = lo
            while k < hi:
                if ptr_like(words[k][1]) and MAIN0_LOAD <= words[k][1] < 0x80200000:
                    j = k
                    while j < hi and MAIN0_LOAD <= words[j][1] < 0x80200000 and ptr_like(words[j][1]): j += 1
                    if j - k >= 3:
                        u = P.unit_at(words[k][0])
                        out["intable"].append((P.table_name(words[k][0], T), words[k][0],
                                               "%d pointer words at %08X..%08X inside %s" % (j - k, words[k][0], words[j - 1][0], u.name if u else "?")))
                    k = j
                else: k += 1

    def constants(self):
        """address literals gen2 skipped as constants (round values) that fall inside a code unit"""
        p = os.path.join(self.T.gen, "literals.txt")
        out = []
        if not os.path.exists(p): return out
        for l in open(p):
            m = re.match(r"(\S+):(\d+): (0[xX][0-9A-Fa-f]+)\S* \(constant\)", l)
            if not m: continue
            a = int(m.group(3), 16)
            if self.T.is_code(a) or any(Q.unit_at(a) for Q in self.P.values()):
                src = os.path.join(ROOT, m.group(1))
                if not os.path.exists(src): src = os.path.join(self.T.gen, m.group(1))
                line = ""
                if os.path.exists(src):
                    ls = open(src, errors="replace").read().split("\n")
                    line = ls[int(m.group(2)) - 1].strip() if int(m.group(2)) <= len(ls) else ""
                fp = bool(re.search(r"\(\s*void\s*\(\s*\*|\)\s*\(\s*\)|handler|fn\b|func\b", line))
                out.append((m.group(1), int(m.group(2)), a, line[:110], fp))
        return out


KINDS = ["ok", "nonative", "label-jtbl", "label", "gap", "thunk-data", "intable", "round", "data-at-start"]


def main():
    args = sys.argv[1:]
    gen = os.path.join(ROOT, "build", "port2", "gen"); outp = None; show_all = False; progs = []
    while args:
        a = args.pop(0)
        if a == "--gen": gen = args.pop(0)
        elif a == "--out": outp = args.pop(0)
        elif a == "--all": show_all = True
        else: progs.append(a.upper())
    A = Audit(gen, show_all)
    images = ["MAIN0", "X000"] + UN.OVERLAYS + list(sysovl.PROGS) + ["LD0SYS"] + ["LD0AR%02d" % n for n in range(20)]
    if progs: images = [i for i in images if i in progs]
    for im in images:
        if im == "MAIN0": A.walk_main0()
        elif im.startswith("LD0"): A.walk_loadlist(im)
        elif im in A.P and A.P[im].blob: A.walk_overlay(im)
    lines = []
    W = lines.append
    W("pointer audit of the native port (port2/tools/audit_pointers.py; tables of %s)" % os.path.relpath(gen, ROOT))
    W("")
    W("%-9s %6s %9s %10s %7s %5s %10s %7s %6s %8s" % ("image", "ok", "nonative", "label-jtbl", "label", "gap", "thunk-data", "intable", "round", "data@fn"))
    tot = defaultdict(int)
    for im in images:
        r = A.results.get(im)
        if r is None: continue
        W("%-9s %6d %9d %10d %7d %5d %10d %7d %6d %8d" % (im, *[len(r.get(k, [])) for k in KINDS]))
        for k in KINDS: tot[k] += len(r.get(k, []))
    W("%-9s %6d %9d %10d %7d %5d %10d %7d %6d %8d" % ("total", *[tot[k] for k in KINDS]))
    W("")
    W("ok: native function / resolving thunk.  nonative: known function start without native code (a stub or `no native code`, returns 0).")
    W("label-jtbl: switch jump table (harmless natively).  label: a handler entry inside a function (crash when called).")
    W("gap: a pointer to code outside every unit of the overlay (a function the csv lacks: relocated as data, a call crashes).")
    W("thunk-data: a data pointer that is another overlay's function start (relocated to its thunk: wrong data).  intable: pointer runs inside a code range (never relocated).")
    W("round: 0x80NN0000 words (flags to emit_data, data pointers to the run-time relocator).  data@fn: data pointers at another overlay's function start (data since the per-overlay starts).")
    W("MAIN0 words into the overlay region are classified per overlay (a thunk if any overlay starts a function there, else a data pointer).")
    bad = tot["gap"] + tot["thunk-data"] + tot["intable"]
    for im in images:
        r = A.results.get(im)
        if not r: continue
        for k in (KINDS if show_all else ["nonative", "label", "gap", "thunk-data", "intable"]):
            rows = r.get(k, [])
            if not rows: continue
            W(""); W("## %s: %s (%d)" % (im, k, len(rows)))
            seen = set()
            for where, v, what in rows:
                key = (where, v, what) if k != "ok" else (v, what)
                if key in seen: continue
                seen.add(key)
                W("  %-40s %08X  %s" % (where, v, what))
    cs = A.constants()
    W(""); W("## address literals skipped as constants that lie inside a code unit (%d; * = looks like a function pointer use)" % len(cs))
    for f, l, a, line, fp in cs: W("  %s%s:%d: %08X  %s" % ("*" if fp else " ", f, l, a, line))
    txt = "\n".join(lines) + "\n"
    if outp: open(outp, "w").write(txt)
    sys.stdout.write(txt)
    sys.exit(1 if bad or any(c[4] for c in cs) else 0)


if __name__ == "__main__":
    main()

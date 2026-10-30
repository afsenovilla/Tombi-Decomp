#!/usr/bin/env python3
"""Static census of dialogue references (research tool, read-only).

WFM3 header bytes 16..143 are NOT reserved: they are 64 x u16 group bases. openMessageBox(k, dx, ...) shows dialogue
`base[k] + dx` (src/openMessageBox.c: `*(short*)(wfm + 0x10 + k*2) + dx`). Group 0 (base 0) = system/item/info text.

References found without running the game:
  * `jal` to showMessageBox / ...Timed / ...Direct / ...DirectTimed / openMessageBox in the area overlays (X0nn.BIN) whose
    a0 (k) and a1 (dx) are loaded with an immediate in the same basic block;
  * script op 0x85 (`05 nargs [01 imm32]* 85`) in AREAnn/*.WSS (args: obj, cmd=4, k, dx, ...).
Calls with computed k/dx (NPC objects whose ids come from room data, loops) are NOT seen, so "unreferenced" is only a
candidate list.   usage: dialogue_refs.py [--delta] [AREAnn/MFNTnn0S ...]  (default: all Spanish files)
The static scan is joined by literal (k, dx) pairs of the matched C sources (src_refs; tools/objects.py dialogue_consts).
"""
import glob, os, re, struct, sys, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wfm_py

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FS = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "game", "disc", "fs")
TG = {0x8002DC50, 0x8002DC88, 0x8002DCC8, 0x8002DCF0, 0x8002DD1C}
# overlay of area nn -> its Spanish dialogue files (file names shared between areas: 07 uses 010, 12 uses 040, 19 uses 020+190, 13 uses 001+130)
FILES = {0: ["MFNT000S", "MFNT001S"], 1: ["MFNT010S"], 7: ["MFNT010S"], 12: ["MFNT040S"], 13: ["MFNT001S", "MFNT130S"], 19: ["MFNT020S", "MFNT190S"]}


def files_of(a):
    return FILES.get(a, ["MFNT%02d0S" % a])


def call_refs(path, with_addr=False):
    d = open(path, "rb").read()
    n = len(d) // 4
    w = struct.unpack("<%dI" % n, d[:n * 4])
    out, unk = set(), 0
    for i, x in enumerate(w):
        if x >> 26 == 3 and (0x80000000 | ((x & 0x3FFFFFF) << 2)) in TG:
            r = {}
            for j in [i + 1] + list(range(i - 1, max(i - 40, 0), -1)):
                y = w[j]
                op, rs, rt = y >> 26, (y >> 21) & 31, (y >> 16) & 31
                if j < i and (op in (2, 3, 4, 5, 6, 7) or (op == 0 and (y & 0x3F) in (8, 9))):
                    break
                if op in (9, 13) and rs == 0 and rt in (4, 5) and rt not in r:
                    r[rt] = y & 0xFFFF
            if 4 in r and 5 in r:
                out.add((0x800E8028 + 4 * i, r[4], r[5]) if with_addr else (r[4], r[5]))
            else:
                unk += 1
    return out, unk


def src_refs(a):
    """Literal (k, dx) pairs in the matched C sources of overlay X0<a>: direct showMessageBox*/openMessageBox calls AND `m = K; n = DX;`
    assignments (NPC handlers set them per state and call once, which the basic-block scan of call_refs cannot see).  AREA00 = src/*.c
    functions headed `// FUNC .. X000`; the others live in src/x0NN/."""
    import objects
    out = set()
    if a == 0:
        files = [f for f in glob.glob(os.path.join(ROOT, "src", "*.c")) if open(f).readline().rstrip().endswith("X000")]
    else:
        files = glob.glob(os.path.join(ROOT, "src", "x%03d" % a, "*.c"))
    for f in files:
        out |= objects.dialogue_consts(open(f).read())
    return out


WSS_VAR = collections.Counter()      # op 0x85 / cmd 4 with a variable (k or dx) per scene file: invisible to the resolver


def wss_refs(path):
    d = open(path, "rb").read()
    out = set()
    for m in re.finditer(rb"\x05([\x01-\x08])", d):
        n, i, vals, ok = m.group(1)[0], m.end(), [], True
        for _ in range(n):
            if i >= len(d) or d[i] not in (0, 1):
                ok = False
                break
            if d[i] == 1:
                vals.append(struct.unpack("<I", d[i + 1:i + 5])[0]); i += 5
            else:
                vals.append(None); i += 2
        if ok and i < len(d) and d[i] == 0x85 and len(vals) >= 4 and vals[1] == 4 and vals[2] is not None and vals[3] is not None:
            if vals[3] < 0x8000:
                out.add((vals[2], vals[3]))
        elif ok and i < len(d) and d[i] == 0x85 and len(vals) >= 4 and vals[1] == 4:
            WSS_VAR[os.path.basename(path)] += 1
    return out


# ---------------------------------------------------------------------------------------------------------------------------
# Computed-argument resolver (--resolve).  Scans the matched C of src/ (never the *_slps01144.c JP ports) for EVERY call of the
# message functions (names below, incl. the raw func_8002Dxxx names of the I0NFO overlays and #define wrappers such as CALL/SPAWN)
# and works out the possible (k, dx):
#   literal                      -> exact
#   TABLE[idx]                   -> every element of the table (static array in the C, or read from the overlay / MAIN0 binary at the
#                                   address in the symbol name; 2-D `TAB[i][c]` -> column c; BUTTON_CONFIG indices -> 6 entries)
#   DAT_xxxxxxxx (state byte)    -> every literal assigned to it anywhere in src/; if it is also ++'d / address-taken, the counter
#                                   closure (v..group end) is added as "maybe" (range, not proof)
#   e[1] / c->arg                -> the args of the message op of the scene-sequence tables (objects.SEQ_TABLES)
#   showMessageBox(text, opt)    -> MAIN0 sign table D_80079EBC (+ the 4 button-config tables), limited to placed subtypes
# What it cannot see stays "undetermined": call sites with an unresolved dx and a literal k mark the WHOLE group k of the files
# they feed; WSS op 0x85 with variable args (counted per scene file).  Classes: reachable / undetermined / unreachable.
MSG_NAMES = r"(?:showMessageBox\w*|openMessageBox|func_8002(?:DC50|DC88|DCC8|DCF0|DD1C))"
NUMRE = r"(?:0x[0-9a-fA-F]+|\d+)"
BTN = 6                        # button configs (BUTTON_CONFIG_INDEX 0..5; MAIN0 tables D_80079E8C.. are short[6])
TOWN_AREAS = [5, 8, 11, 16, 17, 18, 19]
SIGN_ALL = {}
SEQ_OVL = {0: 0, 2: 2, 4: 4, 6: 6, 13: 13}


def _split_args(t, i):
    """args of the call whose '(' is at t[i-1]; returns (list of arg strings, end index)"""
    depth, cur, out = 1, "", []
    while i < len(t) and depth:
        c = t[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth == 0:
                break
        if c == "," and depth == 1:
            out.append(cur.strip()); cur = ""
        else:
            cur += c
        i += 1
    out.append(cur.strip())
    return out, i


def _strip(t):
    t = re.sub(r"/\*.*?\*/", " ", t, flags=re.S)
    return re.sub(r"//[^\n]*", "", t)


def src_files():
    """[(path, kind)] kind = 'main' (MAIN0: every file), or overlay number; JP port files are skipped"""
    out = []
    for f in sorted(glob.glob(os.path.join(ROOT, "src", "**", "*.c"), recursive=True)):
        b = os.path.basename(f)
        rel = os.path.relpath(f, os.path.join(ROOT, "src"))
        if "slps01144" in b or rel.startswith(("wip", "data")):
            continue
        d = rel.split(os.sep)[0]
        if d.startswith("x0") and len(d) == 4:
            out.append((f, int(d[1:]))); continue
        if d in ("i0nfo", "i0nfo2", "i0nfo3"):
            out.append((f, d)); continue
        if d in ("d0spsub", "d0spsub2", "d0spsub3", "g0over", "o0ptsub0"):
            out.append((f, d)); continue
        h = open(f, errors="replace").readline()
        m = re.match(r"//\s*FUNC\s+\w+\s+\w+\s+(\w+)", h)
        if m and m.group(1) == "MAIN0":
            out.append((f, "main"))
        elif m and re.fullmatch(r"X0\d\d", m.group(1)):
            out.append((f, int(m.group(1)[1:])))
    return out


def feeds(kind):
    """dialogue files an overlay/module can show text from"""
    if kind == "main":
        return None                                         # all
    if kind in ("d0spsub", "d0spsub2", "d0spsub3", "g0over", "o0ptsub0"):
        return []
    if kind == "i0nfo" or kind == "i0nfo3":
        return ["MFNT020S"]
    if kind == "i0nfo2":
        return sum([files_of(a) for a in TOWN_AREAS], [])
    r = list(files_of(kind))
    if kind == 2:
        r += files_of(19)
    if kind == 1:
        r += files_of(7)
    return r


NPCSET_AREA = {"07": 5, "0C": 8, "0D": 11, "0E": 16, "0F": 17, "10": 18, "11": 19}


def ffiles(kind, fn):
    """files a function of module `kind` can show text from; I0NFO2 functions are attributed to their town by name
    (topdownAnn* / npcSetNN* per docs/CIVILIZATION.md; generic topdown* = Baccus Village/Lake; unknown = every town)"""
    if kind != "i0nfo2":
        return feeds(kind)
    m = re.search(r"topdownA(\d\d)", fn)
    if m:
        ar = [int(m.group(1))]
    else:
        m = re.search(r"npcSet([0-9A-F]{2})", fn)
        if m and m.group(1) in NPCSET_AREA:
            ar = [NPCSET_AREA[m.group(1)]]
        elif re.match(r"topdown(Actor|TalkSpot|Section)", fn):
            ar = [5, 8]
        else:
            ar = TOWN_AREAS
    return sum([files_of(a) for a in ar], [])


_ovl_bin = {}


def read_mem(kind, addr, n):
    """bytes at run address addr of MAIN0 or an overlay (all overlays/I0NFO load at BASE 0x800E8028 from file offset 0)"""
    if kind == "main":
        import objects
        m = objects.main0()
        return m[addr - 0x80010000: addr - 0x80010000 + n]
    if kind not in _ovl_bin:
        if isinstance(kind, int):
            rel = glob.glob(FS + "/AREA%02d/X%03d.BIN" % (kind, kind))
        else:
            rel = glob.glob(FS + "/SYSTEM/%s.BIN" % kind.upper())
        _ovl_bin[kind] = open(rel[0], "rb").read() if rel else b""
    return _ovl_bin[kind][addr - 0x800E8028: addr - 0x800E8028 + n]


def data_sizes(kind):
    """{name: (addr, size)} from notes/data_names_<ovl>.csv (size = distance to the next listed datum)"""
    nm = {"main": "main0"}.get(kind, kind if isinstance(kind, str) else "x%03d" % kind)
    p = os.path.join(ROOT, "notes", "data_names_%s.csv" % nm)
    rows = []
    try:
        for ln in open(p):
            f = ln.split(",")
            if re.fullmatch(r"[0-9a-f]{8}", f[0]):
                rows.append((int(f[0], 16), f[1]))
    except OSError:
        pass
    rows.sort()
    return {n: (a, (rows[i + 1][0] - a) if i + 1 < len(rows) else 64) for i, (a, n) in enumerate(rows)}


def table_vals(name, kind, text, idxs, rowsz=None):
    """set of element values of table `name`; static C array in `text`, else binary data.  idxs = number of [] suffixes."""
    m = re.search(r"static\s+(?:unsigned\s+)?(short|char|int)\s+%s\s*(?:\[[^\]]*\])+\s*=\s*\{(.*?)\};" % re.escape(name), text, re.S)
    if m:
        return {int(x, 0) for x in re.findall(r"-?0x[0-9a-fA-F]+|-?\d+", m.group(2))}, None
    m = re.fullmatch(r"D_([0-9A-Fa-f]{8})", name)
    addr = int(m.group(1), 16) if m else None
    sz = None
    ds = data_sizes(kind)
    if name in ds:
        addr, sz = ds[name]
    if addr is None:
        sym = os.path.join(ROOT, "build", "port2", "gen", "symaddr.txt")
        try:
            for ln in open(sym):
                f = ln.split()
                if f and f[0] == name:
                    addr = int(f[1], 16)
        except OSError:
            pass
    return None, (addr, sz)


def width_of(decl_text, name):
    m = re.search(r"extern\s+(unsigned\s+char|short|unsigned\s+short|char|int)\s+%s((?:\s*\[[^\]]*\])+)" % re.escape(name), decl_text)
    if not m:
        return 2, [None]
    w = 1 if "char" in m.group(1) else 4 if m.group(1) == "int" else 2
    dims = [int(x) if x.strip() else None for x in re.findall(r"\[([^\]]*)\]", m.group(2))]
    return w, dims


def counter_vals(name, alltext):
    """literals assigned to state byte DAT_xxxxxxxx anywhere in src (+ whether it is incremented / address-taken)"""
    base = re.sub(r"[AR]?$", "", name)
    pat = base + r"[AR]?(?:\[0\])?"
    vals = {int(v, 0) for v in re.findall(pat + r"\s*=\s*(%s)\s*;" % NUMRE, alltext)}
    vals |= {int(v, 0) for v in re.findall(pat + r"\s*=\s*(%s)\s*;" % NUMRE, alltext)}
    inc = bool(re.search(pat + r"\s*(\+\+|\+=)|\+\+\s*" + pat + r"|&\s*" + pat, alltext))
    return vals, inc


def seq_msg_args(ovl):
    """{(op): args} -> set of the args of message ops in overlay ovl's scene-sequence tables"""
    import objects
    tab, n, _s, _c, _m = objects.SEQ_TABLES[ovl]
    d = read_mem(ovl, tab, 4 * n)
    return tab, n


def seq_args(ovl, ops):
    import objects
    tab, n = objects.SEQ_TABLES[ovl][:2]
    out = set()
    for i in range(n):
        p = struct.unpack_from("<I", read_mem(ovl, tab + 4 * i, 4))[0]
        a = p
        for _ in range(80):
            op, arg = struct.unpack("<hh", read_mem(ovl, a, 4))
            a += 4
            if op in ops:
                out.add(arg)
            if op == 0 and a > p + 4:
                break
    return out


def sign_pairs():
    """(k,dx) of the MAIN0 sign table D_80079EBC (37 entries) per placed subtype: {area: {(k,dx)}} and the unplaced entries"""
    import objects
    T = []
    for i in range(37):
        t, o = struct.unpack("<HH", read_mem("main", 0x80079EBC + 8 * i, 4))
        T.append((t, o))
    cfg = {2: 0x80079E8C, 3: 0x80079E98, 4: 0x80079EA4, 6: 0x80079EB0}

    def pairs(sub):
        t, o = T[sub]
        if sub in cfg:
            return {(t, struct.unpack("<H", read_mem("main", cfg[sub] + 2 * b, 2))[0]) for b in range(BTN)}
        return {(t, o)}
    placed = collections.defaultdict(set)
    for r in objects.build():
        if r.get("layer") == 4 and r.get("type") == 0x1A:
            sub = r.get("subtype")
            if sub is not None and sub < len(T):
                placed[r["area"]].add(sub)
                if sub == 0x13: placed[r["area"]].add(0x14)         # subtype rewrites by game state
                if sub == 0x14: placed[r["area"]].add(0x23)
    return T, pairs, placed


def resolve_all():
    """-> (refs {file: {(k,dx): kind}}, unresolved {file: [(k or None, site text)]}, notes [str])"""
    import objects
    files = src_files()
    texts = {f: _strip(open(f, errors="replace").read()) for f, _ in files}
    alltext = "\n".join(texts.values())
    refs = collections.defaultdict(dict)
    unres = collections.defaultdict(list)
    log = []
    allf = sorted({f for a in range(20) for f in files_of(a)})

    def put(kind, k, dx, how, where):
        for fl in (allf if feeds(kind) is None else ffiles(kind, where)):
            if dx is not None and (k, dx) not in refs[fl] or refs[fl].get((k, dx)) == "maybe":
                refs[fl][(k, dx)] = how
    tabcache = {}
    for f, kind in files:
        t = texts[f]
        macros = {}
        for m in re.finditer(r"#define\s+(\w+)\(([^)]*)\)([^\n]*(?:\\\n[^\n]*)*)", t):
            body = m.group(3)
            mm = re.search(r"\b%s\(" % MSG_NAMES, body)
            if mm:
                args, _ = _split_args(body, mm.end())
                macros[m.group(1)] = ([p.strip() for p in m.group(2).split(",")], args)
        sites = []
        for m in re.finditer(r"\b(%s)\(" % MSG_NAMES, t):
            args, end = _split_args(t, m.end())
            if len(args) < 2 or re.match(r"(int|short|void|unsigned|char|TObj|P6|S)\b", args[0]):
                continue
            ln = t.rfind("#define", 0, m.start())
            if ln != -1 and "\n" not in t[ln:m.start()].replace("\\\n", ""):
                continue                                    # inside a macro body: handled through its invocations
            if kind == "main" and args[0] == "text":
                continue
            sites.append((args[0], args[1], m.start()))
        for mname, (params, margs) in macros.items():
            for m in re.finditer(r"\b%s\(" % mname, t):
                a, _ = _split_args(t, m.end())
                if len(a) != len(params) or "#define" in t[max(0, m.start() - 8):m.start()] or re.match(r"(int|short)\b", a[0]):
                    continue
                sub = dict(zip(params, a))
                sites.append((sub.get(margs[0], margs[0]), sub.get(margs[1], margs[1]), m.start()))
        for ka, da, pos in sites:
            fn = os.path.basename(f)[:-2]
            if re.fullmatch(NUMRE, ka) and re.fullmatch(NUMRE, da):
                continue                                    # literal: already seen by dialogue_consts / call_refs
            kset = {int(ka, 0)} if re.fullmatch(NUMRE, ka) else None
            dset, how = None, "tab"
            mt = re.fullmatch(r"(\w+)((?:\s*\[[^\]]*\])+)", da)
            mc = re.fullmatch(r"(DAT_[0-9a-f]{8})[AR]?(?:\[0\])?", da)
            if mc:
                v, inc = counter_vals(mc.group(1), alltext)
                v0 = set(v) | {0}                           # globals start at 0
                dset = set(v0)
                if inc:
                    dset |= set(range(min(v0), max(v0) + 6))
                    how = "maybe"
                log.append("%s: %s %s %s" % (fn, mc.group(1), "counter (range, maybe)" if inc else "state byte literals", sorted(v)))
            elif mt:
                name, idx = mt.group(1), re.findall(r"\[([^\]]*)\]", mt.group(2))
                sv, loc = table_vals(name, kind, t, len(idx))
                if sv is not None:
                    dset = sv
                elif loc[0]:
                    w, dims = width_of(t, name)
                    if any(("BUTTON_CONFIG" in x or "bcf1c" in x) for x in idx) and len(idx) == 1:
                        n = BTN
                    elif len(idx) == 1 and loc[1]:
                        n = loc[1] // w
                    else:
                        n = (loc[1] or 64) // w
                    raw = read_mem(kind, loc[0], n * w)
                    fmt = {1: "B", 2: "H", 4: "I"}[w]
                    dset = set(struct.unpack("<%d%s" % (len(raw) // w, fmt), raw[:len(raw) // w * w]))
                    log.append("%s %s: %s[%s] = %d entries @%08X" % (fn, ka + "," + da, name, "][".join(idx), len(dset), loc[0]))
            elif da in ("e[1]", "c->arg") and isinstance(kind, int) and kind in SEQ_OVL:
                cs = [int(x) for x in re.findall(r"case\s+(\d+):", t[:pos])]
                op = cs[-1]
                dset = seq_args(kind, {op})
                log.append("%s: scene sequence op %d args %s" % (fn, op, sorted(dset)))
            if dset is not None and kset:
                for k in kset:
                    for dx in dset:
                        put(kind, k, dx, how, fn)
            else:
                for fl in (allf if feeds(kind) is None else ffiles(kind, fn)):
                    unres[fl].append((min(kset) if kset else None, "%s(%s, %s)" % (fn, ka, da)))
    # special: columns of 2-D talk-spot tables (k,dx taken from the same row)
    for kind, tname, nm in (("i0nfo2", "TOPDOWN_TALK_SPOT_DEFS", "i0nfo2"), ("i0nfo3", "TUNNEL_TALK_SPOT_DEFS", "i0nfo3")):
        a, sz = data_sizes(kind).get(tname, (None, None))
        if a:
            raw = read_mem(kind, a, sz)
            for r in range(sz // 8):
                k, dx = struct.unpack_from("<hh", raw, 8 * r)
                if k > 0:
                    put(kind, k, dx, "tab", "topdownTalkSpotStep" if kind == "i0nfo2" else tname)
            for fl in feeds(kind):
                unres[fl] = [u for u in unres[fl] if "TALK_SPOT_DEFS" not in u[1]]
    # sign table (MAIN0 updateL4Type1AObject): placed subtypes only
    T, pairs, placed = sign_pairs()
    for area, subs in placed.items():
        for fl in files_of(area):
            for s_ in subs:
                for k, dx in pairs(s_):
                    refs[fl].setdefault((k, dx), "tab")
    for fl in list(unres):
        unres[fl] = [u for u in unres[fl] if "text, opt" not in u[1]]
    global SIGN_ALL
    SIGN_ALL = {(k, dx): i for i in range(len(T)) for (k, dx) in pairs(i)}
    log.append("sign table: placed signs %s" % {a: sorted(v) for a, v in sorted(placed.items())})
    # signs in the table that no room places (code-reachable only)
    unplaced = sorted(set(range(len(T))) - set().union(*placed.values()))
    log.append("sign subtypes never placed in any room: %s" % unplaced)
    return refs, unres, log


def resolve_report(only=None):
    import objects
    orig = collections.defaultdict(set)                       # exactly what the plain scan (main()) sees
    for p in sorted(glob.glob(FS + "/AREA*/X0??.BIN")):
        a = int(os.path.basename(p)[2:4])
        r, _u = call_refs(p)
        for f in files_of(a):
            orig[f] |= r | src_refs(a)
    for p in sorted(glob.glob(FS + "/AREA15/*.WSS")):
        b = os.path.basename(p)
        pre = 1 if b.startswith("SCR01") else int(b[:2])
        orig[files_of(pre)[0]] |= wss_refs(p)
    old = collections.defaultdict(set)                        # + I0NFO overlays, MAIN0 and every literal call in src/
    for f in orig:
        old[f] |= orig[f]
    fcsv = sorted((int(l.split(",")[0], 16), l.split(",")[2]) for l in open(os.path.join(ROOT, "notes", "functions_i0nfo2.csv")) if re.match(r"[0-9a-f]{8},", l))
    for ov in ("I0NFO", "I0NFO2", "I0NFO3"):
        for ad, k, dx in call_refs(FS + "/SYSTEM/%s.BIN" % ov, True)[0]:
            fn = ""
            if ov == "I0NFO2":
                fn = [n for a_, n in fcsv if a_ <= ad][-1]
            for fl in ffiles(ov.lower(), fn):
                old[fl].add((k, dx))
    num = r"(%s)" % NUMRE
    for f, kind in src_files():
        t = _strip(open(f, errors="replace").read())
        fn = os.path.basename(f)[:-2]
        tg = feeds(kind) if kind == "main" else ffiles(kind, fn)
        for m in re.finditer(r"\b%s\(\s*%s\s*,\s*%s\s*[,)]" % (MSG_NAMES, num, num), t):
            for fl in (list(orig) if tg is None else tg):
                old[fl].add((int(m.group(1), 0), int(m.group(2), 0)))
        for m in re.finditer(r"\b(?:CALLS?|SPAWN)\(\s*%s\s*,\s*%s\s*[,)]" % (num, num), t):
            for fl in (tg or []):
                old[fl].add((int(m.group(1), 0), int(m.group(2), 0)))
        for m in re.finditer(r"\bm\s*=\s*%s;\s*n\s*=\s*%s;" % (num, num), t):
            for fl in (list(orig) if tg is None else tg):
                old[fl].add((int(m.group(1), 0), int(m.group(2), 0)))
    refs, unres, log = resolve_all()
    for l in log:
        print("# " + l)
    flagged = 0
    for f in sorted(orig.keys() | refs.keys()):
        if only and f not in only:
            continue
        area = glob.glob(FS + "/AREA*/" + f + ".WFM")
        if not area:
            continue
        w = wfm_py.decode(open(area[0], "rb").read())
        we = wfm_py.decode(open(area[0].replace("S.WFM", "B.WFM"), "rb").read())
        base = list(struct.unpack("<64H", w.reserved))
        ng = max([i for i, x in enumerate(base) if x] + [0]) + 1
        size = [(base[g + 1] if g + 1 < ng else len(w.dialogues)) - base[g] for g in range(ng)]
        for g in range(1, ng):
            if {dx for (k, dx) in orig[f] if k == g and dx < size[g]}:
                continue                                     # the plain scan sees a reference: not one of the flagged groups
            lit = {dx for (k, dx) in old[f] if k == g}
            allres = {dx: v for (k, dx), v in refs[f].items() if k == g}
            flagged += 1
            und_all = [u for u in unres[f] if u[0] == g]
            unk_k = [u for u in unres[f] if u[0] is None]
            st = {}
            for dx in range(size[g]):
                if dx in lit or allres.get(dx) == "tab":
                    st[dx] = "reachable"
                elif dx in allres or und_all:
                    st[dx] = "undetermined"
                else:
                    st[dx] = "unreachable"
            cnt = collections.Counter(st.values())
            print("== %s group %d (%d lines): reachable %d, undetermined %d, unreachable %d%s" % (
                f, g, size[g], cnt["reachable"], cnt["undetermined"], cnt["unreachable"],
                "  [unresolved sites with this k: %s]" % "; ".join(u[1] for u in und_all) if und_all else ""))
            if unk_k:
                print("   (file has %d call sites with unknown k, e.g. %s)" % (len(unk_k), unk_k[0][1]))
            print("   reachable dx:", [d for d in st if st[d] == "reachable"], " undetermined dx:", [d for d in st if st[d] == "undetermined"])
            for dx in range(size[g]):
                if st[dx] == "unreachable" and dx < 24:
                    i = base[g] + dx
                    es = wfm_py.words_to_text(we, we.dialogues[i].words, we.dialogues[i].font) if i < len(we.dialogues) else "?"
                    ss = wfm_py.words_to_text(w, w.dialogues[i].words, w.dialogues[i].font)
                    cl = lambda x: re.sub(r"\{[^}]*\}", "", x).replace("\n", " ")[:110]
                    print("   UNREACHABLE dx %d (#%d): ES %r | EN %r%s" % (dx, i, cl(ss), cl(es), ("  [sign-table entry %d exists, placed in no room of this file's areas]" % SIGN_ALL[(g, dx)]) if (g, dx) in SIGN_ALL else ""))
    print("%d groups without a literal reference" % flagged)
    print("WSS op 0x85 (message) with variable args, per scene file: %s" % dict(WSS_VAR))



def main():
    if "--resolve" in sys.argv:
        return resolve_report(set(a for a in sys.argv[1:] if not a.startswith("--")))
    want = set(a.split("/")[-1] for a in sys.argv[1:] if not a.startswith("--"))
    refs, unk = collections.defaultdict(set), collections.defaultdict(int)
    for p in sorted(glob.glob(FS + "/AREA*/X0??.BIN")):
        a = int(os.path.basename(p)[2:4])
        r, u = call_refs(p)
        r0 = set(r)
        r = r | src_refs(a)
        if "--delta" in sys.argv:
            print("X%03d: %d static-call refs + %d from sources = %d" % (a, len(r0), len(r - r0), len(r)))
        for f in files_of(a):
            refs[f] |= r
            unk[f] += u
    # WSS scenes (AREA15): scene prefix -> file (INFERRED from the numbering, not verified)
    for p in sorted(glob.glob(FS + "/AREA15/*.WSS")):
        b = os.path.basename(p)
        pre = 1 if b.startswith("SCR01") else int(b[:2])
        for f in files_of(pre)[:1]:
            refs[f] |= wss_refs(p)
    for f in sorted(refs):
        if want and f not in want:
            continue
        area = [p for p in glob.glob(FS + "/AREA*/" + f + ".WFM")][0]
        w = wfm_py.decode(open(area, "rb").read())
        base = list(struct.unpack("<64H", w.reserved))
        ng = max([i for i, x in enumerate(base) if x] + [0]) + 1
        size = [(base[g + 1] if g + 1 < ng else len(w.dialogues)) - base[g] for g in range(ng)]
        hit = set()
        for k, dx in refs[f]:
            if 0 < k < ng and dx < size[k]:
                hit.add(base[k] + dx)
        print("== %s: %d dialogues, group bases %s, %d computed-arg call sites unseen" % (f, len(w.dialogues), base[:ng], unk[f]))
        for g in range(1, ng):
            allg = range(base[g], base[g] + size[g])
            mine = [i for i in allg if i in hit]
            miss = [i for i in allg if i not in hit]
            tag = "no static reference at all" if not mine else "%d/%d referenced" % (len(mine), size[g])
            print("  group %d (%d dialogues): %s; unreferenced dx %s" % (g, size[g], tag, [i - base[g] for i in miss][:40]))
            for i in miss[:2]:
                d = w.dialogues[i]
                print("      dx %d: %r" % (i - base[g], wfm_py.words_to_text(w, d.words, d.font)[:90]))


if __name__ == "__main__":
    main()

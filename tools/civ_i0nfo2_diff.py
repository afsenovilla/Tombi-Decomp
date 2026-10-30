#!/usr/bin/env python3
"""Compare the top-down engine overlay (I0NFO2 / INFO2, kind 0xF0) of another disc with PAL. Summary only.

  python3 -I tools/civ_i0nfo2_diff.py DISC_TREE [--pal game/disc/fs/SYSTEM/I0NFO2.BIN] [--pal-exe game/MAIN0.EXE]
                                     [--exe TARGET_EXE] [--md]

1. Finds candidates: every file named *INFO2*.BIN, plus every other file (< 8 MB) that holds >= 1500 aligned 16-byte
   windows of PAL I0NFO2's code (an embedded 0xF0 block). Prints name, size, sha1, hits.
2. Per candidate: identical or not; if not, a word-level alignment (masked like tools/xmatch.py: j/jal targets, lui and
   the low 16 bits of loads/stores/addiu/ori are ignored) against PAL, reported per PAL symbol
   (config/symbol_addrs_i0nfo2.txt + notes/data_names_i0nfo2.csv): symbols missing/changed, inserted target words.
   Civ functions (A11/A16/A17/A18, NpcSet0D..10, SectionStepC/D/E) also list immediate-level differences.
3. Civ tables re-parsed through the address mapping: NPC_SET_0D/0E/0F/10 SpawnA/SpawnB records (residents: count,
   position, patrol box, behaviour, dialogue case, subtype), A11/16/17/18 actor defs, scenery defs, anim script counts,
   A16 dialogue ids. Each is reported as identical to PAL or with the differing fields.
4. --exe: the PAL warp records of AREA11/16/17/18 (MAIN0 0x80079A48) searched byte-for-byte in the target exe.
No game data is printed beyond numbers, offsets and the differing fields. Reads only.
"""
import argparse, csv, hashlib, os, re, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
from xmatch import mask  # noqa: E402

BASE = 0x800E8028
TEXT_END = BASE + 0x16BA8
MAIN0_BASE = 0x80010000 - 0x800
CIV_FN = re.compile(r"A1[1678]|NpcSet(0D|0E|0F|10)|SectionStep[CDE]")
NPC_SETS = {0x0D: (0x80100080, [0x800FFE24]), 0x0E: (0x80100364, ["*0x801001D4", "*0x801001D8"]),
            0x0F: (0x80100784, [0x80100368]), 0x10: (0x801008D8, [])}
DEFS = [("TOPDOWN_A11_ACTOR_DEFS", 0x800FEEB8, "obj", 5), ("TOPDOWN_A16_ACTOR_DEFS", 0x800FEEF4, "obj", 4),
        ("TOPDOWN_A17_ACTOR_DEFS", 0x800FEF64, "obj", 5), ("TOPDOWN_A18_ACTOR_DEFS", 0x800FEFA0, "obj", 7),
        ("TOPDOWN_A11_SCENERY_DEFS", 0x800FF054, "obj3", 2), ("TOPDOWN_A16_SCENERY_DEFS", 0x800FEDD0, "obj3", 3),
        ("TOPDOWN_A17_SCENERY_DEFS", 0x800FEE04, "obj3", 3)]
A_FIELDS = ("type", "room", "frame", "anim", "bx0", "by0", "bx1", "by1", "dlg", "beh", "x", "y", "sub", "bc")
B_FIELDS = ("type", "room", "w4", "anim", "x", "y", "sub", "bc")
OUT = []


def say(s=""):
    OUT.append(s)


def words(b):
    return list(struct.unpack("<%dI" % (len(b) // 4), b[:len(b) // 4 * 4]))


def symbols():
    syms = {}
    for line in open(os.path.join(ROOT, "config", "symbol_addrs_i0nfo2.txt")):
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);", line)
        if m:
            syms[int(m.group(2), 16)] = m.group(1)
    p = os.path.join(ROOT, "notes", "data_names_i0nfo2.csv")
    if os.path.exists(p):
        for r in csv.DictReader(open(p)):
            syms.setdefault(int(r["address"], 16), r["name"])
    return sorted(syms.items())


def find_candidates(tree, pal):
    win = set()
    for o in range(0xB00, 0x16BA8 - 16, 4):
        c = pal[o:o + 16]
        if c.count(0) < 12:
            win.add(c)
    out = []
    for dp, _, fs in os.walk(tree):
        for f in sorted(fs):
            p = os.path.join(dp, f)
            try:
                sz = os.path.getsize(p)
            except OSError:
                continue
            named = "INFO2" in f.upper()
            if not named and (sz < 0x4000 or sz > 8 << 20):
                continue
            b = open(p, "rb").read()
            hits = sum(1 for o in range(0, len(b) - 16, 16) if b[o:o + 16] in win)
            if named or hits >= 1500:
                out.append((p, b, hits))
    return out


def blob_region(b, pal):
    """(offset, length) of the I0NFO2-like part of a file (whole file if it is about the size of PAL's)."""
    if len(b) < len(pal) * 1.5:
        return 0, len(b)
    win = {pal[o:o + 16]: o for o in range(0xB00, 0x16BA8 - 16, 16) if pal[o:o + 16].count(0) < 12}
    offs = [o - win[b[o:o + 16]] for o in range(0, len(b) - 16, 4) if b[o:o + 16] in win]
    if not offs:
        return 0, len(b)
    start = max(0, min(offs, key=lambda s: -offs.count(s)))
    return start, min(len(b) - start, int(len(pal) * 1.2))


class Map:
    """PAL word index -> target word index from a masked SequenceMatcher alignment."""

    def __init__(self, P, T, K=6):
        self.P, self.T = P, T
        mp, mt = [mask(w) for w in P], [mask(w) for w in T]

        def uniq(m):
            c = {}
            for i in range(len(m) - K):
                k = tuple(m[i:i + K])
                c[k] = -1 if k in c else i
            return c
        up, ut = uniq(mp), uniq(mt)
        pairs = sorted((i, ut[k]) for k, i in up.items() if i >= 0 and ut.get(k, -1) >= 0)
        # longest increasing chain of j (patience sorting)
        import bisect
        tails, tidx, prev = [], [], [None] * len(pairs)
        for n, (i, j) in enumerate(pairs):
            q = bisect.bisect_left(tails, j)
            if q == len(tails):
                tails.append(j); tidx.append(n)
            else:
                tails[q] = j; tidx[q] = n
            prev[n] = tidx[q - 1] if q else None
        chain, n = [], (tidx[-1] if tidx else None)
        while n is not None:
            chain.append(pairs[n]); n = prev[n]
        self.fwd, usedj = {}, set()
        for i, j in reversed(chain):
            for d in (1, -1):
                a, b = (i, j) if d == 1 else (i - 1, j - 1)
                while 0 <= a < len(mp) and 0 <= b < len(mt) and mp[a] == mt[b] and a not in self.fwd and b not in usedj:
                    self.fwd[a] = b; usedj.add(b); a += d; b += d
        # drop non-monotone leftovers, then build difflib-style opcodes
        keep, last = {}, -1
        for i in sorted(self.fwd):
            if self.fwd[i] > last:
                keep[i] = self.fwd[i]; last = self.fwd[i]
        self.fwd = keep
        ops, i, j = [], 0, 0
        for a in sorted(keep):
            b = keep[a]
            if a > i or b > j:
                ops.append(("replace" if a > i and b > j else "delete" if a > i else "insert", i, a, j, b))
            if ops and ops[-1][0] == "equal" and ops[-1][2] == a and ops[-1][4] == b:
                ops[-1] = ("equal", ops[-1][1], a + 1, ops[-1][3], b + 1)
            else:
                ops.append(("equal", a, a + 1, b, b + 1))
            i, j = a + 1, b + 1
        if i < len(P) or j < len(T):
            ops.append(("replace" if i < len(P) and j < len(T) else "delete" if i < len(P) else "insert", i, len(P), j, len(T)))
        self.ops = ops

    def addr(self, pa, tbase):
        i = (pa - BASE) // 4
        if i in self.fwd:
            return tbase + 4 * self.fwd[i] + (pa - BASE) % 4
        return None


def symbol_report(m, syms, pal, tgt, tbase):
    P, T = m.P, m.T
    ranges = []
    for n, (a, name) in enumerate(syms):
        end = syms[n + 1][0] if n + 1 < len(syms) else BASE + len(pal)
        ranges.append((a, end, name))
    status = {}
    ins = []
    for tag, i1, i2, j1, j2 in m.ops:
        if tag == "equal":
            continue
        if i2 > i1:
            for a, e, name in ranges:
                lo, hi = (a - BASE) // 4, (e - BASE + 3) // 4
                if lo < i2 and hi > i1:
                    status.setdefault(name, [0, 0, a, e])[0] += min(hi, i2) - max(lo, i1)
        if j2 > j1 and i2 == i1:
            ins.append((BASE + 4 * i1, tbase + 4 * j1, j2 - j1))
    # masked-equal but raw-different words (immediates / relocations)
    for i, j in m.fwd.items():
        if P[i] != T[j]:
            pa = BASE + 4 * i
            for a, e, name in ranges:
                if a <= pa < e:
                    status.setdefault(name, [0, 0, a, e])[1] += 1
                    break
    return ranges, status, ins


def reloc_deltas(m):
    """imm16 deltas seen >= 3 times over the whole file: moved globals (relocation), not real changes."""
    from collections import Counter
    c = Counter()
    for i, j in m.fwd.items():
        if m.P[i] != m.T[j] and (m.P[i] >> 26) not in (2, 3, 0x0F):
            c[(m.T[j] - m.P[i]) & 0xFFFF] += 1
    return {d for d, n in c.items() if n >= 3}


def imm_diffs(m, a, e, rel, limit=8):
    out = []
    for i in range((a - BASE) // 4, (e - BASE) // 4):
        j = m.fwd.get(i)
        if j is not None and m.P[i] != m.T[j]:
            if (m.P[i] >> 26) in (2, 3, 0x0F) or ((m.T[j] - m.P[i]) & 0xFFFF) in rel:
                continue  # j/jal target, lui, or a common relocation delta
            out.append("+0x%X %08X->%08X" % (4 * i - (a - BASE), m.P[i], m.T[j]))
    return out[:limit], len(out)


def u32(b, base, a):
    o = a - base
    return struct.unpack_from("<I", b, o)[0] if 0 <= o <= len(b) - 4 else None


def parse_list(b, base, p, n, fields, maxn=64):
    out = []
    while p is not None and 0 <= p - base <= len(b) - 2 * n and len(out) < maxn:
        r = struct.unpack_from("<%dh" % n, b, p - base)
        if r[0] == 0xFF:
            return out
        out.append(dict(zip(fields, r)))
        p += 2 * n
    return None


def cmp_records(label, pr, tr):
    if tr is None:
        say("- %s: not parsable in target" % label)
        return
    if pr == tr:
        say("- %s: identical (%d records)" % (label, len(pr)))
        return
    say("- %s: DIFFERENT, PAL %d records vs target %d" % (label, len(pr), len(tr)))
    for i in range(max(len(pr), len(tr))):
        a = pr[i] if i < len(pr) else None
        c = tr[i] if i < len(tr) else None
        if a == c:
            continue
        if a is None or c is None:
            say("  - #%d only in %s: %s" % (i, "target" if a is None else "PAL", c or a))
        else:
            say("  - #%d %s" % (i, ", ".join("%s %d->%d" % (k, a[k], c[k]) for k in a if a[k] != c[k])))


def civ_tables(m, pal, tgt, tbase):
    say("**Civ tables (through the alignment)**")
    for var, (pa, pbs) in sorted(NPC_SETS.items()):
        pl = u32(pal, BASE, pa)
        ta = m.addr(pa, tbase)
        tl = u32(tgt, tbase, ta) if ta else m.addr(pl, tbase)
        pr = parse_list(pal, BASE, pl, 14, A_FIELDS)
        tr = parse_list(tgt, tbase, tl, 14, A_FIELDS) if tl else None
        cmp_records("NPC_SET_%02X SpawnA (residents) PAL list %08X / target %s" % (var, pl, "%08X" % tl if tl else "unmapped"),
                    pr, tr)
        for pb in pbs:
            if isinstance(pb, str):
                v = int(pb[1:], 16)
                pp = u32(pal, BASE, v)
                tv = m.addr(v, tbase)
                tp = u32(tgt, tbase, tv) if tv else m.addr(pp, tbase)
            else:
                pp, tp = pb, m.addr(pb, tbase)
            cmp_records("NPC_SET_%02X SpawnB (props) PAL %08X" % (var, pp), parse_list(pal, BASE, pp, 8, B_FIELDS),
                        parse_list(tgt, tbase, tp, 8, B_FIELDS) if tp else None)
    for name, pa, kind, n in DEFS:
        ta = m.addr(pa, tbase)
        if ta is None:
            say("- %s: unmapped" % name)
            continue
        diffs = []
        for i in range(n):
            po, to = pa - BASE + 12 * i, ta - tbase + 12 * i
            ph, th = pal[po:po + 8], tgt[to:to + 8]
            pn = scripts(pal, BASE, struct.unpack_from("<I", pal, po + 8)[0])
            tn = scripts(tgt, tbase, struct.unpack_from("<I", tgt, to + 8)[0])
            if ph != th or pn != tn:
                diffs.append("[%d] head %s->%s scripts %s->%s" % (i, ph.hex(), th.hex(), pn, tn))
        say("- %s (%d): %s" % (name, n, "identical (incl. anim script counts/steps)" if not diffs else "; ".join(diffs)))
    pa = 0x800FEF24
    ta = m.addr(pa, tbase)
    if ta:
        say("- TOPDOWN_A16_ACTOR_DIALOGUE_IDS (64 B): %s" % (
            "identical" if pal[pa - BASE:pa - BASE + 64] == tgt[ta - tbase:ta - tbase + 64] else "DIFFERENT"))


def scripts(b, base, p):
    """(script count, steps hashed, hash of frame/control halfwords of up to 16 steps per script) of an anim pointer table."""
    n, steps, h = 0, 0, hashlib.sha1()
    while n < 24:
        v = u32(b, base, p + 4 * n) if p else None
        if v is None or not base <= v < base + len(b) - 8:
            break
        if base <= struct.unpack_from("<I", b, v - base)[0] < base + len(b):
            break  # points at another pointer table, not at a script
        k = 0  # 16 steps max; stop at a word that is an internal pointer (the next table, not a step)
        while k < 16 and v - base + 8 * k + 8 <= len(b):
            if base <= struct.unpack_from("<I", b, v - base + 8 * k)[0] < base + len(b):
                break
            k += 1
        for q in range(k):  # frame and control halfwords (the middle word may be a pointer)
            o = v - base + 8 * q
            h.update(b[o:o + 2] + b[o + 6:o + 8])
        steps += k
        n += 1
    return (n, steps, h.hexdigest()[:8])


def warps(pal_exe, exe):
    say("**Warp records (PAL MAIN0 0x80079A48) of AREA11/16/17/18 in the target exe**")
    tab = [struct.unpack_from("<I", pal_exe, 0x80079A48 - MAIN0_BASE + 4 * a)[0] for a in range(20)]
    starts = sorted(set(x for x in tab if 0x80070000 < x < 0x80090000))
    for area in (11, 16, 17, 18):
        t = tab[area]
        res = []
        for s in range(12):
            p = struct.unpack_from("<I", pal_exe, t - MAIN0_BASE + 4 * s)[0]
            if not 0x80070000 < p < 0x80090000 or (s and t + 4 * s in tab):
                break
            nxt = [q for q in starts if q > p]
            n = min((nxt[0] - p) // 8 if nxt else 4, 8)
            rec = pal_exe[p - MAIN0_BASE:p - MAIN0_BASE + 8 * n]
            res.append("s%d %s" % (s, "found x%d" % exe.count(rec) if exe.count(rec) else "NOT FOUND (%d recs)" % n))
            starts.append(p)
            starts.sort()
        say("- AREA%02d: %s" % (area, ", ".join(res)))


def load_base(m, lp, lt):
    """Target link address: from aligned data words that hold an internal pointer in PAL (value maps through the
    alignment), the most common (target value - mapped offset)."""
    from collections import Counter
    c = Counter()
    for i, j in m.fwd.items():
        pv, tv = m.P[i], m.T[j]
        if BASE <= pv < BASE + lp and pv % 4 == 0:
            k = (pv - BASE) // 4
            if k in m.fwd:
                c[tv - 4 * m.fwd[k]] += 1
    return c.most_common(1)[0][0] if c else BASE


def analyse(path, b, pal, syms, tree):
    off, ln = blob_region(b, pal)
    tgt = b[off:off + ln]
    rel = os.path.relpath(path, tree)
    say("### %s (offset 0x%X, 0x%X bytes; PAL 0x%X)" % (rel, off, ln, len(pal)))
    if tgt == pal:
        say("Byte-identical to PAL I0NFO2.BIN.")
        return
    m = Map(words(pal), words(tgt))
    eq = sum(i2 - i1 for t, i1, i2, _, _ in m.ops if t == "equal")
    raw = sum(1 for i, j in m.fwd.items() if m.P[i] == m.T[j])
    tbase = load_base(m, len(pal), len(tgt))
    say("Target link base 0x%08X (from internal pointers; PAL 0x%08X)." % (tbase, BASE))
    say("Not identical. Masked-aligned words: %d of PAL %d (%.1f%%), raw-equal among them %d. Target words %d." % (
        eq, len(m.P), 100.0 * eq / len(m.P), raw, len(m.T)))
    ranges, status, ins = symbol_report(m, syms, pal, tgt, tbase)
    fn = [r for r in ranges if r[0] < TEXT_END]
    gone = [n for a, e, n in fn if n in status and status[n][0] >= (e - a) // 4]
    chg = [n for a, e, n in fn if n in status and 0 < status[n][0] < (e - a) // 4]
    immc = [n for a, e, n in fn if n in status and status[n][0] == 0]
    say("PAL code symbols: %d; fully unaligned %d, partly changed %d, immediates/relocs only %d, untouched %d." % (
        len(fn), len(gone), len(chg), len(immc), len(fn) - len(gone) - len(chg) - len(immc)))
    if gone:
        say("- unaligned: " + ", ".join(gone[:40]) + (" ..." if len(gone) > 40 else ""))
    if chg:
        say("- partly changed: " + ", ".join("%s(%d w)" % (n, status[n][0]) for n in chg[:40]))
    tins = [x for x in ins if x[0] < TEXT_END]
    say("Target-only code insertions: %d runs, %d words%s" % (
        len(tins), sum(x[2] for x in tins),
        (" (largest: " + ", ".join("%d w after PAL %08X" % (x[2], x[0]) for x in sorted(tins, key=lambda x: -x[2])[:6]) + ")") if tins else ""))
    rel = reloc_deltas(m)
    say("**Civ functions** (unaligned words / immediates that are not a common relocation delta; %d deltas treated as relocation)" % len(rel))
    nciv, nlist = 0, 0
    for a, e, n in fn:
        if CIV_FN.search(n):
            nciv += 1
            ch = status.get(n, [0, 0])
            d, tot = imm_diffs(m, a, e, rel)
            if ch[0] == 0 and tot == 0:
                continue
            nlist += 1
            say("- %s: %d unaligned, %d odd immediates%s" % (n, ch[0], tot, (" " + " ".join(d)) if d else ""))
    say("(%d civ functions in PAL, %d listed; the rest match after relocation)" % (nciv, nlist))
    dat = [r for r in ranges if r[0] >= TEXT_END]
    dch = [(n, status[n]) for a, e, n in dat if n in status]
    say("PAL data symbols changed: %d of %d%s" % (len(dch), len(dat), (": " + ", ".join(
        "%s(%d unal/%d raw)" % (n, s[0], s[1]) for n, s in dch[:30])) if dch else ""))
    dins = [x for x in ins if x[0] >= TEXT_END]
    say("Target-only data insertions: %d runs, %d words" % (len(dins), sum(x[2] for x in dins)))
    civ_tables(m, pal, tgt, tbase)


def main():
    p = argparse.ArgumentParser()
    p.add_argument("tree")
    p.add_argument("--pal", default=os.path.join(ROOT, "game", "disc", "fs", "SYSTEM", "I0NFO2.BIN"))
    p.add_argument("--pal-exe", default=os.path.join(ROOT, "game", "MAIN0.EXE"))
    p.add_argument("--exe")
    p.add_argument("--title", default="")
    a = p.parse_args()
    pal = open(a.pal, "rb").read()
    syms = symbols()
    say("## I0NFO2 vs PAL %s" % a.title)
    cands = find_candidates(a.tree, pal)
    say("Candidates (name, size, sha1, PAL code 16-byte window hits):")
    if not cands:
        say("- none: no *INFO2* file and no file with >= 1500 window hits")
    seen = {}
    for path, b, hits in cands:
        h = hashlib.sha1(b).hexdigest()
        say("- %s 0x%X %s hits %d%s" % (os.path.relpath(path, a.tree), len(b), h[:12], hits,
                                        " (same as %s)" % seen[h] if h in seen else ""))
        seen.setdefault(h, os.path.relpath(path, a.tree))
    done = set()
    for path, b, hits in cands:
        h = hashlib.sha1(b).hexdigest()
        if h in done:
            continue
        done.add(h)
        analyse(path, b, pal, syms, a.tree)
    if a.exe and os.path.exists(a.pal_exe):
        e = open(a.exe, "rb").read()
        warps(open(a.pal_exe, "rb").read(), e[0x800:] if e[:8] == b"PS-X EXE" else e)
    print("\n".join(OUT))


if __name__ == "__main__":
    main()

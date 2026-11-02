#!/usr/bin/env python3
"""PAL (Tombi!, SCES-01331) vs USA (Tomba!, SCUS-94236) code diff, function by function, to find what the PAL build
changed for 50 Hz (docs/PORT.md "60 Hz mode"). Reads only; prints a report.

Every PAL function with a size in config/symbol_addrs_<module>.txt (main0, x000..x019 and the menu overlays d0spsub*,
i0nfo*, o0ptsub0, g0over) is looked up in the matching USA file:
  1. masked twin (tools/xmatch.py masking: jal/j targets, lui, non-$sp load/store/addiu/ori offsets zeroed): the
     words that still differ are classified; relocations (j/jal, address lui, the low half after an address lui,
     $gp offsets) are dropped, what is left is a changed constant (li, slti/sltiu/andi/xori, shift amount, small lui,
     addiu on a non-address register) or a changed struct offset;
  2. no masked twin: the PAL function is aligned (difflib on masked words) against the USA bytes between the twins
     of its PAL neighbours, and the inserted / deleted / replaced instructions plus the changed constants are printed.
Constants whose USA/PAL ratio is 6/5 (50 -> 60 Hz) and division magic numbers (lui+ori before multu/mult) are
flagged `TIMING?`.

  python3 tools/isoext.py game_usa/tomba_us.bin US_FS          (once)
  python3 -I tools/ntsc_diff.py US_FS [--module x013] [--all]  (--all also prints functions with only offset changes)
"""
import argparse, collections, difflib, os, re, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import xmatch as X  # noqa: E402

try:
    import rabbitizer

    def dis(w):
        return rabbitizer.Instruction(w, 0).disassemble()
except ImportError:  # pragma: no cover
    def dis(w):
        return "%08x" % w

MODS = {"main0": ("game/MAIN0.EXE", "SCUS_942.36"), "d0spsub": (None, "SYSTEM/DSPSUB.BIN"),
        "d0spsub2": (None, "SYSTEM/DSPSUB2.BIN"), "d0spsub3": (None, "SYSTEM/DSPSUB3.BIN"),
        "g0over": (None, "SYSTEM/GOVER.BIN"), "i0nfo": (None, "SYSTEM/INFO.BIN"), "i0nfo2": (None, "SYSTEM/INFO2.BIN"),
        "i0nfo3": (None, "SYSTEM/INFO3.BIN"), "o0ptsub0": (None, "SYSTEM/OPTSUB00.BIN")}
for _n in range(20):
    MODS["x0%02d" % _n] = (None, "SYSTEM/X%02d.BIN" % _n)

OPN = {8: "addi", 9: "addiu", 10: "slti", 11: "sltiu", 12: "andi", 13: "ori", 14: "xori", 15: "lui"}


def s16(x):
    return x - 0x10000 if x & 0x8000 else x


def load(path):
    b = open(path, "rb").read()
    return b[0x800:] if b[:8] == b"PS-X EXE" else b


def pal_funcs(mod):
    y = os.path.join(ROOT, "config", mod + ".yaml")
    sym = os.path.join(ROOT, "config", "symbol_addrs_%s.txt" % mod)
    if not os.path.exists(y) or not os.path.exists(sym):
        return []
    yt = open(y).read()
    path = MODS[mod][0] or re.search(r"target_path:\s*(\S+)", yt).group(1)
    vram = 0x80010000 if mod == "main0" else int(re.search(r"vram:\s*(0x[0-9A-Fa-f]+)", yt).group(1), 16)
    if not os.path.exists(os.path.join(ROOT, path)):
        return []
    b = load(os.path.join(ROOT, path))
    out = []
    for line in open(sym):
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func.*size:0x([0-9A-Fa-f]+)", line)
        if m:
            a, s = int(m.group(2), 16), int(m.group(3), 16)
            off = a - vram
            if s >= 8 and 0 <= off and off + s <= len(b):
                out.append((mod, m.group(1), a, s, b[off:off + s]))
    return out


def classify(i, pw, u):
    """PAL word pw[i] vs USA word u: None for a relocation, else a tuple describing the changed constant/offset
    ('struct' if the encodings differ)."""
    p = pw[i]
    op = p >> 26
    if op != u >> 26:
        return ("struct",)
    if op in (2, 3):
        return None
    if op == 0:
        if (p & 0x3F) == (u & 0x3F) and (p & ~0x7C0) == (u & ~0x7C0):
            return ("shamt", (p >> 6) & 31, (u >> 6) & 31)
        return ("struct",)
    if (p >> 16) != (u >> 16):
        return ("struct",)
    rs, ip, iu = (p >> 21) & 31, p & 0xFFFF, u & 0xFFFF
    if op == 15:
        return None if ip >= 0x7F00 or iu >= 0x7F00 else ("lui", ip, iu)
    if rs == 28:
        return None
    if op in (8, 9) and rs == 0:
        return ("li", s16(ip), s16(iu))
    if op == 13 and rs == 0:
        return ("li", ip, iu)
    if op in (10, 11):
        return (OPN[op], s16(ip), s16(iu))
    if op in (12, 14):
        return (OPN[op], ip, iu)
    for q in reversed(pw[max(0, i - 12):i]):  # low half of an address built by lui
        if q >> 26 == 15 and (q >> 16) & 31 == rs and (q & 0xFFFF) >= 0x7F00:
            return None
    if op in (13,):
        return ("ori", ip, iu)
    if op in (8, 9):
        return ("addiu", s16(ip), s16(iu))
    if op in (1, 4, 5, 6, 7):
        return None
    return ("off", op, s16(ip), s16(iu))


def magic_divisors(words):
    """{index: divisor} for lui/ori magic constants followed by a multu/mult (the compiler's division by a constant)."""
    out = {}
    for i in range(len(words) - 1):
        w = words[i]
        if w >> 26 == 15:
            rt = (w >> 16) & 31
            for j in range(i + 1, min(i + 4, len(words))):
                v = words[j]
                if v >> 26 == 13 and (v >> 21) & 31 == rt:
                    magic = ((w & 0xFFFF) << 16) | (v & 0xFFFF)
                    for k in range(j + 1, min(j + 8, len(words))):
                        x = words[k]
                        if x >> 26 == 0 and (x & 0x3F) in (0x18, 0x19):
                            out[i] = magic
                            break
                    break
    return out


def timing_hint(c):
    if c[0] in ("li", "addiu", "slti", "sltiu", "lui") and c[1] and c[2]:
        r = c[2] / c[1]
        if abs(r - 1.2) < 0.02 or abs(r - 1 / 1.2) < 0.02:
            return True
    return False


def fmt(c):
    return "%s %s->%s" % (c[0], c[-2], c[-1]) if c[0] != "struct" else "struct"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("usfs")
    ap.add_argument("--module", nargs="*")
    ap.add_argument("--all", action="store_true")
    a = ap.parse_args()
    mods = a.module or sorted(MODS, key=lambda m: (m != "main0", m))
    x000 = {tuple(X.mask(w) for w in X.words(fb)) for _, _, _, _, fb in pal_funcs("x000")}
    for mod in mods:
        funcs = pal_funcs(mod)
        upath = os.path.join(a.usfs, MODS[mod][1])
        if not funcs or not os.path.exists(upath):
            continue
        ub = load(upath)
        uw = X.words(ub)
        um = [X.mask(w) for w in uw]
        idx = collections.defaultdict(list)
        for i in range(len(um) - 4):
            idx[tuple(um[i:i + 4])].append(i)
        cov = bytearray(len(um))
        twin = {}
        for f in sorted(funcs, key=lambda f: -f[3]):
            pw = X.words(f[4])
            mw = tuple(X.mask(w) for w in pw)
            n = len(mw)
            for i in idx.get(mw[:4], ()):
                if tuple(um[i:i + n]) == mw and not any(cov[i:i + n]):
                    cov[i:i + n] = b"\1" * n
                    twin[f[1]] = i
                    break
        # the X000 code every area overlay repeats (same masked words as an X000 function): reported once, under x000
        engine_dup = lambda f: mod not in ("x000", "main0") and tuple(X.mask(w) for w in X.words(f[4])) in x000
        print("## %s (%s): %d functions, %d masked twins in %s" % (mod, MODS[mod][0] or "PAL", len(funcs), len(twin), MODS[mod][1]))
        order = sorted(funcs, key=lambda f: f[2])
        for k, f in enumerate(order):
            _, name, addr, size, fb = f
            pw = X.words(fb)
            lines, consts = [], []
            if engine_dup(f):
                continue
            if name in twin:
                i0 = twin[name]
                qw = uw[i0:i0 + len(pw)]
                for i in range(len(pw)):
                    if pw[i] != qw[i]:
                        c = classify(i, pw, qw[i])
                        if c and (c[0] != "off" or a.all):
                            consts.append((i, c))
                pd, ud = magic_divisors(pw), magic_divisors(qw)
                for i in pd:
                    if i in ud and pd[i] != ud[i]:
                        consts.append((i, ("magic", "%08X" % pd[i], "%08X" % ud[i])))
                for i, c in consts:
                    tag = " TIMING?" if c[0] == "magic" or timing_hint(c) else ""
                    lines.append("    +%04x %-26s %s%s" % (i * 4, fmt(c), dis(pw[i]) if c[0] != "magic" else "", tag))
                if lines:
                    print("  %s %08X (%d B), twin at US+%X:" % (name, addr, size, i0 * 4))
                    print("\n".join(lines))
                continue
            prev = next((g for g in reversed(order[:k]) if g[1] in twin), None)
            nxt = next((g for g in order[k + 1:] if g[1] in twin), None)
            us = twin[prev[1]] * 4 + prev[3] if prev else max(0, (twin[nxt[1]] * 4 if nxt else 0) - (nxt[2] - addr if nxt else 0) - 0x800)
            ue = twin[nxt[1]] * 4 if nxt else min(len(ub), us + size + 0x800)
            if ue <= us:
                print("  %s %08X (%d B): no USA counterpart found" % (name, addr, size))
                continue
            qw = X.words(ub[us:ue])
            sm = difflib.SequenceMatcher(None, [X.mask(w) for w in pw], [X.mask(w) for w in qw], autojunk=False)
            ops = sm.get_opcodes()
            while ops and ops[0][0] == "insert":
                ops = ops[1:]
            while ops and ops[-1][0] == "insert":
                ops = ops[:-1]
            eq = sum(i2 - i1 for t, i1, i2, _, _ in ops if t == "equal")
            for t, i1, i2, j1, j2 in ops:
                if t == "equal":
                    for d in range(i2 - i1):
                        if pw[i1 + d] != qw[j1 + d]:
                            c = classify(i1 + d, pw, qw[j1 + d])
                            if c and c[0] not in ("struct", "off"):
                                tag = " TIMING?" if timing_hint(c) else ""
                                lines.append("    ~%04x %-26s %s%s" % ((i1 + d) * 4, fmt(c), dis(pw[i1 + d]), tag))
                    continue
                if t == "replace" and i2 - i1 == j2 - j1 and all(
                        pw[x] >> 26 == qw[y] >> 26 and pw[x] >> 26 in (1, 4, 5, 6, 7) for x, y in zip(range(i1, i2), range(j1, j2))):
                    continue  # branch displacements only
                lines.append("    %s PAL+%04x US+%04x" % (t, i1 * 4, j1 * 4))
                lines += ["      - " + dis(pw[x]) for x in range(i1, i2)]
                lines += ["      + " + dis(qw[x]) for x in range(j1, j2)]
            pd, ud = magic_divisors(pw), magic_divisors(qw)
            if sorted(pd.values()) != sorted(ud.values()):
                lines.append("    magic divisors PAL %s US %s TIMING?" % (
                    ["%08X" % v for v in pd.values()], ["%08X" % v for v in ud.values()]))
            if lines:
                print("  %s %08X (%d B), no masked twin, %d%% aligned:" % (name, addr, size, 100 * eq // max(1, len(pw))))
                print("\n".join(lines))


if __name__ == "__main__":
    main()

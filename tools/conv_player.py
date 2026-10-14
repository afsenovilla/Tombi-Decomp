#!/usr/bin/env python3
"""Replace raw D_800A60xx (offset 0..0xBF of PLAYER 0x800A6038) by PLAYER.obj.<field> in the given src files.
Usage: conv_player.py files...   (verify afterwards with tools/objcmp.py)"""
import re, sys, os
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BASE = 0x800A6038
fields = []  # (off, name, size, type)
for l in open(os.path.join(ROOT, "include/tobj.h")):
    m = re.match(r"\s*(unsigned char|short|unsigned short|int|void \*|Fix16 \*|Fix16)\s+(\w+);\s*/\* \+0x([0-9a-f]+)", l)
    if not m or m.group(2).startswith("_pad"): continue
    t, n, off = m.groups()
    sz = {"unsigned char": 1, "short": 2, "unsigned short": 2, "int": 4, "void *": 4, "Fix16 *": 4, "Fix16": 4}[t]
    fields.append((int(off, 16), n, sz, t))
fields.sort()
SCAL = {"unsigned char": (1, 0), "u_char": (1, 0), "u8": (1, 0), "char": (1, 0), "U8": (1, 0), "uchar": (1, 0),
        "short": (2, 1), "s16": (2, 1), "S16": (2, 1), "unsigned short": (2, 0), "u16": (2, 0), "U16": (2, 0), "ushort": (2, 0), "u_short": (2, 0),
        "int": (4, 1), "s32": (4, 1), "S32": (4, 1), "long": (4, 1), "unsigned": (4, 0), "unsigned int": (4, 0), "u32": (4, 0), "U32": (4, 0), "uint": (4, 0), "unsigned long": (4, 0), "u_long": (4, 0), "u_int": (4, 0)}
FSIGN = {"unsigned char": (1, 0), "short": (2, 1), "unsigned short": (2, 0), "int": (4, 1)}


def base_of(off):
    """(address-of expression text, offset delta within it) of the field containing off, or None."""
    fo, n, sz, ft = max((x for x in fields if x[0] <= off), key=lambda x: x[0])
    if ft == "Fix16":
        if off == fo: return "PLAYER.obj.%s.raw" % n, "int", 0
        if off == fo + 2: return "PLAYER.obj.%s.p.whole" % n, "short", 0
        return None
    return "PLAYER.obj." + n, ft, off - fo


def lval(off, sig, ctype):
    b = base_of(off)
    if b is None: return None
    e, ft, delta = b
    if delta == 0 and sig is not None and FSIGN.get(ft) == sig: return e
    if delta: return "(*(%s*)((char*)&%s + %d))" % (ctype, e, delta)
    return "(*(%s*)&%s)" % (ctype, e)


DIRECT = "--direct" in sys.argv


def direct_ok(off, btype, st, ar):
    if st: return False
    b = base_of(off)
    return b is not None and b[2] == 0 and FSIGN.get(b[1]) == SCAL.get(btype)


def conv(path):
    s = open(path, errors="surrogateescape").read()
    decl = {}  # name -> (ctype, isarray, sig, off)
    out = []
    for ln in s.split("\n"):
        m = re.match(r"^(\s*)extern\s+([^;(]*);\s*$", ln)
        if m and re.search(r"\bD_800A60[0-9A-F]{2}\b", m.group(2)):
            pieces = [p.strip() for p in m.group(2).split(",")]
            fm = re.match(r"^(.*?)(\**)\s*(\w+)\s*(\[[^\]]*\])?$", pieces[0], re.S)
            parsed, ok = [], bool(fm)
            if fm:
                btype = fm.group(1).strip()
                parsed.append((fm.group(2), fm.group(3), fm.group(4)))
                for p in pieces[1:]:
                    pm = re.match(r"^(\**)\s*(\w+)\s*(\[[^\]]*\])?$", p)
                    if not pm: ok = False; break
                    parsed.append(pm.groups())
            if ok:
                keep = []
                for st, nm, ar in parsed:
                    mm = re.fullmatch(r"D_800A60([0-9A-F]{2})", nm)
                    off = 0x800A6000 + int(mm.group(1), 16) - BASE if mm else -1
                    if mm and 0 <= off < 0xC0 and (btype in SCAL or st) and not (st and ar) and (not DIRECT or direct_ok(off, btype, st, ar)):
                        decl[nm] = (btype + (" " + st if st else ""), ar is not None, SCAL.get(btype) if not st else None, off)
                    else:
                        keep.append((st, nm, ar))
                if len(keep) != len(parsed):
                    if keep:
                        out.append("%sextern %s %s;" % (m.group(1), btype, ", ".join("%s%s%s" % (a, b, c or "") for a, b, c in keep)))
                    continue
        out.append(ln)
    if not decl: return False
    body = "\n".join(out)

    def rep(m):
        nm = m.group(1)
        if nm not in decl: return m.group(0)
        ctype, isarr, sig, off = decl[nm]
        if isarr:
            b = base_of(off)
            if b is None: return m.group(0)
            e, ft, delta = b
            a = "&" + e
            if delta: a = "((char*)%s + %d)" % (a, delta)
            return "((%s*)%s)" % (ctype, a)
        r = lval(off, sig, ctype)
        return m.group(0) if r is None else r
    body2 = re.sub(r"\b(D_800A60[0-9A-F]{2})\b", rep, body)
    # a token whose use could not be converted but whose declaration was removed would not compile: objcmp catches it
    if "PLAYER" not in body2: return False
    if not re.search(r'#include\s+"player\.h"', body2):
        ls = body2.split("\n"); i = 0
        while i < len(ls) and ls[i].startswith("//"): i += 1
        ls.insert(i, '#include "player.h"')
        body2 = "\n".join(ls)
    open(path, "w", errors="surrogateescape").write(body2)
    return True


if __name__ == "__main__":
    for p in [a for a in sys.argv[1:] if a != '--direct']:
        if conv(p): print("conv", p)

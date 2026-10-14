#!/usr/bin/env python3
"""Replace raw D_8009C93x..D_8009C95x (GVARS block at 0x8009C930) by GVARS.<field> (include/gvars.h).
Usage: greedy_conv.py conv_gvars files...   (derived from conv_player.py)"""
import re, sys, os
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BASE = 0x8009C930
fields = [(0, "b30", 1, "unsigned char"), (4, "i34", 4, "int")]
for i, n in enumerate(range(0x38, 0x44)):
    fields.append((8 + i, "b%x" % n, 1, "unsigned char"))
fields += [(0x14, "s44", 2, "short"), (0x16, "s46", 2, "short"), (0x18, "p48", 4, "void *"), (0x1c, "p4c", 4, "void *"),
           (0x20, "p50", 4, "void *"), (0x24, "p54", 4, "void *"), (0x28, "b58", 1, "unsigned char"), (0x29, "b59", 1, "unsigned char"),
           (0x2c, "i5c", 4, "int")]
fields.sort()
SCAL = {"unsigned char": (1, 0), "u_char": (1, 0), "u8": (1, 0), "char": (1, 0), "U8": (1, 0), "uchar": (1, 0),
        "short": (2, 1), "s16": (2, 1), "S16": (2, 1), "unsigned short": (2, 0), "u16": (2, 0), "U16": (2, 0), "ushort": (2, 0), "u_short": (2, 0),
        "int": (4, 1), "s32": (4, 1), "S32": (4, 1), "long": (4, 1), "unsigned": (4, 0), "unsigned int": (4, 0), "u32": (4, 0), "U32": (4, 0), "uint": (4, 0), "unsigned long": (4, 0), "u_long": (4, 0), "u_int": (4, 0)}
FSIGN = {"unsigned char": (1, 0), "short": (2, 1), "unsigned short": (2, 0), "int": (4, 1)}


def base_of(off):
    c = [x for x in fields if x[0] <= off]
    if not c: return None
    fo, n, sz, ft = c[-1]
    if off - fo >= sz: return None
    return "GVARS." + n, ft, off - fo


def lval(off, sig, ctype):
    b = base_of(off)
    if b is None: return None
    e, ft, delta = b
    if delta == 0 and sig is not None and FSIGN.get(ft) == sig: return e
    if delta: return "(*(%s*)((char*)&%s + %d))" % (ctype, e, delta)
    return "(*(%s*)&%s)" % (ctype, e)


DIRECT = "--direct" in sys.argv
ONLY = None  # dict token name -> direct-only flag (None = all, DIRECT global)


def direct_ok(off, btype, st, ar):
    if st: return False
    b = base_of(off)
    return b is not None and b[2] == 0 and FSIGN.get(b[1]) == SCAL.get(btype)


def tokens(path):
    return sorted(set(re.findall(r"\bD_8009C9[3-5][0-9A-F]\b", open(path, errors='surrogateescape').read())))


def conv(path):
    s = open(path, errors="surrogateescape").read()
    decl = {}  # name -> (ctype, isarray, sig, off)
    out = []
    for ln in s.split("\n"):
        m = re.match(r"^(\s*)extern\s+([^;(]*);\s*$", ln)
        if m and re.search(r"\bD_8009C9[3-5][0-9A-F]\b", m.group(2)):
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
                    mm = re.fullmatch(r"D_8009C9([0-9A-F]{2})", nm)
                    off = 0x8009C900 + int(mm.group(1), 16) - BASE if mm else -1
                    if mm and (ONLY is None or nm in ONLY) and 0 <= off < 0x30 and (btype in SCAL or btype.replace("volatile ", "", 1) in SCAL or st) and not (st and ar) and (not (DIRECT if ONLY is None else ONLY.get(nm, False)) or direct_ok(off, btype, st, ar)):
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
    body2 = re.sub(r"\b(D_8009C9[3-5][0-9A-F])\b", rep, body)
    # a token whose use could not be converted but whose declaration was removed would not compile: objcmp catches it
    if "GVARS" not in body2: return False
    if not re.search(r'#include\s+"gvars\.h"', body2):
        ls = body2.split("\n"); i = 0
        while i < len(ls) and ls[i].startswith("//"): i += 1
        ls.insert(i, '#include "gvars.h"')
        body2 = "\n".join(ls)
    open(path, "w", errors="surrogateescape").write(body2)
    return True


if __name__ == "__main__":
    for p in [a for a in sys.argv[1:] if a != '--direct']:
        if conv(p): print("conv", p)

#!/usr/bin/env python3
"""Compare the USA disc (Tomba!, SCUS-94236) with the PAL one, block by block.  Findings: docs/CIVILIZATION.md, "USA disc".

The USA disc has one load-list set (SYS/LDARnn.BIN, LDSYS.BIN; PAL has LD0..LD8 per language) and its own file table in
SCUS_942.36 at file offset 0x699A0 (1054 entries, same 8-byte BCD CdlLOC + size format as MAINn, see tools/cdfiles.py).
File names were renumbered, so a name diff is meaningless: this compares the bytes of every load-list block (and every
whole file, GAM unpacked) against all PAL blocks/files and prints the USA blocks whose bytes exist nowhere on PAL.

  tools/usa_compare.py USA_IMAGE USA_FS [--lists AREA..]   new blocks; with --lists also dump those load lists
"""
import argparse, hashlib, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import area000 as A  # noqa: E402
import cdfiles as C  # noqa: E402

USA_TABLE = 0x699A0


def usa_names(img, fs):
    lay = C.layout_from_image(img)
    byk = {(l, C.tsize(n, s)): n for n, (l, s) in lay.items()}
    known = {l for l, s in lay.values()}
    d = open(os.path.join(fs, "SCUS_942.36"), "rb").read()
    names, o = {}, USA_TABLE
    while C.dec_loc(d[o:o + 4]) in known:
        names[len(names)] = byk.get((C.dec_loc(d[o:o + 4]), struct.unpack_from("<I", d, o + 4)[0]))
        o += 8
    return names


def usa_lists(img, fs):
    names, out = usa_names(img, fs), {}
    for fn in sorted(os.listdir(os.path.join(fs, "SYS"))):
        b = open(os.path.join(fs, "SYS", fn), "rb").read()
        lists, cur = [], []
        for o in range(4, len(b) - 19, 20):
            fid, slot, kind, addr, arg, w, h, typ = struct.unpack_from("<hBBIIhhI", b, o)
            if fid == -1:
                lists.append(cur)
                cur = []
            elif fid in names:
                cur.append(dict(name=names[fid], kind=kind, type=typ, addr=addr, arg=arg, w=w, h=h))
            else:
                break
        if cur:
            lists.append(cur)
        out[fn[2:-4]] = lists
    return out


def blocks(lst, fs):
    out, cur, pos = [], None, 0
    for r in lst:
        if r["type"] != 0xFFFFFFFF:
            try:
                cur = A.read_resource(r["name"], fs) if r["name"].endswith((".GAM", ".000")) else \
                    open(os.path.join(fs, r["name"]), "rb").read()
            except (OSError, ValueError):
                cur = None
            pos = 0
        if r["kind"] & 0xF0 == 0x10:
            out.append((r, cur))
            continue
        out.append((r, cur[pos:pos + r["arg"]] if cur is not None else None))
        pos = (pos + r["arg"] + 3) & ~3
    return out


def pal_hashes():
    seen = set()
    for ld, L in A.load_lists().items():
        for i in range(len(L)):
            for r, d in A.list_blocks(ld, i):
                if d is not None:
                    seen.add(hashlib.md5(d).hexdigest())
    for dp, _, fns in os.walk(A.FS):
        for f in fns:
            try:
                d = open(os.path.join(dp, f), "rb").read()
                seen.add(hashlib.md5(A.gam_unpack(d) if f.endswith(".GAM") else d).hexdigest())
            except (OSError, ValueError, IndexError):
                pass
    return seen


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("image")
    ap.add_argument("fs")
    ap.add_argument("--lists", nargs="*", default=[])
    a = ap.parse_args()
    L = usa_lists(a.image, a.fs)
    for ar in a.lists:
        for i, lst in enumerate(L["AR%02d" % int(ar)]):
            print("AR%02d list %d (%s): %d records" % (int(ar), i, "area" if i == 0 else "section %d" % (i - 1), len(lst)))
            for r in lst:
                print("    %-18s kind %02X type %X arg %d" % (r["name"], r["kind"], r["type"], r["arg"]))
    pal = pal_hashes()
    for ld, lists in L.items():
        for i, lst in enumerate(lists):
            for r, d in blocks(lst, a.fs):
                if d is not None and hashlib.md5(d).hexdigest() not in pal:
                    print("NEW %s list %d %-18s kind %02X %d bytes" % (ld, i, r["name"], r["kind"], len(d)))


if __name__ == "__main__":
    main()

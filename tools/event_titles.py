#!/usr/bin/env python3
"""Event -> event-title tables of any version, from an extracted disc tree (tools/isoext.py output) or a single exe.

Two layouts exist:
  * PAL / beta 980203: one 200-byte table (PAL MAIN0 RAM 0x80080410), shared by the 20 area slots of D_800806B4.
  * JP (SLPS-01144): one 200-byte table per area, indexing that area's own title list (WFM2 block of
    AREAnn/A00x.GAM). Areas 0,1,3,4,6,9,10,11,13,14,16,17 keep it in their X overlay, areas 2,5,8,18,19 in the exe.
    Each JP table is followed by the bytes 00 01 02 .. 09 (start of the next structure); that is how they are found.
0xFF = no title (spawnEventTitle falls back to entry 1, the dummy title).

  python3 tools/event_titles.py TREE_OR_FILE [--pal PAL_MAIN0.EXE]
Prints every table found (file, offset, highest index, events with a title), the indices 2..max that no event of
that table uses, and with --pal the events whose title differs from PAL (PAL 0xFF vs a title here is what matters).
"""
import argparse, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

SEQ = bytes(range(10))


def files_of(path):
    if os.path.isfile(path):
        yield os.path.basename(path), open(path, "rb").read()
        return
    for dp, _, fn in os.walk(path):
        for n in sorted(fn):
            if n.upper().endswith((".STR", ".XA", ".WVD", ".SEQ", ".VB", ".VH")):
                continue
            d = open(os.path.join(dp, n), "rb").read()
            if d[:4] == b"GAM\0":
                try:
                    import gam_py
                    d = bytes(gam_py.unpack(d))
                except Exception:
                    pass
            yield os.path.relpath(os.path.join(dp, n), path), d


def tables(d, exe=True):
    """(offset, 200 bytes) of every plausible event-title table."""
    seen, out = set(), []
    i = d.find(SEQ)
    while i >= 0:   # JP style
        b = d[i - 200:i] if i >= 200 else b""
        nf = [x for x in b if x != 255]
        if b and b.count(255) > 100 and nf and max(nf) < 80:
            out.append((i - 200, b)); seen.add(i - 200)
        i = d.find(SEQ, i + 1)
    for o in range(0, len(d) - 200, 4) if exe else ():   # PAL style (exe only): events 0,1 -> titles 2,3, mostly distinct
        if d[o] != 2 or d[o + 1] != 3 or o in seen:
            continue
        b = d[o:o + 200]
        nf = [x for x in b if x != 255]
        if 20 <= b.count(255) <= 120 and max(nf) < 160 and len(set(nf)) >= len(nf) - 2 and len(nf) > 100:
            out.append((o, b))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("path")
    ap.add_argument("--pal", help="PAL MAIN0.EXE (title table at RAM 0x80080410)")
    a = ap.parse_args()
    pal = None
    if a.pal and os.path.exists(a.pal):
        m = open(a.pal, "rb").read()
        pal = m[0x80080410 - int.from_bytes(m[0x18:0x1C], "little") + 0x800:][:200]
    for name, d in files_of(a.path):
        exe = os.path.basename(name).upper().startswith(("SLPS", "SCUS", "SCES", "SLPM", "MAIN0", "TOMBA"))
        for o, b in tables(d, exe):
            nf = [x for x in b if x != 255]
            unused = sorted(set(range(2, max(nf) + 1)) - set(nf))
            print("%s @0x%X: max index %d, %d events titled, indices never used: %s" % (name, o, max(nf), len(nf), unused))
            print("  " + " ".join("%d:%d" % (e, v) for e, v in enumerate(b) if v != 255))
            if pal is not None:
                if b == pal:
                    print("  == PAL table")
                else:
                    gain = [e for e in range(200) if pal[e] == 255 and b[e] != 255]
                    print("  events PAL leaves 0xFF but titled here: %s" % " ".join("%d(0x%X)" % (e, e) for e in gain))


if __name__ == "__main__":
    main()

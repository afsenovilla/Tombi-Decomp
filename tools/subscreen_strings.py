#!/usr/bin/env python3
"""The texts of a Select sub-screen overlay (item / event names, places...) found by their encoding, with no table
layout assumed: a string is a run of 16-bit glyph ids (latin-1 + 140 below 267, + 74 from there; -3 a line break)
ended by -1, as tools/subscreen_text.py decodes the retail D0SPSUB.BIN.  Made for the demo / prototype discs, whose
overlays are laid out differently (docs/BUILDS.md); on a retail overlay it lists the same names the tables hold.

  python3 tools/subscreen_strings.py FILE_OR_DIR [--min 3] [--all]

A directory is scanned for every file whose name holds SPSUB (any case).  --all prints every run, --min the shortest
run (in glyphs) worth printing.  Prints offsets and texts only; writes nothing."""
import os, struct, sys

EXTRA = {332: "¡", 333: "ª", 337: "¿"}


def glyph(x):
    if x == -3:
        return "\n"
    if x in EXTRA:
        return EXTRA[x]
    v = x - 140 if x < 267 else x - 74
    return chr(v) if 32 <= v < 0x250 else None


def strings(d, min_len=3):
    """[(offset, text)] of every -1-ended run of decodable glyphs of min_len+ letters, 2-aligned"""
    out, n = [], len(d) // 2
    vals = struct.unpack("<%dh" % n, d[:n * 2])
    i = 0
    while i < n:
        j, txt, letters = i, [], 0
        while j < n and vals[j] != -1:
            c = glyph(vals[j])
            if c is None:
                break
            txt.append(c); letters += c.strip() != ""; j += 1
        if j < n and vals[j] == -1 and letters >= min_len and j > i:
            out.append((2 * i, "".join(txt)))
            i = j + 1
        else:
            i += 1
    return out


def main(argv):
    if len(argv) < 2 or argv[1] in ("-h", "--help"):
        print(__doc__); return 2
    path, min_len, show_all = argv[1], 3, "--all" in argv
    if "--min" in argv:
        min_len = int(argv[argv.index("--min") + 1])
    files = [path] if os.path.isfile(path) else sorted(os.path.join(r, f) for r, _, fs in os.walk(path) for f in fs if "SPSUB" in f.upper())
    for f in files:
        d = open(f, "rb").read()
        found = strings(d, min_len)
        print("### %s (%d bytes): %d strings" % (os.path.relpath(f, path) if os.path.isdir(path) else f, len(d), len(found)))
        seen = set()
        for off, t in found:
            if not show_all and t in seen:
                continue
            seen.add(t)
            print("0x%05X  %s" % (off, t.replace("\n", " / ")))
        print()
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))

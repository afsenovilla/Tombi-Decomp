#!/usr/bin/env python3
"""Extracts every sprite frame of Tombi! (PAL) as PNG straight from a disc image, with no dependencies beyond
Python 3 (Windows/Linux/macOS).

  python3 tools/extract_sprites.py Tombi.bin OUT_DIR

Steps: the ISO9660 file system of the .bin is extracted to OUT_DIR/fs (tools/isoext.py), then every WPP sprite
file (AREAxx/*.000 of load kind 0x60/0x62, 40 files, about 13 000 frames) is unpacked with tools/wpp.py to
OUT_DIR/sprites/<AREA>/<file>/frame_NNNN.png (indexed PNG, index 0 transparent, Tombi's palette by default; see
wpp.py --clut/--row/--x0 for other palettes).  The retail disc is only read.
A single-file executable can be built with PyInstaller:  pyinstaller --onefile tools/extract_sprites.py
(it bundles isoext.py, area000.py and wpp.py from the same directory)."""
import os, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    img, out = sys.argv[1], os.path.abspath(sys.argv[2])
    fs = os.path.join(out, "fs")
    if not os.path.isdir(os.path.join(fs, "AREA00")):
        print("extracting the file system of", img)
        subprocess.run([sys.executable, os.path.join(HERE, "isoext.py"), img, fs], check=True)
    os.environ["TOMBI_FS"] = fs
    os.environ["TOMBI_IMG"] = os.path.abspath(img)
    sys.path.insert(0, HERE)
    import area000 as A
    import wpp
    A.FS = fs
    n = 0
    for area in sorted(os.listdir(fs)):
        d = os.path.join(fs, area)
        if not os.path.isdir(d): continue
        for f in sorted(os.listdir(d)):
            if not f.endswith(".000"): continue
            path = os.path.join(d, f)
            if not wpp.is_wpp(A.rel_name(path)): continue
            dst = os.path.join(out, "sprites", area, f[:-4])
            class Args: pass
            a = Args(); a.file, a.out, a.clut, a.row, a.x0 = path, dst, None, 0, 0
            wpp.cmd_unpack(a); n += 1
    print("done: %d sprite files under %s" % (n, os.path.join(out, "sprites")))


if __name__ == "__main__":
    main()

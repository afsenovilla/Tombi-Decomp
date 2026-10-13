#!/usr/bin/env python3
"""Extracts every sprite frame of Tombi! (PAL) as PNG straight from a disc image, with no dependencies beyond
Python 3 (Windows/Linux/macOS).

  python3 tools/extract_sprites.py Tombi.bin OUT_DIR

Steps: the ISO9660 file system of the .bin is extracted to OUT_DIR/fs (tools/isoext.py), then every WPP sprite
file (AREAxx/*.000 of load kind 0x60/0x62, 40 files, about 13 000 frames) is unpacked with tools/wpp.py to
OUT_DIR/sprites/<AREA>/<file>/frame_NNNN.png (indexed PNG, index 0 transparent, each frame with the palette the game data assigns it
(CLUT field of the SPR frame definitions, tools/wpp.py --auto; Tombi's palette where the game sets it at run time)).  The retail disc is only read.
Windows users: dist/windows/tombi-assets.exe (built by .github/workflows/assets-exe.yml) does the same with no Python:
  tombi-assets.exe Tombi.bin OUT_DIR"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
BUNDLE = getattr(sys, "_MEIPASS", HERE)   # PyInstaller one-file build: the data files live in a temp dir


def run_isoext(img, fs):
    """tools/isoext.py as a module (it reads sys.argv), so it works inside the frozen exe too."""
    src = open(os.path.join(BUNDLE, "isoext.py")).read()
    argv, sys.argv = sys.argv, ["isoext.py", img, fs]
    try:
        exec(compile(src, "isoext.py", "exec"), {"__name__": "__main__", "__file__": os.path.join(BUNDLE, "isoext.py")})
    finally:
        sys.argv = argv


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    img, out = sys.argv[1], os.path.abspath(sys.argv[2])
    fs = os.path.join(out, "fs")
    if not os.path.isdir(os.path.join(fs, "AREA00")):
        print("extracting the file system of", img)
        run_isoext(img, fs)
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
            a = Args(); a.file, a.out, a.clut, a.row, a.x0, a.auto = path, dst, None, 0, 0, True
            wpp.cmd_unpack(a); n += 1
    print("done: %d sprite files under %s" % (n, os.path.join(out, "sprites")))


if __name__ == "__main__":
    main()

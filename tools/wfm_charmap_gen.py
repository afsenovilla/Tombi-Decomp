#!/usr/bin/env python3
"""Generates tools/wfm_charmap.json: glyph bitmap hash -> character, learned from the output of tools/wfm.py.

The WFM files carry no character codes, only bitmaps. `tools/wfm.py decode` (the TombaTools fork) recognises the bitmaps
by comparing them with the reference fonts it ships; this script copies that knowledge into a small table of hashes
(height, width, SHA-1 of the 4bpp bitmap -> character) so that tools/wfm_py.py and tools/text_editor.py need neither the
fork nor the original files at run time. The table holds hashes only, no bitmaps.

  tools/wfm.py decode                      # -> build/wfm
  python3 tools/wfm_charmap_gen.py [build/wfm] [-o tools/wfm_charmap.json]
"""
import argparse, glob, json, os, sys
import yaml

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wfm_py

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("src", nargs="?", default=os.path.join(ROOT, "build", "wfm"))
    p.add_argument("-o", "--out", default=os.path.join(ROOT, "tools", "wfm_charmap.json"))
    a = p.parse_args()
    table, conflicts = {}, []
    # entries labelled by hand (accented letters, icons, ...) are kept; only the learned ones are refreshed
    if os.path.isfile(a.out):
        table.update(json.load(open(a.out, encoding="utf-8")))
    files = sorted(glob.glob(os.path.join(a.src, "*", "*", "dialogues.yaml")))
    for f in files:
        y = yaml.safe_load(open(f, encoding="utf-8"))
        for g in y.get("glyphs", []):
            ch = g.get("char")
            if not ch:
                continue
            k = wfm_py.glyph_key(g["height"], g["width"], bytes.fromhex(g["image"]))
            if table.setdefault(k, ch) != ch:
                conflicts.append((f, k, table[k], ch))
    for c in conflicts[:10]:
        print("conflict", c, file=sys.stderr)
    json.dump(dict(sorted(table.items())), open(a.out, "w", encoding="utf-8"), ensure_ascii=False, indent=0)
    print("%d bitmaps -> %d characters (%d files, %d conflicts) -> %s"
          % (len(table), len(set(table.values())), len(files), len(conflicts), a.out))
    return 1 if conflicts else 0


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""Writes config/<version>/symbol_addrs_<prog>.txt (PAL names at demo/JP addresses) from a tools/xmatch.py csv.
Version jpdemo (default) or jp (--version jp); programs and bases come from tools/versions.py.

  python3 -I tools/xmatch.py game_jpdemo/SLPM_801.65 game_jpdemo/SYSTEM/X00.BIN --csv build/jpdemo/xmatch.csv
  python3 -I tools/jpdemo_syms.py build/jpdemo/xmatch.csv
  python3 -I tools/xmatch.py game_jp/SLPS_011.44 game_jp/SYSTEM/X*.BIN --csv build/jp/xmatch.csv
  python3 -I tools/jpdemo_syms.py build/jp/xmatch.csv --version jp

Needs the PAL files and symbols only to run xmatch; the generated symbol files are committed, so the demo report
(tools/build_report.py --version jpdemo) runs from the demo disc alone. See docs/JPDEMO.md.
"""
import csv, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from versions import VERSIONS, version_arg


def main():
    ver = version_arg(sys.argv)
    # file (as passed to xmatch) -> (symbol file, vram of xmatch offset 0, first offset of the program)
    TARGETS = {f: (prog, vram, start) for prog, (_, f, vram, start) in VERSIONS[ver]["progs"].items()}
    out = {mod: [] for mod, _, _ in TARGETS.values()}
    for r in csv.DictReader(open(sys.argv[1])):
        base = os.path.basename(r["target"])
        if base not in TARGETS: continue
        mod, vram, start = TARGETS[base]
        if int(r["offset"], 16) < start: continue
        out[mod].append((vram + int(r["offset"], 16), r["name"], int(r["size"]), r["module"], r["pal_addr"], r["identical"]))
    for mod, rows in out.items():
        seen, lines = set(), []
        for a, name, size, pm, pa, ident in sorted(rows):
            # generic PAL names (func_/FUN_<PAL address>) would clash with splat's names at demo addresses
            n = "func_%08X" % a if re.fullmatch(r"(func|FUN)_[0-9A-Fa-f]{8}", name) else name
            while n in seen: n += "_"
            seen.add(n)
            lines.append("%s = 0x%08X; // type:func size:0x%X  PAL %s %s%s" % (n, a, size, pm, pa, " identical" if ident == "1" else ""))
        p = os.path.join(ROOT, "config", ver, "symbol_addrs_%s.txt" % mod)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, "w").write("\n".join(lines) + "\n")
        print(p, len(lines))


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""Genera notes/functions_<prog>.csv (address,name,size,typed) a partir de un decompilado
exportado con ExportAll (game/*_decomp.c). Solo guarda metadatos, no codigo.

Uso: python tools/export_functions.py game/MAIN0_EXE_decomp.c notes/functions_main0.csv
"""
import csv
import re
import sys


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    text = open(sys.argv[1], encoding="utf-8", errors="replace").read()
    parts = re.split(r"^// ==== (\S+) @ (\w+) size=(\d+)\n", text, flags=re.M)
    rows = []
    for i in range(1, len(parts), 4):
        name, addr, size, body = parts[i], parts[i + 1], int(parts[i + 2]), parts[i + 3]
        typed = int(bool(re.search(r"\(TObj \*", body)))
        rows.append((addr, name, size, typed))
    rows.sort(key=lambda r: int(r[0], 16))
    with open(sys.argv[2], "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["address", "name", "size", "typed"])
        w.writerows(rows)
    print("%d funciones -> %s" % (len(rows), sys.argv[2]))


if __name__ == "__main__":
    main()

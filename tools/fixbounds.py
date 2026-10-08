#!/usr/bin/env python3
"""Detecta funciones mal cortadas por Ghidra y propone fusiones.
Una funcion bien cortada termina en `jr/j/b` incondicional + delay slot. Si no, la siguiente es su continuacion.
Salida: notes/merged_todo.csv (prog,address,size,name,partes) con las cadenas fusionadas."""
import csv, os, struct
R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
def img(prog):
    if prog == "MAIN0":
        d = open(R + "/game/MAIN0.EXE", "rb").read(); return d, 0x800 - struct.unpack("<I", d[0x18:0x1c])[0]
    return open(R + "/game/AREA00/X000.BIN", "rb").read(), -0x800E8028
def w(d, off): return struct.unpack("<I", d[off:off + 4])[0] if 0 <= off <= len(d) - 4 else 0
def terminal(wd):
    op = wd >> 26
    if op == 2: return True                               # j
    if op == 0 and (wd & 0x3f) == 8: return True          # jr
    if op == 4 and ((wd >> 21) & 31) == 0 and ((wd >> 16) & 31) == 0: return True  # b (beq r0,r0)
    return False
rows = {p: [] for p in ("MAIN0", "X000")}
for p, f in (("MAIN0", "functions_main0.csv"), ("X000", "functions_x000.csv")):
    for r in csv.DictReader(open(R + "/notes/" + f)):
        rows[p].append((int(r["address"], 16), int(r["size"]), r["name"]))
    rows[p].sort()
def branch_targets(d, bias, a, s):
    """Destinos de ramas (beq/bne/blez/bgtz/bltz/bgez/b) fuera de [a, a+s)."""
    res = []
    for pc in range(a, a + s, 4):
        wd = w(d, pc + bias); op = wd >> 26
        br = op in (4, 5, 6, 7) or (op == 1 and ((wd >> 16) & 31) in (0, 1, 16, 17))
        if br:
            off = wd & 0xffff
            if off & 0x8000: off -= 0x10000
            t = pc + 4 + off * 4
            if t < a or t >= a + s: res.append(t)
    return res
out = []; frag = 0
for p in rows:
    d, bias = img(p); L = rows[p]; i = 0
    while i < len(L):
        a, s, n = L[i]; parts = [n]; end = a + s; j = i
        while j + 1 < len(L) and L[j + 1][0] == end and not terminal(w(d, end - 8 + bias)):
            j += 1; end += L[j][1]; parts.append(L[j][2])
        # ramas hacia delante que salen del rango actual y caen en una de las siguientes funciones
        ch = True
        while ch:
            ch = False
            for t in branch_targets(d, bias, a, end - a):
                if end <= t < end + 0x2000:
                    e0 = end
                    while j + 1 < len(L) and L[j][0] + L[j][1] <= t and L[j + 1][0] == end:
                        j += 1; end += L[j][1]; parts.append(L[j][2])
                    ch = end != e0
                    if ch: break
        if len(parts) > 1: frag += len(parts) - 1
        out.append((p, "%08x" % a, end - a, n, "+".join(parts)))
        i = j + 1
with open(R + "/notes/merged_funcs.csv", "w") as f:
    f.write("prog,address,size,name,parts\n")
    for r in out: f.write("%s,%s,%d,%s,%s\n" % r)
print("funciones:", sum(len(v) for v in rows.values()), "-> tras fusionar:", len(out), "(fusiones:", frag, ")")

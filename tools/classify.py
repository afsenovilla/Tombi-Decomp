#!/usr/bin/env python3
"""Classifies the pending functions as complete ("completa") / fragment ("fragmento", with a reason) so no effort
is wasted on pieces. The CSV column names and values are kept in Spanish for compatibility.
Output: notes/todo_match3.csv (prog,address,size,name,estado=status,motivo=reason)."""
import csv, os, re, struct
R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
def img(p):
    if p == "MAIN0":
        d = open(R + "/game/MAIN0.EXE", "rb").read(); return d, 0x800 - struct.unpack("<I", d[0x18:0x1c])[0]
    return open(R + "/game/AREA00/X000.BIN", "rb").read(), -0x800E8028
have = set()
for dp, _, fs in os.walk(R + "/src"):
    if "wip" in dp: continue
    for f in fs:
        m = re.search(r"//\s*FUNC\s+([0-9a-f]+)", open(os.path.join(dp, f), errors="replace").read())
        if m: have.add(m.group(1))
imgs = {p: img(p) for p in ("MAIN0", "X000")}
rows = []
for r in csv.DictReader(open(R + "/notes/merged_funcs.csv")):
    if r["address"] in have: continue
    rows.append(r)
old = {r["address"] for r in csv.DictReader(open(R + "/notes/todo_match.csv"))}
out = []
for r in rows:
    if r["address"] not in old: continue
    d, bias = imgs[r["prog"]]; a = int(r["address"], 16); s = int(r["size"])
    raw = d[a + bias:a + bias + s]; raw += b"\0" * (-len(raw) % 4)
    W = [struct.unpack("<I", raw[i:i + 4])[0] for i in range(0, len(raw), 4)] or [0]
    sw_ra = any((w >> 16) == 0xAFBF for w in W); lw_ra = any((w >> 16) == 0x8FBF for w in W)
    sp_dn = sum(1 for w in W if (w >> 16) == 0x27BD and (w & 0xffff) >= 0x8000)
    sp_up = sum(1 for w in W if (w >> 16) == 0x27BD and (w & 0xffff) < 0x8000)
    # Reasons are written to the CSV in Spanish (kept for compatibility): too small (<8 B); epilogue without
    # prologue (tail of another function); frees the stack without reserving it (tail); starts with jr (fragment);
    # no exit (jr/j): truncated body.
    why = ""
    if s < 8: why = "demasiado pequeña (<8 B)"
    elif lw_ra and not sw_ra: why = "epílogo sin prólogo (cola de otra función)"
    elif sp_up and not sp_dn: why = "libera pila sin reservarla (cola)"
    elif W[0] == 0x03E00008 or (W[0] >> 26) == 2 and s <= 8 and False: why = "empieza por jr (fragmento)"
    elif not any(w == 0x03E00008 or (w >> 26) == 2 or (w >> 26) == 4 and ((w >> 16) & 0x3ff) == 0 or (w >> 26) == 0 and (w & 0x3f) == 8 for w in W):
        why = "sin salida (jr/j): cuerpo cortado"
    out.append((r["prog"], r["address"], s, r["name"], "fragmento" if why else "completa", why))
out.sort(key=lambda x: x[2])
with open(R + "/notes/todo_match3.csv", "w") as f:
    f.write("prog,address,size,name,estado,motivo\n")
    for o in out: f.write("%s,%s,%d,%s,%s,%s\n" % o)
comp = [o for o in out if o[4] == "completa"]
print("pending", len(out), "complete", len(comp), "fragments", len(out) - len(comp))
import collections
print(collections.Counter(o[5] for o in out if o[5]))
for n, (lo, hi) in (("c1", (0, 100)), ("c2", (100, 220)), ("c3", (220, 400)), ("c4", (400, 800))):
    sel = [o for o in comp if lo < o[2] <= hi]
    with open("/opt/psyq/shards/%s.csv" % n, "w") as f:
        f.write("prog,address,size,name\n")
        for o in sel: f.write("%s,%s,%d,%s\n" % o[:4])
    print(n, len(sel))

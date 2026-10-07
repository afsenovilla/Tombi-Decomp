#!/usr/bin/env python3
"""Calcula el progreso de la descompilacion y escribe docs/PROGRESS.md y docs/progress.json.

Lee notes/functions_*.csv (tools/export_functions.py), notes/names_*.csv y src/ (C propio).
Uso: python tools/progress.py
"""
import csv
import json
import os
import re
import datetime

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
N = lambda *p: os.path.join(ROOT, *p)

# programa -> (csv de funciones, csv de nombres, rango de codigo [lo, hi])
PROGRAMS = {
    "MAIN0.EXE (nucleo del juego)": ("functions_main0.csv", "names_main0.csv", (0x800163F4, 0x800778EF)),
    "X000.BIN (overlay AREA00)": ("functions_x000.csv", "names_x000.csv", (0x800E8028, 0x8013C0F8)),
}


def read_csv(path):
    if not os.path.exists(path):
        return []
    with open(path, newline="") as f:
        return list(csv.DictReader(f))


def count_matching_c():
    """Bytes de funciones con C 'matching' declaradas en src/ con el marcador // MATCHING <direccion> <bytes>."""
    total = 0
    src = N("src")
    if not os.path.isdir(src):
        return 0
    for dp, _, files in os.walk(src):
        for fn in files:
            if fn.endswith(".c"):
                for m in re.finditer(r"//\s*MATCHING\s+\w+\s+(\d+)", open(os.path.join(dp, fn), errors="replace").read()):
                    total += int(m.group(1))
    return total


def analyse(csv_f, names_f, rng):
    funcs = read_csv(N("notes", csv_f))
    ours = {}
    for r in read_csv(N("notes", names_f)):
        ours[int(r["address"], 16)] = r
    d = dict(lib=0, named=0, unnamed=0, typed=0, n_lib=0, n_named=0, n_unnamed=0, n_typed=0,
             excluded=0, n_excluded=0, unnamed_list=[], items=[], unnamed_typed=0, range=list(rng))
    for r in funcs:
        a, size = int(r["address"], 16), int(r["size"])
        if not (rng[0] <= a <= rng[1]):
            d["excluded"] += size
            d["n_excluded"] += 1
            continue
        mine = ours.get(a)
        is_fun = r["name"].startswith(("FUN_", "thunk_FUN_", "func_"))
        if mine is not None and "Psy-Q" in mine.get("comment", ""):
            d["lib"] += size; d["n_lib"] += 1
            d["items"].append((a, size, "lib"))
        elif mine is not None:
            d["named"] += size; d["n_named"] += 1
            if r["typed"] == "1": d["typed"] += size; d["n_typed"] += 1
            d["items"].append((a, size, "named"))
        elif not is_fun:
            d["lib"] += size; d["n_lib"] += 1          # nombre dado por Ghidra (firma de libreria)
            d["items"].append((a, size, "lib"))
        else:
            d["unnamed"] += size; d["n_unnamed"] += 1
            d["unnamed_list"].append((size, r["address"]))
            if r["typed"] == "1":
                d["typed"] += size; d["n_typed"] += 1; d["unnamed_typed"] += size
                d["items"].append((a, size, "typed"))
            else:
                d["items"].append((a, size, "unnamed"))
    d["game"] = d["named"] + d["unnamed"]
    d["n_game"] = d["n_named"] + d["n_unnamed"]
    d["unnamed_list"].sort(reverse=True)
    return d


def pct(a, b):
    return 100.0 * a / b if b else 0.0


def bar(p, w=24):
    k = int(round(p / 100 * w))
    return "[" + "#" * k + "." * (w - k) + "]"


COL = {"match": "#16a34a", "named": "#22c55e", "typed": "#38bdf8", "unnamed": "#f59e0b", "lib": "#94a3b8"}
LAB = {"match": "Matching en C", "named": "Nombrada por nosotros", "typed": "Sin nombre, con TObj",
       "unnamed": "Sin nombre, sin tipos", "lib": "Libreria Sony (Psy-Q)"}
FONT = "font-family=\"Segoe UI,Helvetica,Arial,sans-serif\""


def esc(t):
    return t.replace("&", "&amp;").replace("<", "&lt;")


def write_svgs(res, tot, matching):
    hist_path = N("docs", "history.json")
    hist = json.load(open(hist_path)) if os.path.exists(hist_path) else []
    today = datetime.date.today().isoformat()
    point = {"date": today, "named": round(pct(tot["named"], tot["game"]), 2),
             "typed": round(pct(tot["typed"], tot["game"]), 2), "matching": round(pct(matching, tot["game"]), 2)}
    hist = [h for h in hist if h["date"] != today] + [point]
    json.dump(hist, open(hist_path, "w"), indent=1)

    W = 900
    o = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="440" viewBox="0 0 %d 440" role="img" aria-label="Progreso de la descompilacion">' % (W, W),
         '<rect width="%d" height="440" rx="14" fill="#ffffff" stroke="#e2e8f0"/>' % W,
         '<text x="28" y="44" %s font-size="22" font-weight="700" fill="#0f172a">Tombi! (PAL espa&#241;ol) &#8212; progreso de la descompilaci&#243;n</text>' % FONT,
         '<text x="28" y="66" %s font-size="12.5" fill="#64748b">Actualizado %s &#183; unidad: bytes de c&#243;digo de juego (sin librer&#237;a Psy-Q)</text>' % (FONT, today)]
    # niveles
    levels = [("Matching (C que recompila igual)", matching, "match"), ("Funciones con nombre propio", tot["named"], "named"),
              ("Con estructura TObj aplicada (cobertura)", tot["typed"], "typed")]
    y = 108
    for lab, val, key in levels:
        p = pct(val, tot["game"])
        o.append('<text x="28" y="%d" %s font-size="14" fill="#1e293b">%s</text>' % (y + 4, FONT, esc(lab)))
        o.append('<rect x="330" y="%d" width="450" height="18" rx="9" fill="#e2e8f0"/>' % (y - 10))
        wbar = max(0 if p == 0 else 4, 450 * p / 100.0)
        if wbar: o.append('<rect x="330" y="%d" width="%.1f" height="18" rx="9" fill="%s"/>' % (y - 10, wbar, COL[key]))
        o.append('<text x="796" y="%d" %s font-size="15" font-weight="700" fill="#0f172a">%.1f %%</text>' % (y + 5, FONT, p))
        y += 36
    # composicion
    o.append('<text x="28" y="%d" %s font-size="15" font-weight="700" fill="#0f172a">Composici&#243;n del c&#243;digo analizado</text>' % (y + 26, FONT))
    y += 52
    rows = [(k, r) for k, r in res.items()] + [("TOTAL", None)]
    for k, r in rows:
        if r is None:
            seg = {"lib": tot["lib"], "named": tot["named"], "typed": tot["unnamed_typed"] if "unnamed_typed" in tot else 0,
                   "unnamed": tot["unnamed"] - tot.get("unnamed_typed", 0), "match": matching}
        else:
            seg = {"lib": r["lib"], "named": r["named"], "typed": r["unnamed_typed"],
                   "unnamed": r["unnamed"] - r["unnamed_typed"], "match": 0}
        totb = sum(seg.values()) or 1
        o.append('<text x="28" y="%d" %s font-size="13" fill="#1e293b">%s</text>' % (y + 14, FONT, esc(k)))
        x = 330.0
        for key in ("match", "named", "typed", "unnamed", "lib"):
            wseg = 520.0 * seg[key] / totb
            if wseg > 0:
                o.append('<rect x="%.1f" y="%d" width="%.1f" height="22" fill="%s"><title>%s: %d bytes</title></rect>' % (x, y, wseg, COL[key], LAB[key], seg[key]))
            x += wseg
        o.append('<text x="858" y="%d" %s font-size="12" fill="#64748b" text-anchor="start">%d KB</text>' % (y + 15, FONT, totb // 1024))
        y += 34
    # leyenda
    x = 28
    y += 8
    for key in ("match", "named", "typed", "unnamed", "lib"):
        o.append('<rect x="%d" y="%d" width="13" height="13" rx="3" fill="%s"/>' % (x, y, COL[key]))
        o.append('<text x="%d" y="%d" %s font-size="12" fill="#334155">%s</text>' % (x + 19, y + 11, FONT, esc(LAB[key])))
        x += 19 + 7 * len(LAB[key]) + 16
    o.append('<text x="28" y="%d" %s font-size="11.5" fill="#94a3b8">No incluye SCES_013.31, MAIN1-8 (comparten ~98 %% del c&#243;digo con MAIN0) ni los overlays X*.BIN de las dem&#225;s &#225;reas.</text>' % (y + 38, FONT))
    o.append("</svg>")
    open(N("docs", "progress.svg"), "w").write("\n".join(o))

    # mapas de memoria por programa
    for key, (name, r) in zip(("main0", "x000"), res.items()):
        lo, hi = r["range"]
        ROW = 0x4000                      # 16 KB por fila
        rowsn = (hi - lo) // ROW + 1
        wpx = 880.0
        h = 62 + rowsn * 16 + 28
        m = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" viewBox="0 0 %d %d" role="img" aria-label="Mapa de %s">' % (W, h, W, h, esc(name)),
             '<rect width="%d" height="%d" rx="14" fill="#ffffff" stroke="#e2e8f0"/>' % (W, h),
             '<text x="28" y="36" %s font-size="18" font-weight="700" fill="#0f172a">Mapa de funciones &#8212; %s</text>' % (FONT, esc(name)),
             '<text x="28" y="54" %s font-size="12" fill="#64748b">Cada fila = 16 KB desde 0x%08X. Cada bloque = una funci&#243;n, color seg&#250;n su estado. Gris claro = datos o hueco.</text>' % (FONT, lo)]
        for i in range(rowsn):
            yy = 66 + i * 16
            m.append('<rect x="10" y="%d" width="%.1f" height="13" fill="#f1f5f9"/>' % (yy, wpx))
        for a, size, st in sorted(r["items"]):
            row, col = divmod(a - lo, ROW)
            while size > 0 and row < rowsn:
                part = min(size, ROW - col)
                x0 = 10 + wpx * col / ROW
                ww = max(1.0, wpx * part / ROW)
                m.append('<rect x="%.1f" y="%d" width="%.1f" height="13" fill="%s"><title>0x%08X %d B &#183; %s</title></rect>' % (x0, 66 + row * 16, ww, COL[st], a, size, LAB[st]))
                size -= part; row += 1; col = 0
        m.append("</svg>")
        open(N("docs", "map_%s.svg" % key), "w").write("\n".join(m))


def main():
    res = {k: analyse(*v) for k, v in PROGRAMS.items()}
    tot = {key: sum(r[key] for r in res.values()) for key in
           ("lib", "named", "unnamed", "typed", "game", "n_game", "n_named", "n_unnamed", "n_typed", "n_lib", "excluded", "unnamed_typed")}
    matching = count_matching_c()
    levels = [
        ("Funciones de juego con nombre propio", tot["named"], "bytes"),
        ("Funciones de juego con tipos (TObj)", tot["typed"], "bytes"),
        ("C 'matching' en src/", matching, "bytes"),
    ]
    out = []
    w = out.append
    w("# Progreso de la descompilacion de Tombi! (PAL espanol, SCES_013.31)\n")
    w("_Generado con `python tools/progress.py` (%s). No editar a mano._\n" % datetime.date.today().isoformat())
    w("## Resumen\n")
    w("| Nivel | Progreso | Bytes | Funciones |")
    w("|---|---|---|---|")
    w("| **C que coincide byte a byte (matching)** | **%.1f %%** `%s` | %d / %d | 0 |" % (pct(matching, tot["game"]), bar(pct(matching, tot["game"])), matching, tot["game"]))
    w("| Nombradas por nosotros | %.1f %% `%s` | %d / %d | %d / %d |" % (pct(tot["named"], tot["game"]), bar(pct(tot["named"], tot["game"])), tot["named"], tot["game"], tot["n_named"], tot["n_game"]))
    w("| Con la estructura TObj aplicada (cobertura, no es avance de C) | %.1f %% `%s` | %d / %d | %d / %d |" % (pct(tot["typed"], tot["game"]), bar(pct(tot["typed"], tot["game"])), tot["typed"], tot["game"], tot["n_typed"], tot["n_game"]))
    w("")
    w("\"Codigo de juego\" = funciones dentro del codigo de los programas analizados, **sin** contar las de la")
    w("libreria de Sony (Psy-Q), que Ghidra ya identifica. Esas son %d bytes (%d funciones) aparte.\n" % (tot["lib"], tot["n_lib"]))
    w("## Desglose por programa\n")
    w("| Programa | Codigo de juego | Nombrado | Tipado (TObj) | Matching | Libreria Psy-Q |")
    w("|---|---|---|---|---|---|")
    for k, r in res.items():
        w("| %s | %d B (%d f) | %.1f %% | %.1f %% | 0.0 %% | %d B |" % (
            k, r["game"], r["n_game"], pct(r["named"], r["game"]), pct(r["typed"], r["game"]), r["lib"]))
    w("")
    w("## Que NO esta contado (el denominador real es mayor)\n")
    w("- `MAIN1..8.EXE`: comparten ~98 % del codigo (`.text`) con MAIN0; se tratan como variantes, no se suman.")
    w("- `SCES_013.31` (cargador, 651 KB): casi todo es libreria Psy-Q; sin analizar.")
    w("- Resto de overlays `X*.BIN` de las 20 areas (solo se ha analizado `AREA00/X000.BIN`).")
    w("- Funciones que solo llama un overlay y que Ghidra no reconoce, y overlays aun sin identificar")
    w("  (172 destinos de MAIN0 en `0x800E8000+` no caen en X000).")
    w("- Funciones fuera del codigo (`%d` bytes de falsos positivos de Ghidra en RAM sin contenido)." % tot["excluded"])
    w("")
    w("## Funciones sin nombrar mas grandes (siguientes objetivos)\n")
    allun = []
    for k, r in res.items():
        for size, addr in r["unnamed_list"]:
            allun.append((size, addr, k.split()[0]))
    allun.sort(reverse=True)
    w("| Tamano | Direccion | Programa |")
    w("|---|---|---|")
    for size, addr, k in allun[:15]:
        w("| %d B | `%s` | %s |" % (size, addr, k))
    w("")
    w("## Como se mide\n")
    w("- Unidad: bytes de codigo por funcion (el tamano que da Ghidra).")
    w("- *Nombrada*: esta en `notes/names_*.csv` y no es de libreria. *Tipada*: su primer parametro es `TObj *`.")
    w("- *Matching*: funciones en `src/*.c` marcadas con `// MATCHING <direccion> <bytes>`; hoy `src/` no existe,")
    w("  asi que es 0 %. Todavia no hay toolchain para recompilar y comparar con el original.")
    w("- Las cifras de tipado salen del ultimo export de Ghidra (`notes/functions_*.csv`) y pueden ir por detras.")
    open(N("docs", "PROGRESS.md"), "w").write("\n".join(out) + "\n")
    json.dump({"date": datetime.date.today().isoformat(), "totals": {k: v for k, v in tot.items()},
               "matching_bytes": matching,
               "programs": {k: {x: y for x, y in r.items() if x not in ("unnamed_list", "items")} for k, r in res.items()}},
              open(N("docs", "progress.json"), "w"), indent=2)
    write_svgs(res, tot, matching)
    print("\n".join(out[:16]))


if __name__ == "__main__":
    main()

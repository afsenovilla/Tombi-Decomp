#!/usr/bin/env python3
"""Builds the data folder for the port's --civ-test (the cut top-down town AREA11 s0, docs/CIVILIZATION.md "Port test").

  python3 tools/civ_test_data.py OUT_DIR [--data data] [--cap BYTES]
Also writes OUT_DIR/civ_MFNT110S.WFM with ALL the lines (no cap) for the port's --civ-wfm side-file load.

OUT_DIR becomes a copy of your extracted data folder (tools/extract_data.py): every file is a symlink to the original,
except AREA11/MFNT110S.WFM and the language's SYS/LD?AR11.BIN, which are rebuilt with the Spanish lines below written
into the PAL dialogue slots that hold "···??" (MFNT110S #249.., bank k4 of AREA11). The lines are short Spanish
glosses of the Japanese text (AREA11 A002 block 0); they are written in priority order (lines a placed resident can
say first) for as long as the rebuilt WFM still fits its sectors on the disc (the port maps sectors to files through
data/manifest.txt, so a file must not grow past its sector count, and a build close to that limit already showed
garbage glyphs in the box, so the default cap is 26540 bytes, a size checked in the port); the lines that do not fit are
listed.
Nothing from the disc is stored here: only the text. Needs Pillow-free tools/text_core.py.
"""
import argparse, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import text_core as TC  # noqa: E402

WFM = "AREA11/MFNT110S.WFM"
BASE_K4 = 249          # base[k=4] of MFNT110S (u16 table at WFM+0x10)
# (dx in bank 4, Spanish line). Order = priority: dx reached by topdownA11ActorOpenDialogue first.
LINES = [
    (5, "¿Un engranaje? De madera{NL}se rompería."),
    (3, "Bajo el suelo hay{NL}un laberinto."),
    (1, "La madera es lo mejor.{NL}La piedra es fría."),
    (2, "No funciona, pero es{NL}de hierro..."),
    (0, "Aquí vive una adivina,{NL}pero pone una condición."),
    (4, "A esa choza se llega{NL}por debajo del suelo."),
    (6, "Al taller se entra{NL}por arriba."),
    (7, "Este cartel es viejo{NL}y peligroso."),
    (10, "Tengo sed... iré a{NL}por un coco."),
    (11, "Siempre estás lleno{NL}de energía."),
    (16, "El pueblo Karakuri{NL}se hundió."),
    (8, "Uf, por poco..."),
    (9, "Esa roca grande{NL}molesta mucho."),
    (12, "¿Y el dueño de este{NL}vehículo tan raro?"),
    (13, "Sin hechizo, esta{NL}tierra renacerá."),
    (14, "El reloj funciona, pero{NL}el pueblo aún no..."),
    (15, "¿Estará bien el{NL}de las balsas?"),
    (17, "Me casaré con una{NL}chica de piedra."),
]


def sector_limit(data, path):
    for line in open(os.path.join(data, "manifest.txt"), encoding="utf-8"):
        p = line.split()
        if p and p[0] == "file" and p[-1] == path:
            return int(p[2]) * int(p[3])
    raise SystemExit("%s not in %s/manifest.txt" % (path, data))


def build(data, out, lang=5, cap=None):
    limit = sector_limit(data, WFM)
    if cap: limit = min(limit, cap)
    used, files = [], None
    for dx, text in LINES:
        p = TC.Project(TC.Source(data))
        for d, t in used + [(dx, text)]:
            p.edits.setdefault(WFM, {})[BASE_K4 + d] = "{BOX 256 42}{VOICE 6 0}{MOVE 0 0}" + t + "{WAIT}{CLEAR}"
        f, errors = p.encode_edited()
        if errors:
            raise SystemExit("\n".join(errors[:10]))
        if len(f[WFM]) <= limit:
            used.append((dx, text)); files = f
        else:
            print("does not fit (%d > %d bytes): k4 dx%d #%d" % (len(f[WFM]), limit, dx, BASE_K4 + dx))
    if files is None:
        raise SystemExit("no line fits")
    for root, dirs, fs in os.walk(data):
        rel = os.path.relpath(root, data)
        os.makedirs(os.path.join(out, rel), exist_ok=True)
        for f in fs:
            dst = os.path.join(out, rel, f)
            if os.path.lexists(dst):
                os.remove(dst)
            os.symlink(os.path.abspath(os.path.join(root, f)), dst)
    for name, blob in files.items():
        dst = os.path.join(out, *name.split("/"))
        if os.path.lexists(dst):
            os.remove(dst)
        open(dst, "wb").write(blob)
    print("%s: %d lines written (%s), %s = %d bytes of %d" % (out, len(used), " ".join("#%d" % (BASE_K4 + d) for d, _ in used),
                                                            WFM, len(files[WFM]), limit))


def build_wfm(data, out_wfm):
    """All lines, no cap: the whole MFNT110S bank as a side file for the port's --civ-wfm (read straight into memory,
    not through the disc sectors)."""
    p = TC.Project(TC.Source(data))
    for d, t in LINES:
        p.edits.setdefault(WFM, {})[BASE_K4 + d] = "{BOX 256 42}{VOICE 6 0}{MOVE 0 0}" + t + "{WAIT}{CLEAR}"
    f, errors = p.encode_edited()
    if errors:
        raise SystemExit("\n".join(errors[:10]))
    open(out_wfm, "wb").write(f[WFM])
    print("%s: all %d lines, %d bytes (port: --civ-wfm %s)" % (out_wfm, len(LINES), len(f[WFM]), out_wfm))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("--data", default=os.path.join(os.path.dirname(HERE), "data"))
    ap.add_argument("--cap", type=int, default=26540, help="byte limit for the rebuilt WFM (default 26540: checked in the port; a 26600-byte build with 7 lines showed garbage glyphs, so the 13-sector limit of 26624 is not the real one)")
    a = ap.parse_args()
    build(a.data, a.out, cap=a.cap)
    build_wfm(a.data, os.path.join(a.out, "civ_MFNT110S.WFM"))


if __name__ == "__main__":
    main()

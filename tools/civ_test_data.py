#!/usr/bin/env python3
"""Builds the data folder for the port's --civ-test (the cut top-down town AREA11 s0, docs/CIVILIZATION.md "Port test").

  python3 tools/civ_test_data.py OUT_DIR [--data data] [--cap BYTES]
Also writes OUT_DIR/civ_MFNT110S.WFM with ALL the lines (no cap), and civ_MFNT160S.WFM / civ_MFNT170S.WFM (AREA16 and
AREA17 s0, AREA_LINES), for the port's --civ-wfm side-file load (--civ-wfm OUT_DIR picks the town's file).

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


# AREA16 / AREA17 s0: Spanish glosses of the Japanese lines (AREA16/A003.GAM and AREA17/A003.GAM WFM2 block 0,
# tools/jp_text.py) for the PAL slots that hold "···??", by dialogue index of the PAL bank. "/" = new line,
# "|" = next box (the JP {C:▼} page break). Only the residents' lines of AREA17 (#213-#218, #255, #257-#259,
# #262-#268, #270-#289); its scripted generator scene (#219-#239, #253, #254) is left as it is.
AREA_LINES = {
    16: {
        230: "¡Aúpa... YA!",
        231: "¡Hop... AAAH!",
        232: "Esto pegado a la pared/parece un ascensor.../pero no se mueve.",
        233: "¿Que no puedes entrar en/los otros pueblos? Ni idea./No quiero saber nada de ellos.",
        234: "La gente de los otros pueblos...|Parece que se burlan/de nosotros./¡Me pone de los nervios!",
        235: "¿Primera vez en el pueblo?|Lo único de lo que/presumimos es la Torre/reloj, pero...",
        236: "¿Entro o no entro...?|Pero me dijeron que no/me metiera en el pozo.../¿Qué hago?",
        237: "¿Has visto el engranaje?|Mmm.../¿qué podemos hacer?",
        238: "¿Un engranaje que no se/rompa? De hierro, claro...|Pero a mí no me gustan/ni la madera ni el hierro.",
        239: "Je, je... He abierto a/escondidas la puerta...|entre el pueblo de hierro/y el de madera. ¡Chitón!",
        240: "¿Un generador? ¡Eso es!/El ascensor de esta casa/funcionaba con eso.",
        241: "Tan pequeño y tan/valiente...|Quizá yo también deje/de ser tan cabezota.",
        242: "Hace tiempo intenté/meterme en el río...|y de repente ¡me dio/un calambrazo! ¿Qué sería?",
        243: "Je, je... Me han pillado.|Pero es un rollo que/todos estén peleados.",
        244: "El generador funciona,/pero dentro del muro/que rodea el país|las máquinas no van.../Será el Cerdiablo...",
        245: "Quiero hacer las paces/con todos...|pero todavía estamos/un poco tensos.",
        246: "¿Vas al pueblo Karakuri/desde el estanque viejo?/¡Ánimo!",
        247: "Hermano, tú vencerás/al Cerdiablo que hechizó/este país, ¿verdad?",
    },
    17: {
        213: "¿Eh? ¿Qué quieres?|Aunque no lo parezca,/soy un herrero de primera.|Si quieres algo de hierro,/ven cuando quieras.",
        214: "¿Un engranaje de hierro?|¡Ja, ja! Eso lo hago/en un periquete...|...si tuviera hierro./Tráeme el material/y te lo hago enseguida.",
        215: "¡Buen material!/Espera un momento...",
        216: "¡Toma! Así trabajo yo./Llévatelo.",
        217: "Pero ¿para qué quieres/un engranaje de hierro?",
        218: "Dicen que arreglaste/la torre de piedra.|Arreglar cosas está/bien... Gracias.",
        255: "¿Gantetsu? ¿Ese doctor/tan cascarrabias?|Se fue a su casa: la del/medio, se llega por/el ascensor.",
        257: "Hmm, cara nueva... Ahí/está el Castillo de hierro./Si no vas a nada, vete.|¿Yo? Me llamo Gantetsu.|Si eres útil a este país,/te echaré una mano.",
        258: "Quién iba a decir que un/chico arreglaría la torre.|Siento lo que te dije./Y otra cosa...",
        259: "La torre que arreglaste...|para que funcione de/verdad hay que arrancar/el generador del castillo.|Pero hay un problema./Ve al Castillo de hierro/y míralo tú mismo.",
        262: "Para hacer una bomba,/esta máquina necesita/un poco de electricidad.|Basta con una Anguila/eléctrica de la jungla.|Y el Agua negra, la base/de la bomba. Tráeme las/dos cosas. ¡Cuento contigo!",
        263: "¡Espera! ¿Vas a coger la/anguila con las manos?|¡Qué impaciente!/Toma, llévate esto.",
        264: "¡Vaya! Ya tienes las dos./Dame el material.",
        265: "¡Bien! Ya puedo hacer/la mezcla de la bomba.|Mientras la preparo,/¿me traes un coco?/Hay en la jungla.|Se llena la cáscara con/la mezcla ¡y listo!",
        266: "¡Por fin!/Dame el coco.",
        267: "Se mete la mezcla en/la cáscara... ¡Lista!",
        268: "¿Quieres más bombas?|Tráeme cocos y te hago/todas las que quieras.",
        270: "¿Quieres subir?/Entonces entra en esta casa.",
        271: "Bienvenido. Esto es el/pueblo de los Hombres/de hierro.",
        272: "Duro y duradero.../¡El hierro es lo mejor!",
        273: "Yo me pregunto... ¿por/qué se pelea todo el mundo?/No me gusta nada.",
        274: "Todos con la madera y la/piedra, ¡qué blandos!|¡Un hombre es de hierro!/¿Qué otra cosa?",
        275: "Si bajas recto desde mí,/llegas a la salida. Desde/aquí cuesta verlo, ¿eh?",
        276: "Antes el ascensor iba/con la máquina, sin/manivela...",
        277: "Sube en el ascensor y/sigue el camino del medio:/allí está el castillo.",
        278: "¡Ese viejo de Gantetsu!/¿Por qué no me deja ir/al Castillo de hierro?",
        279: "A este país también lo/hechizó el Cerdiablo.../y no podemos hacer nada.",
        280: "¿Buscas algo? Si quieres/algo de hierro, ve a la/casa del ascensor.",
        281: "¿Arreglaste la torre?/¡Qué pasada!|Tal como está el país,/es increíble.",
        282: "¿La torre de piedra se/arregló con un engranaje/de hierro?|¿Eh? ¿Que no se mueve?/Vaya...",
        283: "¿La puerta del Castillo/de hierro?|¡Ja, ja, ja! Esa no/la rompe nadie.",
        284: "¿Sabes nadar bien, chico?|Yo me hundo como/una piedra.",
        285: "Yo también quiero decir/con orgullo...|¡Esto es la Ciudad/de civilización!",
        286: "¿T-también arreglaste/el generador?/¡E-eres increíble!",
        287: "Madera, piedra y hierro.../lo importante es el/equilibrio.",
        288: "¿Vas a ver al Viejo de/los 10.000 años?/¿Tienes una balsa?",
        289: "Vaya, eres increíble.../Siento haberme burlado./Ven a jugar otra vez.",
    },
}


def dialogue(text):
    """One WFM dialogue: a box sized for its tallest page (13 px per line, 8 px per character), pages joined by
    {WAIT}{CLEAR}."""
    pages = [p.split("/") for p in text.split("|")]
    n = max(len(p) for p in pages)
    w = min(300, max(120, 8 * max(len(l) for p in pages for l in p) + 24))
    body = "{WAIT}{CLEAR}".join("{NL}".join(p) for p in pages)
    return "{BOX %d %d}{VOICE 6 0}{MOVE 0 0}%s{WAIT}{CLEAR}" % (w, 16 + 13 * n, body)


def build_area_wfm(data, area, out_wfm):
    """AREA16 / AREA17: every line of AREA_LINES into that area's bank, as a side file for --civ-wfm (no disc file is
    rebuilt for them)."""
    wfm = "AREA%d/MFNT%d0S.WFM" % (area, area)
    p = TC.Project(TC.Source(data))
    for n, t in AREA_LINES[area].items():
        p.edits.setdefault(wfm, {})[n] = dialogue(t)
    f, errors = p.encode_edited()
    if errors:
        raise SystemExit("\n".join(errors[:10]))
    open(out_wfm, "wb").write(f[wfm])
    print("%s: %d lines, %d bytes (port: --civ-wfm %s)" % (out_wfm, len(AREA_LINES[area]), len(f[wfm]),
                                                          os.path.dirname(out_wfm)))


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
    for area in (16, 17):
        build_area_wfm(a.data, area, os.path.join(a.out, "civ_MFNT%d0S.WFM" % area))


if __name__ == "__main__":
    main()

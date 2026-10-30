#!/usr/bin/env python3
"""Builds the data folder for the port's --civ-test (the cut top-down town AREA11 s0, docs/CIVILIZATION.md "Port test").

  python3 tools/civ_test_data.py OUT_DIR [--data data] [--cap BYTES]
Also writes OUT_DIR/civ_MFNT110<L>.WFM with ALL the lines (no cap), and civ_MFNT160<L>.WFM / civ_MFNT170<L>.WFM
(AREA16 and AREA17 s0, AREA_LINES), one set per PAL language letter L (BFGIHSPWD = --lang 0..8): Spanish in S, English
in B and, as a fallback, in the other languages; for the port's --civ-wfm side-file load (--civ-wfm OUT_DIR picks the
town's file of the current --lang).

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

# English versions (translated from the same Japanese lines), by dx / dialogue index; the English text is also written
# into the banks of the PAL languages without a translation here (F G I H P W D: English fallback).
LINES_EN = {
    5: "A gear? A wooden one{NL}would break.",
    3: "There's a maze{NL}under the floor.",
    1: "Wood is the best.{NL}Stone is cold.",
    2: "It doesn't work, but{NL}it's made of iron...",
    0: "A fortune-teller lives here,{NL}but she has a condition.",
    4: "You reach that hut{NL}from under the floor.",
    6: "You get into the{NL}workshop from above.",
    7: "This sign is old{NL}and dangerous.",
    10: "I'm thirsty... I'll go{NL}get a coconut.",
    11: "You're always so{NL}full of energy.",
    16: "Karakuri Village{NL}has sunk.",
    8: "Phew, that was close...",
    9: "That big rock is{NL}really in the way.",
    12: "Who owns this{NL}strange vehicle?",
    13: "Without the spell, this{NL}land will be reborn.",
    14: "The clock works, but{NL}the village still doesn't...",
    15: "I wonder if the raft{NL}guy is all right.",
    17: "I'm going to marry{NL}a stone girl.",
}

AREA_LINES_EN = {
    16: {
        230: "Heave... HO!",
        231: "Hup... WAAAH!",
        232: "This thing on the wall/looks like an elevator.../but it won't move.",
        233: "You can't get into the/other villages? No idea./I want nothing to do with them.",
        234: "The people of the other villages...|They seem to look/down on us./It really gets on my nerves!",
        235: "First time in the village?|The only thing we can/brag about is the Clock/tower, but...",
        236: "Should I go in or not...?|But they told me not/to go into the well.../What do I do?",
        237: "Have you seen the gear?|Hmm.../what can we do?",
        238: "A gear that won't break?/Iron, of course...|But I don't like/wood or iron.",
        239: "Heh heh... I secretly/opened the door...|between the iron village/and the wood one. Shh!",
        240: "A generator? That's it!/This house's elevator/used to run on that.",
        241: "So small and so/brave...|Maybe I should stop/being so stubborn too.",
        242: "Once I tried to/go into the river...|and suddenly I got/a big shock! What was it?",
        243: "Heh heh... I got caught.|But it's a drag that/everyone is fighting.",
        244: "The generator works,/but inside the wall/around the country|machines won't run.../Must be the Evil Pig...",
        245: "I want to make peace/with everyone...|but things are still/a little tense.",
        246: "Going to Karakuri Village/from the old pond?/Good luck!",
        247: "Big brother, you'll beat/the Evil Pig who cursed/this land, right?",
    },
    17: {
        213: "Huh? What do you want?|I may not look it, but/I'm a first-class smith.|If you need something iron,/come by anytime.",
        214: "An iron gear?|Ha ha! I can make that/in no time...|...if I had iron./Bring me the material/and I'll make it at once.",
        215: "Good material!/Wait a moment...",
        216: "Here! That's how I work./Take it.",
        217: "But what do you need/an iron gear for?",
        218: "They say you fixed/the stone tower.|Fixing things is/good... Thanks.",
        255: "Gantetsu? That grumpy/old doctor?|He went home: the middle/house, you get there by/the elevator.",
        257: "Hmm, a new face... That's/the Iron Castle over there./If you've no business, go.|Me? I'm Gantetsu.|If you're useful to this/land, I'll lend a hand.",
        258: "Who'd have thought a/kid would fix the tower.|Sorry about what I said./And one more thing...",
        259: "The tower you fixed...|to make it really work/you must start the/castle's generator.|But there's a problem./Go to the Iron Castle/and see for yourself.",
        262: "To make a bomb, this/machine needs a little/electricity.|An Electric eel from the/jungle will do.|And Black water, the base/of the bomb. Bring me/both. I'm counting on you!",
        263: "Wait! You're going to/grab the eel by hand?|So impatient!/Here, take this.",
        264: "Wow! You've got both./Give me the material.",
        265: "Good! Now I can make/the bomb mixture.|While I make it, can/you bring me a coconut?/There are some in the jungle.|Fill the shell with/the mixture and done!",
        266: "At last!/Give me the coconut.",
        267: "Put the mixture in/the shell... Done!",
        268: "Want more bombs?|Bring me coconuts and/I'll make as many as you like.",
        270: "Want to go up?/Then go into this house.",
        271: "Welcome. This is the/village of the Iron/men.",
        272: "Hard and long-lasting.../Iron is the best!",
        273: "I wonder... why is/everyone fighting?/I don't like it at all.",
        274: "Everyone with wood and/stone, how soft!|A man is made of iron!/What else?",
        275: "Go straight down from me/and you reach the exit./It's hard to see from here, huh?",
        276: "The elevator used to run/on the machine, without/a crank...",
        277: "Take the elevator and/follow the middle path:/the castle is there.",
        278: "That old Gantetsu!/Why won't he let me go/to the Iron Castle?",
        279: "This land was cursed/by the Evil Pig too.../and we can't do a thing.",
        280: "Looking for something? For/iron things, go to the/elevator house.",
        281: "You fixed the tower?/That's amazing!|With the land as it is,/it's incredible.",
        282: "The stone tower was fixed/with an iron gear?|Huh? It doesn't move?/Oh well...",
        283: "The Iron Castle's/door?|Ha ha ha! Nobody can/break that one.",
        284: "Can you swim well, kid?|I sink like/a stone.",
        285: "I want to say it/proudly too...|This is the City/of civilization!",
        286: "Y-you fixed the/generator too?/Y-you're amazing!",
        287: "Wood, stone and iron.../what matters is/balance.",
        288: "Going to see the/10,000-Year-Old Man?/Do you have a raft?",
        289: "Wow, you're amazing.../Sorry I made fun of you./Come play again.",
    },
}

LANGS = "BFGIHSPWD"     # language letter of the WFM files by --lang N (tools/title_layout.py LANG_N)


def lang_lines(letter):
    """(LINES, AREA_LINES) for a language: Spanish for S, English for every other one."""
    if letter == "S":
        return LINES, AREA_LINES
    return [(d, LINES_EN[d]) for d, _ in LINES], AREA_LINES_EN


def dialogue(text):
    """One WFM dialogue: a box sized for its tallest page (13 px per line, 8 px per character), pages joined by
    {WAIT}{CLEAR}."""
    pages = [p.split("/") for p in text.split("|")]
    n = max(len(p) for p in pages)
    w = min(300, max(120, 8 * max(len(l) for p in pages for l in p) + 24))
    body = "{WAIT}{CLEAR}".join("{NL}".join(p) for p in pages)
    return "{BOX %d %d}{VOICE 6 0}{MOVE 0 0}%s{WAIT}{CLEAR}" % (w, 16 + 13 * n, body)


def build_area_wfm(data, area, out_wfm, lang="S"):
    """AREA16 / AREA17: every line of AREA_LINES into that area's bank (of language letter lang), as a side file for
    --civ-wfm (no disc file is rebuilt for them)."""
    wfm = "AREA%d/MFNT%d0%s.WFM" % (area, area, lang)
    lines = lang_lines(lang)[1][area]
    p = TC.Project(TC.Source(data))
    p.langs = {lang}
    for n, t in lines.items():
        p.edits.setdefault(wfm, {})[n] = dialogue(t)
    f, errors = p.encode_edited()
    if errors:
        raise SystemExit("\n".join(errors[:10]))
    open(out_wfm, "wb").write(f[wfm])
    print("%s: %d lines, %d bytes (port: --civ-wfm %s)" % (out_wfm, len(lines), len(f[wfm]),
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


def build_wfm(data, out_wfm, lang="S"):
    """All lines, no cap: the whole MFNT110<lang> bank as a side file for the port's --civ-wfm (read straight into
    memory, not through the disc sectors)."""
    wfm = WFM.replace("S.WFM", lang + ".WFM")
    lines = lang_lines(lang)[0]
    p = TC.Project(TC.Source(data))
    p.langs = {lang}
    for d, t in lines:
        p.edits.setdefault(wfm, {})[BASE_K4 + d] = "{BOX 256 42}{VOICE 6 0}{MOVE 0 0}" + t + "{WAIT}{CLEAR}"
    f, errors = p.encode_edited()
    if errors:
        raise SystemExit("\n".join(errors[:10]))
    open(out_wfm, "wb").write(f[wfm])
    print("%s: all %d lines, %d bytes (port: --civ-wfm %s)" % (out_wfm, len(lines), len(f[wfm]), out_wfm))


def build_all_wfm(data, out_dir):
    """Every language's side files: OUT/civ_MFNTnn0<letter>.WFM for AREA11/16/17 and each letter of LANGS (Spanish for
    S, English for the rest)."""
    for L in LANGS:
        build_wfm(data, os.path.join(out_dir, "civ_MFNT110%s.WFM" % L), L)
        for area in (16, 17):
            build_area_wfm(data, area, os.path.join(out_dir, "civ_MFNT%d0%s.WFM" % (area, L)), L)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("--data", default=os.path.join(os.path.dirname(HERE), "data"))
    ap.add_argument("--cap", type=int, default=26540, help="byte limit for the rebuilt WFM (default 26540: checked in the port; a 26600-byte build with 7 lines showed garbage glyphs, so the 13-sector limit of 26624 is not the real one)")
    a = ap.parse_args()
    build(a.data, a.out, cap=a.cap)
    build_all_wfm(a.data, a.out)


if __name__ == "__main__":
    main()

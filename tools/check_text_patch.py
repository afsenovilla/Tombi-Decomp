#!/usr/bin/env python3
"""Validate a text patch (*.tombipatch.json) against the original disc with the text editor's own checks.

For every edited dialogue:
  - the text encodes (known tags, glyphs that exist in the PAL fonts): Project.validate; a character the file's own
    font lacks is added from another file's font of the same height, like the editor does (listed as a note; with
    --strict-glyphs it is an error); a character no font of the disc has (':' at 16 px, '%', ...) is always an error
  - the preview checks of the editor (text_preview): no line wider than the box, no page with more lines than the box
    height allows, no page over 128 glyphs (a problem the original already had is tolerated, a new one is not)
  - event titles: no more letters than the layout list (Project.title_problem), and every copy of the list (the I?NFO blocks
    and the per-area copies in X?NN, title_layout.copies) laid out as the text says (words not split, no leftover tiles); the "completed" banner (CFNT999 #0) at
    most as many letters as the executable's banner table (text_core pads a shorter one to that size)
  - control codes are kept: the sequence of tags other than {NL} and {BOX} (pauses, {WAIT}{CLEAR} pages, colours, voices,
    moves, choices, button/item codes {Cxxx}) must be identical to the original one
  - {BOX w h}: present iff the original has it; h <= original h; w <= 312 when the original box is >= 240 wide or sits at
    {MOVE 0 0} (default position), <= 256 for one-line popups without MOVE, else <= original w (boxes placed next to a
    speaker with {MOVE dx dy}); dialogues 0-2 of MFNT files (item popup verbs, width constants in the executable)
    must not get wider (unless the patch sets verbFirst: the executable's box constants then follow
    the verb widths, Project.verb_deltas)
  - the patch's disc sha1 matches the disc
  - the whole patch, encoded like the tools do it (Project.encode_edited): every glyph word of every dialogue of the
    patched files is below the file's glyph count, and every WFM that changed size has its entries in the area load
    lists (SYS/LDnARxx.BIN) updated (tools/load_lists.py; a stale size let the next file overwrite the WFM's tail in
    memory: "UUUUUU" boxes and a crash in AREA00)
  - the pause menu's Si/No questions of EXE/MAIN5.EXE ("Cargar juego?", "Salir juego?", 8 px a glyph): the port widens
    their frame to the text plus a 12-px margin (port/fixes.txt, drawPauseMenu) and reads "Salir del juego?"; that box
    must still fit on the 320-px screen (pause_boxes)

  check_text_patch.py DISC.bin PATCH.tombipatch.json [-v]
Exit status 1 when anything fails.
"""
import argparse, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import text_core as C
import text_patch as TP
import text_preview as P
import wfm_py as W

MAX_W = 312
ONE_LINE_W = 256
STRICT_GLYPHS = False  # --strict-glyphs: a character the file's own font lacks is an error
NOTES = []
VERB_FIRST = False   # set from the patch: the executable patch (verb_order.py) then follows the verb box widths
TAG = re.compile(r"\{[^}]*\}")


def tags(text):
    return [t for t in TAG.findall(text) if not t.startswith("{NL") and not t.startswith("{BOX")]


def box(text):
    m = re.search(r"\{BOX (\d+) (\d+)\}", text)
    return (int(m.group(1)), int(m.group(2))) if m else None


def problems_of(wc, words):
    """Kinds of preview problems: {'wide', 'lines', 'chars'} (same rules as text_preview.render)."""
    pages, bx = P.paginate(wc, words)
    out = set()
    for p in pages:
        lines = p.lines[:-1] if len(p.lines) > 1 and not p.lines[-1] else p.lines
        if bx:
            bw, bh = bx
            if any(p.width(i) > bw - 2 * P.MARGIN for i in range(len(lines))):
                out.add("wide")
            if len(lines) > (bh - 2 * P.MARGIN) // P.LINE_STEP:
                out.add("lines")
        if p.glyphs > P.GLYPH_LIMIT:
            out.add("chars")
    return out


def check_entry(proj, path, idx, text):
    errs = []
    orig = proj.original(path, idx)
    try:
        words, wc = proj.words(path, idx, text)
    except W.WfmError as e:
        return ["encode: %s" % e]
    if len(wc.glyphs) > len(proj.wfm(path).glyphs):
        miss = sorted({g.char or "?" for g in wc.glyphs[len(proj.wfm(path).glyphs):]})
        if STRICT_GLYPHS:
            errs.append("characters missing from this file's font (would be borrowed from another file): %s" % " ".join(miss))
        else:
            NOTES.append("%s #%d: glyphs added from another file's font of the same size: %s" % (path, idx, " ".join(miss)))
    tp = proj.title_problem(path, idx, text)
    if tp:
        errs.append("title: %s" % tp)
    if proj.title_max(path, idx) is None:          # ordinary dialogue
        ow, owc = proj.words(path, idx, orig)
        new = problems_of(wc, words) - problems_of(owc, ow)
        if new:
            errs.append("preview: " + ", ".join(sorted(new)))
        b, ob = box(text), box(orig)
        if (b is None) != (ob is None):
            errs.append("BOX added/removed")
        elif b:
            if b[1] > ob[1]:
                errs.append("BOX taller than original (%d > %d)" % (b[1], ob[1]))
            mv = re.search(r"\{MOVE (\d+) (\d+)\}", orig)
            if ob[0] >= 240 or (mv and mv.groups() == ("0", "0")):
                lim = MAX_W                      # full-width / default-position boxes
            elif not mv and ob[1] <= 29:
                lim = max(ob[0], ONE_LINE_W)     # one-line system popups (item names, notices) are centred by the code
            else:
                lim = ob[0]                      # boxes placed next to a speaker
            if os.path.basename(path).startswith("MFNT") and idx <= 2 and not VERB_FIRST:
                lim = ob[0]
            if b[0] > lim:
                errs.append("BOX wider than allowed (%d > %d)" % (b[0], lim))
    if tags(text) != tags(orig):
        errs.append("control codes differ: %s vs %s" % (tags(text), tags(orig)))
    return errs


def check_encoded(proj, patch):
    """Problems of the files the patch produces (glyph indices, load list sizes)."""
    import load_lists
    try:
        TP.apply_to_project(proj, patch)
        files, errors = proj.encode_edited()
    except C.CoreError as e:
        return ["encode: %s" % e]
    if errors:
        return errors[:20]
    out = []
    for path, data in sorted(files.items()):
        if not path.upper().endswith(".WFM"):
            continue
        w = W.decode(data)
        for i, d in enumerate(w.dialogues):
            j, ws = 0, d.words
            while j < len(ws):
                x = ws[j]
                if x in W.CTRL:
                    j += 1 + W.CTRL[x][1]
                    continue
                if 0x8000 <= x < 0xF000 and (x & 0xfff) >= len(w.glyphs):
                    out.append("%s #%d: glyph %d of %d" % (path, i, x & 0xfff, len(w.glyphs)))
                    break
                j += 1
    read_new = lambda p: files[p] if p in files else proj.src.read_any(p)
    names = proj.src.all_names()
    out += title_copies(proj, read_new, names)
    wfms = [p for p in files if p.upper().endswith(".WFM")]
    sizes = {p: len(files[p]) for p in wfms}
    pristine = {p: len(proj.src.read_any(p)) for p in wfms}
    out += load_lists.problems(read_new, names, sizes, pristine)
    for p in wfms:
        if sizes[p] != pristine[p] and load_lists.AREA.fullmatch(p) and \
                not load_lists._matches(read_new, names, {p: sizes[p]}):
            out.append("%s changed size (%d -> %d) but no load list entry follows it" % (p, pristine[p], sizes[p]))
    return out


def title_copies(proj, read_new, names):
    """Every copy of an edited event title's layout list (I?NFO blocks and the per-area copies of X?NN, title_layout.copies) must
    hold the letters as the text lays them out: a copy left with the original spacing shows the new letters at the old places
    (AREA01/X501.BIN: "Objetos p / erdidos", "¿Quép / asa?¡")."""
    import title_layout as TL
    out, seen = [], set()
    for path, e in sorted(proj.edits.items()):
        lay = proj.layout(path) if TL.is_title_file(path) else None
        if lay is None:
            continue
        lang = C.lang_of(path)
        ref = {}
        for q in proj.src.names():
            if TL.is_title_file(q) and C.lang_of(q) == lang and proj.layout(q):
                ref.update(proj.layout(q).recs)
        orig = TL.copies(proj.src.read_any, names, proj.wfm(path), lang, ref)
        for idx, text in sorted(e.items()):
            if idx not in lay.recs:
                continue
            want = proj._effective_lines(lay, idx, text)
            for name, where in sorted(orig.items()):
                if idx not in where or (name, idx, text) in seen:
                    continue
                seen.add((name, idx, text))
                got = TL.parse_list(read_new(name), where[idx][0])
                n = TL.text_to_lines(text)[1]
                if got is None or len(got) != len(where[idx][1]):
                    out.append("%s #%d: the copy in %s has %s records, the list %d" % (path, idx, name, len(got or ()), len(where[idx][1])))
                elif len(where[idx][1]) < n:
                    out.append("%s #%d: %d letters, the copy in %s holds %d" % (path, idx, n, name, len(where[idx][1])))
                elif TL.records_to_lines(TL.visible(got)) != want:
                    out.append("%s #%d: the copy in %s is laid out %s, the text %s" % (path, idx, name,
                               TL.records_to_lines(TL.visible(got)), want))
    return out


PAUSE_GLYPH_W, PAUSE_MARGIN, PAUSE_SCREEN_W = 8, 12, 320
PAUSE_SALIR = (bytes([0x1e, 0x0b, 0x0d, 0x14, 0x11, 0x19, 0x00, 0x13, 0x1c, 0x0f, 0x10, 0x17, 0x1f]),
               bytes([0x1e, 0x0b, 0x0d, 0x14, 0x11, 0x19, 0x00, 0x0e, 0x0f, 0x14, 0x00, 0x13, 0x1c, 0x0f, 0x10, 0x17, 0x1f]))


def pause_boxes(proj, exe="EXE/MAIN5.EXE"):
    """The pause menu's Si/No questions (drawPauseMenu entry lists: glyph bytes, 0xFF-ended, '¿' 0x1E .. '?' 0x1F)
    in the Spanish executable, as the port draws them, and the frame width the port gives each: problems."""
    try:
        b = proj.src.read_any(exe)
    except Exception:
        return []
    out = []
    for m in re.finditer(rb"\x1e[\x00-\x1d]{1,40}\x1f\xff", b):
        q = m.group()[:-1]
        if q == PAUSE_SALIR[0]:
            q = PAUSE_SALIR[1]
        w = len(q.rstrip(b"\x00")) * PAUSE_GLYPH_W + 2 * PAUSE_MARGIN
        if w > PAUSE_SCREEN_W - 16:
            out.append("%s @%X: pause question of %d glyphs needs a %d-px box (screen %d)" % (exe, m.start(), len(q), w, PAUSE_SCREEN_W))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("disc"); ap.add_argument("patch"); ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--strict-glyphs", action="store_true", help="do not accept glyphs borrowed from other files")
    a = ap.parse_args()
    patch = TP.load(a.patch)
    global VERB_FIRST, STRICT_GLYPHS
    STRICT_GLYPHS = a.strict_glyphs
    VERB_FIRST = bool(patch.get("verbFirst"))
    bad = 0
    try:
        TP.check_disc(patch, a.disc)
    except C.CoreError as e:
        print("DISC: %s" % e)
        bad += 1
    proj = C.Project(C.Source(a.disc))
    n = 0
    for path, d in sorted(patch["edits"].items()):
        if path not in proj.src.files:
            print("%s: not on the disc" % path)
            bad += 1
            continue
        for k, text in sorted(d.items(), key=lambda kv: int(kv[0])):
            i = int(k)
            n += 1
            if not 0 <= i < proj.count(path):
                print("%s #%s: no such dialogue" % (path, k))
                bad += 1
                continue
            errs = check_entry(proj, path, i, text)
            if errs:
                bad += 1
                print("%s #%d: %s" % (path, i, "; ".join(errs)))
            elif a.verbose:
                print("%s #%d: ok" % (path, i))
    for msg in check_encoded(proj, patch):
        print("ENCODED: %s" % msg)
        bad += 1
    for msg in pause_boxes(proj):
        print("PAUSE: %s" % msg)
        bad += 1
    for note in NOTES:
        print("note:", note)
    print("%d entries checked, %d with problems" % (n, bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())

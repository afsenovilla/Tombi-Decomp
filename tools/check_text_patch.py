#!/usr/bin/env python3
"""Validate a text patch (*.tombipatch.json) against the original disc with the text editor's own checks.

For every edited dialogue:
  - the text encodes (known tags, glyphs that exist in the PAL fonts): Project.validate; a character the file's own
    font lacks is added from another file's font of the same height, like the editor does (listed as a note; with
    --strict-glyphs it is an error); a character no font of the disc has (':' at 16 px, '%', ...) is always an error
  - the preview checks of the editor (text_preview): no line wider than the box, no page with more lines than the box
    height allows, no page over 128 glyphs (a problem the original already had is tolerated, a new one is not)
  - event titles: no more letters than the layout list (Project.title_problem)
  - control codes are kept: the sequence of tags other than {NL} and {BOX} (pauses, {WAIT}{CLEAR} pages, colours, voices,
    moves, choices, button/item codes {Cxxx}) must be identical to the original one
  - {BOX w h}: present iff the original has it; h <= original h; w <= 312 when the original box is >= 240 wide or sits at
    {MOVE 0 0} (default position), <= 256 for one-line popups without MOVE, else <= original w (boxes placed next to a
    speaker with {MOVE dx dy}); dialogues 0-2 of MFNT files (item popup verbs, width constants in the executable)
    must not get wider (unless the patch sets verbFirst: the executable's box constants then follow
    the verb widths, Project.verb_deltas)
  - the patch's disc sha1 matches the disc

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
    for note in NOTES:
        print("note:", note)
    print("%d entries checked, %d with problems" % (n, bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())

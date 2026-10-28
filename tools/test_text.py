#!/usr/bin/env python3
"""Regression tests of the text editor core: edit -> build the ISO -> reopen it -> the text must be there.
Needs game/disc/Tombi.bin and mkpsxiso/dumpsxiso (skipped otherwise).  python3 tools/test_text.py"""
import os, shutil, sys, tempfile, unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import text_core as C

DISC = os.path.join(os.path.dirname(HERE), "game", "disc", "Tombi.bin")
NAME = "CFNT999S.WFM"


def areas(src):
    return [n for n in src.names() if os.path.basename(n).upper() == NAME]


@unittest.skipUnless(os.path.isfile(DISC) and C.have_mkpsxiso(), "needs game/disc/Tombi.bin and mkpsxiso")
class BuildRoundTrip(unittest.TestCase):
    def check(self, img, text, names):
        proj = C.Project(C.Source(img))
        for n in names:
            self.assertEqual(proj.original(n, 0), text, n)

    def test_edit_all_copies_build_reopen(self):
        tmp = tempfile.mkdtemp(prefix="tt_")
        try:
            proj = C.Project(C.Source(DISC))
            names = areas(proj.src)
            self.assertGreaterEqual(len(names), 19)
            self.assertEqual(proj.original(names[0], 0), "¡DESPEJADA!")
            self.assertEqual(len(proj.set_text_linked(names[0], 0, "¡LOGRADO!")), len(names))
            out = os.path.join(tmp, "Tombi-ES.bin")
            rep = C.build_report(proj, out, os.path.join(tmp, "work"))
            self.assertEqual(rep["out"], out)
            self.assertTrue(os.path.isfile(out) and os.path.isfile(os.path.splitext(out)[0] + ".cue"))
            self.assertEqual(sorted(rep["files"]), sorted(names + ["EXE/MAIN5.EXE"]))   # + the banner tables of the executable
            self.check(out, "¡LOGRADO!", names)
            # the engine draws the banner from tables of MAIN5.EXE (not from the text): they must follow the 9 letters (L O G R A D O are glyphs of the file)
            import title_layout as TL
            src = C.Source(out)
            info = TL.banner_info(src.read_any, "S")
            import wfm_py as W
            words = W.decode(src.read(names[0])).dialogues[0].words
            self.assertEqual(info["k"], 11)
            self.assertEqual(len(words), 11)            # dialogue 0 keeps K words (padded): a shorter one hung the native port on NOW LOADING
            self.assertEqual(info["shown"], 9)
            self.assertEqual(info["ids"][:9], [w & 0x7FFF for w in words[:9]])
            self.assertEqual(info["xs"][:9], [-80 + 20 * i for i in range(9)])
            self.assertEqual(info["xs"][9:], [80, 80])
            # second edit built over the same output (and then with the output as the opened image, in place)
            proj2 = C.Project(C.Source(out))
            proj2.set_text_linked(names[3], 0, "¡HECHO!")
            C.build_report(proj2, out, os.path.join(tmp, "work2"), disc=out)
            self.check(out, "¡HECHO!", names)
            self.assertFalse([f for f in os.listdir(tmp) if f.startswith("~build")])
        finally:
            shutil.rmtree(tmp, ignore_errors=True)


@unittest.skipUnless(os.path.isfile(DISC), "needs game/disc/Tombi.bin")
class VerbFirst(unittest.TestCase):
    def test_patch_all_executables(self):
        import verb_order as V
        src = C.Source(DISC)
        proj = C.Project(src)
        proj.verb_first = True
        proj.langs = set(proj.languages())
        files, errors = proj.encode_edited()
        self.assertEqual(errors, [])
        self.assertEqual(len(files), 9)
        for name, data in files.items():
            old = src.read_any(name)
            self.assertEqual(len(data), len(old))
            self.assertTrue(V.is_patched(data) and not V.is_patched(old), name)
            self.assertEqual(V.patch(data), data)                      # idempotent
            self.assertEqual(sum(x != y for x, y in zip(old, data)), 28, name)
            self.assertTrue(all(FUNC_LO <= i < FUNC_HI for i, (x, y) in enumerate(zip(old, data)) if x != y))

    def test_width_follows_verb(self):
        import verb_order as V
        src = C.Source(DISC)
        proj = C.Project(src)
        old = V.widths(src.read_any("EXE/MAIN5.EXE"))
        self.assertEqual(old, {0: 72, 1: 88, 4: 88})
        self.assertEqual(proj.verb_deltas("S"), (0, 0))
        for n in src.names():
            if C.lang_of(n) == "S" and "MFNT" in n:
                proj.edits.setdefault(n, {})[2] = "Obtienes: "
        files, errors = proj.encode_edited()
        self.assertEqual(errors, [])
        self.assertEqual(V.widths(files["EXE/MAIN5.EXE"]), {0: 96, 1: 88, 4: 88})
        self.assertNotIn("EXE/MAIN0.EXE", files)


FUNC_LO, FUNC_HI = 0x3DE20, 0x3DE20 + 0x560


if __name__ == "__main__":
    unittest.main()

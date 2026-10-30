#!/usr/bin/env python3
"""Regression tests of the text editor core (the text patches are applied by the port; no ISO is built here).
Needs game/disc/Tombi.bin (skipped otherwise).  python3 tools/test_text.py"""
import os, sys, unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import text_core as C

DISC = os.path.join(os.path.dirname(HERE), "game", "disc", "Tombi.bin")


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

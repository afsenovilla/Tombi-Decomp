#!/usr/bin/env python3
"""Tests of the translation editor (tools/text_editor.py, tools/text_catalog.py) and of the port's translation file rule
(port2/shims/tpatch.c, runtime/main.c: translation.tpatch next to tombi2 is loaded first and wins over --text-patch).
Needs game/disc/Tombi.bin (skipped otherwise); the port test also build/port2/tombi2.
  QT_QPA_PLATFORM=offscreen python3 tools/test_translation_tool.py"""
import json, os, shutil, subprocess, sys, tempfile, unittest

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
DISC = os.path.join(ROOT, "game", "disc", "Tombi.bin")
IMPROVED = os.path.join(ROOT, "patches", "es_mejorada.tombipatch.json")
TOMBI2 = os.path.join(ROOT, "build", "port2", "tombi2")


@unittest.skipUnless(os.path.isfile(DISC), "needs game/disc/Tombi.bin")
class Catalog(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import text_core as C, text_catalog as T
        cls.C, cls.T = C, T
        cls.imp = json.load(open(IMPROVED, encoding="utf-8"))
        cls.cat = T.Catalog(C.Source(DISC), "S", cls.imp, cls.imp)

    def test_categories(self):
        cats = {e.cat for e in self.cat.entries}
        for c in ("dialogue", "sign", "system", "item_name", "item_desc", "event_title", "event_name", "event_hint",
                  "place", "memcard", "title_menu", "pause"):
            self.assertIn(c, cats)

    def test_roundtrip_improved(self):
        """Loading the improved translation and writing it back gives the same edits and menus."""
        p = self.cat.to_patch()
        self.assertEqual(p["edits"], self.imp["edits"])
        self.assertEqual(json.dumps(p["menus"], sort_keys=True), json.dumps(self.imp["menus"], sort_keys=True))
        for k in ("verbFirst", "freeTitles", "glyphShift", "portGfx"):
            self.assertEqual(p.get(k), self.imp.get(k), k)

    def test_edit_and_checks(self):
        cat = self.cat
        e = next(x for x in cat.entries if x.kind == "wfm" and ("AREA00/MFNT000S.WFM", 128) in x.targets)
        old = cat.text(e)
        try:
            self.assertEqual(cat.check(e, old), [])
            bad = cat.check(e, old.replace("pelo rosa", "pelo rosa 100% muy muy muy largo"))
            self.assertTrue(any("U+0025" in m for _, m in bad))                      # no '%' glyph
            self.assertTrue(any(m.startswith("preview: wide") for _, m in bad))       # and still the box check
            new = old.replace("¡Eh, tú, el del pelo rosa!", "¡Hola, Tombi pelirrosa!")
            cat.set_text(e, new)
            self.assertEqual(cat.to_patch()["edits"]["AREA00/MFNT000S.WFM"]["128"], new)
            im, _ = cat.preview(e, new)
            self.assertGreater(im.width, 100)
            # item names: one entry for the same name in every MFNT file
            rana = next(x for x in cat.entries if x.cat == "item_name" and x.kind == "wfm" and "Rana" in cat.original(x))
            self.assertGreater(len(rana.targets), 10)
            # a menu text with a letter the pause strip lacks
            p = cat.by_id["m:pause:0"]
            self.assertTrue(cat.check(p, "PAUSX"))
            self.assertEqual(cat.check(p, "PAUSA"), [])
            self.assertEqual(cat.other(e, "B")[-40:], self.C.Project(self.C.Source(DISC)).original("AREA00/MFNT000B.WFM", 128)[-40:])
        finally:
            cat.set_text(e, old)


@unittest.skipUnless(os.path.isfile(DISC), "needs game/disc/Tombi.bin")
class Editor(unittest.TestCase):
    def test_open_edit_save(self):
        os.environ.setdefault("QT_QPA_PLATFORM", "offscreen")
        tmp = tempfile.mkdtemp()
        os.environ["XDG_CONFIG_HOME"] = tmp
        os.environ["APPDATA"] = tmp
        try:
            import i18n
            i18n.set_language("es", save=False)
            import text_editor as TE
            from PySide6.QtWidgets import QApplication
            app = QApplication.instance() or QApplication([])
            w = TE.MainWindow(DISC)
            e = next(x for x in w.cat.entries if x.kind == "wfm" and ("AREA00/MFNT000S.WFM", 128) in x.targets)
            w.select(e.eid)
            self.assertIs(w.cur, e)
            new = w.cat.improved_text(e).replace("¡Eh, tú, el del pelo rosa!", "¡Hola, Tombi pelirrosa!")
            w.e_cur.setPlainText(new)
            w.refresh_checks()
            self.assertNotIn(e.eid, w.errors)
            w.e_cur.setPlainText(new.replace("Hola", "Hola%"))
            w.refresh_checks()
            self.assertIn(e.eid, w.errors)
            self.assertIn("%", w.checks.item(0).text())
            w.e_cur.setPlainText(new)
            w.path = os.path.join(tmp, "mine.tombipatch.json")
            self.assertTrue(w.save())
            p = json.load(open(w.path, encoding="utf-8"))
            self.assertEqual(p["language"], "S")
            self.assertEqual(p["edits"]["AREA00/MFNT000S.WFM"]["128"], new)
            self.assertEqual(p.get("portGfx"), "es")         # started from the improved translation
            w.filter.setCurrentIndex(w.filter.findData("f.changed"))
            self.assertIn(e, w.model.rows)
            w.dirty = False
            w.close()
        finally:
            shutil.rmtree(tmp, ignore_errors=True)


@unittest.skipUnless(os.path.isfile(DISC) and os.path.isfile(TOMBI2) and os.path.isfile(IMPROVED[:-len(".tombipatch.json")] + ".tpatch"),
                     "needs game/disc/Tombi.bin, build/port2/tombi2 and patches/es_mejorada.tpatch")
class PortTranslationFile(unittest.TestCase):
    def run_port(self, cwd, *extra):
        r = subprocess.run([TOMBI2, "--disc", DISC, "--lang", "5", "--headless", "--frames", "2"] + list(extra),
                           cwd=cwd, capture_output=True, text=True, timeout=300)
        return r.stdout + r.stderr

    def test_translation_file_wins(self):
        tmp = tempfile.mkdtemp()
        try:
            shutil.copyfile(IMPROVED[:-len(".tombipatch.json")] + ".tpatch", os.path.join(tmp, "translation.tpatch"))
            out = self.run_port(tmp, "--text-patch", IMPROVED)
            self.assertIn("translation file translation.tpatch found", out)
            self.assertRegex(out, r"es_mejorada\.tpatch skipped: \S+ is already provided by translation\.tpatch")
            out = self.run_port(tmp, "--no-translation-file", "--text-patch", IMPROVED)
            self.assertNotIn("translation file", out)
            self.assertNotIn("skipped", out)
        finally:
            shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    unittest.main()

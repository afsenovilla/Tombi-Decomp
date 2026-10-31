#!/usr/bin/env python3
"""Headless smoke test of the Qt save editor (tools/tombi_save_editor.py) on a synthetic card: open, edit through the
widgets, inline validation, undo, Save (keeps .bak) and Save as.  Skipped when PySide6 is not installed."""
import os, sys, tempfile, unittest
os.environ.setdefault("QT_QPA_PLATFORM", "offscreen")
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import tombi_save as T
from test_tombi_save import fake_card

try:
    import tombi_save_editor as E
except ImportError:
    E = None


@unittest.skipIf(E is None, "PySide6 not installed")
class TestEditorGui(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.app = E.make_app(["test"])

    def setUp(self):
        self.d = tempfile.mkdtemp()
        E.CFG = os.path.join(self.d, "cfg.json")
        self.p = os.path.join(self.d, "card.mcd")
        open(self.p, "wb").write(fake_card())
        self.w = E.Editor(None, cfg={"ui": "en", "theme": "dark"})
        self.w.resize(1300, 820)
        self.w.show()

    def tearDown(self):
        self.w.clean = bytes(self.w.card.data) if self.w.card else None
        self.w.close()

    def test_edit_undo_save(self):
        w = self.w
        self.assertTrue(w.open_card(self.p))
        self.assertFalse(w.dirty())
        w.fields["ap"].spin.setValue(4321)
        self.assertEqual(w.save.get("ap"), 4321)
        self.assertTrue(w.dirty() and w.dirty_chip.isVisibleTo(w))
        w.fields["health"].spin.setValue(16)          # > max HP: inline error, save untouched
        self.assertFalse(w.fields["health"].err.isHidden())
        self.assertEqual(w.save.get("health"), 4)
        w.item_cards[27].chk.setChecked(True)
        self.assertEqual(w.save.inventory(), [27])
        w.ev_view.selectRow(2)
        w.set_events(255)
        self.assertEqual(w.save.event(2), 255)
        w.undo()
        self.assertEqual(w.save.event(2), 0)
        w.undo()
        self.assertEqual(w.save.inventory(), [])
        for i in range(3):
            w.tab_group.button(i).click()
            self.app.processEvents()
        out = os.path.join(self.d, "out.mcd")
        self.assertTrue(w.write(out))
        s = T.Card(out).saves()[0]
        self.assertEqual((s.get("ap"), s.checksum_ok()), (4321, True))
        w.fields["lives"].spin.setValue(9)
        self.assertTrue(w.write(out))                  # overwrite: previous file kept as .bak
        self.assertEqual(T.Card(out + ".bak").saves()[0].get("lives"), 3)
        self.assertEqual(T.Card(out).saves()[0].get("lives"), 9)
        self.assertEqual(open(self.p, "rb").read(), fake_card())

    def test_untouched_roundtrip_and_ui(self):
        w = self.w
        w.open_card(self.p)
        w.set_lang("es")
        w.toggle_theme()
        self.app.processEvents()
        out = os.path.join(self.d, "same.mcd")
        self.assertTrue(w.write(out))
        self.assertEqual(open(out, "rb").read(), fake_card())

    def test_bad_file(self):
        bad = os.path.join(self.d, "x.mcd")
        open(bad, "wb").write(b"nope")
        self.assertFalse(self.w.open_card(bad))
        self.assertEqual(self.w.status.property("sev"), "err")


if __name__ == "__main__":
    unittest.main()

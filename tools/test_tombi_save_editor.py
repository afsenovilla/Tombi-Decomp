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


ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DISC = os.path.join(ROOT, "game", "disc", "fs")      # the developer's own extracted PAL disc (never committed)


@unittest.skipIf(E is None, "PySide6 not installed")
class TestItemIconsAndNames(unittest.TestCase):
    """Item id -> what the Items list draws (subScreenDrawItemIcon) and shows (ITEM_NAME_STRINGS[ITEM_NAME_INDEX[id]])."""

    def test_icon_spec(self):
        cl = bytes(160)
        self.assertEqual(E.item_icon_spec(0, cl), ((192, 0), 8, 8, 0x7813))
        self.assertEqual(E.item_icon_spec(14, cl), ((192, 0), 232, 8, 0x7813))
        self.assertEqual(E.item_icon_spec(15, cl), ((192, 0), 8, 24, 0x7813))
        self.assertEqual(E.item_icon_spec(159, bytes([1]) * 160), ((192, 0), 152, 168, 0x7853))

    def test_item_names_synthetic(self):
        """ITEM_NAME_INDEX (identity, 200 bytes) + ITEM_NAME_STRINGS pointers -> list-font strings, by id."""
        import struct
        base = 0x800E8000
        strings = b""
        offs = []
        for k in range(160):
            offs.append(len(strings))
            strings += struct.pack("<%dh" % (5 + 2), *[ord(c) + 140 for c in "It%03d" % k], -3, -1)
        idx = bytes(range(200))
        head = 0x100
        ptr_at = head + len(idx)
        str_at = ptr_at + 4 * 200
        ptrs = b"".join(struct.pack("<I", base + str_at + offs[min(k, 159)]) for k in range(200))
        blob = bytes(head) + idx + ptrs + strings
        names = T.item_names(blob)
        self.assertEqual((len(names), names[0], names[12], names[159]), (160, "It000", "It012", "It159"))

    def test_patch_names(self):
        p = os.path.join(ROOT, "patches", "es_mejorada.tombipatch.json")
        if not os.path.isfile(p):
            self.skipTest("no bundled patch")
        r = T.patch_item_names(p, "S")
        self.assertEqual((r.get("Bola"), r.get("Garfio-gancho")), ("Mangual", "Manguancho"))
        self.assertEqual(T.patch_item_names(p, "B"), {})

    @unittest.skipUnless(os.path.isfile(os.path.join(DISC, "SYSTEM", "D0SPSUB.BIN")), "no extracted PAL disc")
    def test_pal_disc(self):
        """Pinned from the PAL disc and checked pixel for pixel against the port's Items list (docs/SAVE_EDITOR.md)."""
        dsp = open(os.path.join(DISC, "SYSTEM", "D0SPSUB.BIN"), "rb").read()
        cl = dsp[E.ITEM_ICON_CLUTS - E.D0SPSUB_BASE:][:160]
        self.assertEqual(E.item_icon_spec(12, cl), ((192, 0), 200, 8, 0x7893))     # Tear Jar: a jar
        self.assertEqual(E.item_icon_spec(27, cl), ((192, 0), 200, 24, 0x7853))    # Blackjack / Bola
        self.assertEqual(E.item_icon_spec(28, cl), ((192, 0), 216, 24, 0x7A53))    # Flash Pants
        self.assertEqual(E.item_icon_spec(64, cl), ((192, 0), 72, 72, 0x7853))     # Pipe (grey row 1)
        en = T.names_from_disc(DISC, "B")["items"]
        self.assertEqual((len(en), en[10], en[12], en[21], en[27], en[34]),
                         (160, "Bucket", "Tear Jar", "Wood Boomerang", "Blackjack", "GrappleJack"))
        es = T.names_from_disc(DISC, "S", patch=os.path.join(ROOT, "patches", "es_mejorada.tombipatch.json"))["items"]
        self.assertEqual((es[27], es[34], es[12]), ("Mangual", "Manguancho", "Tarro lágrimas"))
        icons = E.item_icons(DISC)
        self.assertEqual(len(icons), 160)


if __name__ == "__main__":
    unittest.main()

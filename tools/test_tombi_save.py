#!/usr/bin/env python3
"""Tests of tools/tombi_save.py on a synthetic card (no game data): round trip, edits, checksum, inventory, title."""
import os, struct, sys, tempfile, unittest
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import tombi_save as T


def fake_card():
    c = T._blank_card()
    f = bytearray(T.FRAME)
    f[0] = 0x51; struct.pack_into("<I", f, 4, T.BLOCK); f[8] = f[9] = 0xFF
    f[10:30] = b"BESCES-01331TOMBA-03"; f[0x7F] = T.xor(f[:0x7F])
    c[T.FRAME:2 * T.FRAME] = f
    b = T.BLOCK
    c[b:b + 4] = b"SC\x11\x01"
    title = "ＴＯＭＢＩ！−Ｂ［０３］　０００：００　Ｐ＝０３　Ｌｅｖｅｌ０１".encode("shift_jis")
    c[b + 4:b + 4 + len(title)] = title
    g = b + T.GAME_OFS
    c[g + 0x10] = c[g + 0x11] = 4; c[g + 0x20] = 3; c[g + 0x13] = 1
    c[g + T.GAME_LEN - 1] = T.xor(c[g:g + T.GAME_LEN - 1])
    return bytes(c)


def fake_usa_card(slot=2, time=0, lives=3, level=1):
    """a USA (SCUS-94236) save as DSPSUB2 func_800E954C writes it: 'SC' 0x13 (3-frame icon), the one English title
    template with its digits at the USA offsets (slot +0x14, hhh:mm +0x1C, P= +0x2E, Level +0x3E), time in 1/60 s"""
    c = T._blank_card()
    f = bytearray(T.FRAME)
    f[0] = 0x51; struct.pack_into("<I", f, 4, T.BLOCK); f[8] = f[9] = 0xFF
    f[10:30] = b"BASCUS-94236TOMBA-%02d" % slot; f[0x7F] = T.xor(f[:0x7F])
    c[T.FRAME:2 * T.FRAME] = f
    b = T.BLOCK
    c[b:b + 4] = b"SC\x13\x01"
    fw = lambda s: "".join(chr(0xFF10 + int(ch)) for ch in s)
    hh, mm = time // 216000, time // 3600 % 60
    title = ("ＴＯＭＢＡ！　［%s］　%s：%s　Ｐ＝%s　Ｌｅｖｅｌ%s" % (fw("%02d" % slot), fw("%03d" % hh), fw("%02d" % mm),
                                                    fw("%02d" % lives), fw("%02d" % level))).encode("shift_jis")
    c[b + 4:b + 4 + len(title)] = title
    g = b + T.GAME_OFS
    struct.pack_into("<I", c, g + 8, time)
    c[g + 0x10] = c[g + 0x11] = 4; c[g + 0x20] = lives; c[g + 0x13] = level
    c[g + T.GAME_LEN - 1] = T.xor(c[g:g + T.GAME_LEN - 1])
    return bytes(c)


class TestUsaSave(unittest.TestCase):
    """USA saves (docs/SAVE_FORMAT.md "USA saves"): same GAME block, 60 Hz play time, other title offsets"""

    def setUp(self):
        self.d = tempfile.mkdtemp()
        self.p = os.path.join(self.d, "usa.mcd")
        open(self.p, "wb").write(fake_usa_card(slot=2, time=60 * (3600 + 23 * 60 + 5), lives=3, level=1))

    def test_layout(self):
        c = T.Card(self.p)
        self.assertEqual(c.to_bytes(), c.orig)
        s = c.saves()[0]
        self.assertEqual((s.slot, s.region[:3], s.hz), (2, "USA", 60))
        self.assertTrue(s.checksum_ok())
        self.assertEqual(s.hms(), (1, 23, 5))
        self.assertIn(" 1:23 ", s.summary())
        self.assertEqual(T.layout("BESCES-01331TOMBA-00")["hz"], 50)
        self.assertEqual(T.layout("BISLPS-01144TOMBA-00")["hz"], 60)

    def test_edit_title(self):
        c = T.Card(self.p)
        s = c.saves()[0]
        s.set("time", 60 * (12 * 3600 + 34 * 60)); s.set("lives", 9); s.set("level", 7)
        out = os.path.join(self.d, "out.mcd")
        c.save(out)
        s2 = T.Card(out).saves()[0]
        self.assertTrue(s2.checksum_ok())
        # the title as the USA game writes it: the digits at +0x14 / +0x1C / +0x2E / +0x3E, the brackets and colon kept
        self.assertEqual(s2.title(), "ＴＯＭＢＡ！　［０２］　０１２：３４　Ｐ＝０９　Ｌｅｖｅｌ０７")
        self.assertEqual(open(out, "rb").read(), fake_usa_card(slot=2, time=60 * (12 * 3600 + 34 * 60), lives=9, level=7))
        s2.set("time", T.time_max(60))                    # 999:59 at 60 Hz is allowed
        with self.assertRaises(T.SaveError):
            s2.set("time", T.time_max(60) + 1)

    def test_jp_title_untouched(self):
        """JP: 60 Hz, the title layout is not known: an edit keeps the title bytes"""
        raw = bytearray(fake_usa_card())
        raw[T.FRAME + 10:T.FRAME + 22] = b"BISLPS-01144"
        raw[T.FRAME + 0x7F] = T.xor(raw[T.FRAME:T.FRAME + 0x7F])
        p = os.path.join(self.d, "jp.mcd")
        open(p, "wb").write(bytes(raw))
        c = T.Card(p)
        s = c.saves()[0]
        before = s.header
        s.set("time", 60 * 3600); s.set("lives", 5)
        self.assertEqual(s.header, before)
        self.assertEqual(s.hms(), (1, 0, 0))


class TestSave(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()
        self.p = os.path.join(self.d, "card.mcd")
        open(self.p, "wb").write(fake_card())

    def test_roundtrip(self):
        c = T.Card(self.p)
        self.assertEqual(c.to_bytes(), c.orig)
        s = c.saves()[0]
        self.assertEqual((s.slot, s.region[:3]), (3, "PAL"))
        self.assertTrue(s.checksum_ok())

    def test_gme_roundtrip(self):
        g = os.path.join(self.d, "card.gme")
        open(g, "wb").write(b"123-456-STD" + bytes(0xF40 - 11) + fake_card())
        c = T.Card(g)
        self.assertEqual(c.to_bytes(), c.orig)
        self.assertEqual(len(c.saves()), 1)

    def test_edit(self):
        c = T.Card(self.p)
        s = c.saves()[0]
        s.set("ap", 123456); s.set("lives", 7); s.set("time", 5 * 180000 + 33 * 3000)
        s.set_event(2, 255); s.set_item(27, 1); s.set_item(32, 2)
        out = os.path.join(self.d, "out.mcd")
        c.save(out)
        s2 = T.Card(out).saves()[0]
        self.assertTrue(s2.checksum_ok())
        self.assertEqual((s2.get("ap"), s2.get("lives"), s2.event(2), s2.inventory(), s2.item(32)),
                         (123456, 7, 255, [32, 27], 2))
        self.assertIn("００５：３３", s2.title()); self.assertIn("Ｐ＝０７", s2.title())
        s2.set_item(27, 0)
        self.assertEqual(s2.inventory(), [32])
        with self.assertRaises(T.SaveError):
            s2.set("max_health", 12)          # > 8 without the golden bowl
        with self.assertRaises(T.SaveError):
            s2.set("ap", 10 ** 8)

    def test_backup(self):
        c = T.Card(self.p)
        c.saves()[0].set("ap", 5)
        c.save()
        self.assertTrue(os.path.exists(self.p + ".bak"))
        self.assertEqual(open(self.p + ".bak", "rb").read(), fake_card())

    def test_real_cards(self):
        root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
        for n in ("tombi_mc0.mcd", "tombi_mc1.mcd"):
            p = os.path.join(root, n)
            if os.path.exists(p):
                c = T.Card(p)
                self.assertEqual(c.to_bytes(), c.orig)
                for s in c.saves():
                    self.assertTrue(s.checksum_ok())


if __name__ == "__main__":
    unittest.main()

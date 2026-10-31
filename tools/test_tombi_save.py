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

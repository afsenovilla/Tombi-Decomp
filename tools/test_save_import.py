#!/usr/bin/env python3
"""Tests of tools/save_import.py on synthetic saves (no game data): import, same-name conflict, overwrite with the same
and with a different block count, another slot, backups (last 5 kept), the CLI's --overwrite.  The resulting cards
are checked frame by frame (XORs, link chains, free blocks) and loaded with tools/tombi_save.py.

Real cards can be used as the starting point too: TOMBI_TEST_CARDS=DIR (holding tombi_mc0.mcd / tombi_mc1.mcd) copies
them into a temp folder first; the originals are only read (their hash is checked after the run)."""
import hashlib, os, shutil, struct, subprocess, sys, tempfile, unittest
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import save_import as S
import tombi_save as T

FW0 = 0x824F   # Shift-JIS fullwidth '0'


def fake_save(slot=0, area=0, minutes=0, blocks=1, fill=0):
    """an .mcs: directory frame + `blocks` blocks; block 0 = 'SC' title + a GAME block with a valid checksum"""
    name = b"BESCES-01331TOMBA-%02d" % slot
    f = bytearray(S.FRAME)
    f[0] = 0x51; struct.pack_into("<I", f, 4, blocks * S.BLOCK); f[8] = f[9] = 0xFF
    f[10:30] = name; f[0x7F] = S.xor(f[:0x7F])
    d = bytearray(blocks * S.BLOCK)
    d[0:4] = b"SC\x11\x01"
    fw = lambda x: "".join(chr(0xFF10 + int(ch)) for ch in x)     # fullwidth digits, as the game writes them
    t = ("ＴＯＭＢＩ！−Ｂ［%s］　%s：%s" % (fw("%02d" % slot), fw("%03d" % (minutes // 60)), fw("%02d" % (minutes % 60)))).encode("shift_jis")
    d[4:4 + len(t)] = t
    g = 0x200
    struct.pack_into("<H", d, g, area); struct.pack_into("<I", d, g + 8, minutes * 3000)
    d[g + 0x10] = d[g + 0x11] = 8; d[g + 0x20] = 3; d[g + 0x13] = 1
    d[g + 0x40] = fill                          # tells saves apart
    d[g + 0x9FF] = S.xor(d[g:g + 0x9FF])
    for k in range(1, blocks):
        d[k * S.BLOCK:(k + 1) * S.BLOCK] = bytes([0x40 + k + fill]) * S.BLOCK
    return bytes(f) + bytes(d)


def check_card(tc, path):
    """the card is valid: header, every directory frame's XOR, chains that end, no block used twice"""
    c = open(path, "rb").read()
    tc.assertEqual(len(c), S.CARD_SIZE)
    tc.assertEqual(c[:2], b"MC")
    for i in range(0, 36):
        o = i * S.FRAME
        tc.assertEqual(S.xor(c[o:o + 0x7F]), c[o + 0x7F], "frame %d XOR" % i)
    used = {}
    for b, nm, _ in S.list_saves(c):
        ch = S.chain(c, b)
        size = struct.unpack_from("<I", c, b * S.FRAME + 4)[0]
        tc.assertEqual(size, len(ch) * S.BLOCK, nm)
        for k, x in enumerate(ch):
            tc.assertNotIn(x, used, "block %d in %s and %s" % (x, nm, used.get(x)))
            used[x] = nm
            tc.assertEqual(c[x * S.FRAME], 0x51 if k == 0 else 0x53 if k == len(ch) - 1 else 0x52)
    for i in range(1, 16):
        if i not in used:
            tc.assertIn(c[i * S.FRAME], (0xA0, 0xA1, 0xA2, 0xA3), "block %d neither used nor free" % i)
    return c


def save_bytes(c, name):
    b = S.find_save(c, name)
    return b"".join(c[x * S.BLOCK:(x + 1) * S.BLOCK] for x in S.chain(c, b))


class TestSaveImport(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()
        self.saves = os.path.join(self.d, "saves")
        os.makedirs(self.saves)
        self.c0, self.c1 = os.path.join(self.saves, "tombi_mc0.mcd"), os.path.join(self.saves, "tombi_mc1.mcd")
        src = os.environ.get("TOMBI_TEST_CARDS")
        self.orig = {}
        if src:
            for n in ("tombi_mc0.mcd", "tombi_mc1.mcd"):
                p = os.path.join(src, n)
                if os.path.exists(p):
                    self.orig[p] = hashlib.sha1(open(p, "rb").read()).hexdigest()
                    shutil.copyfile(p, os.path.join(self.saves, n))

    def tearDown(self):
        for p, h in self.orig.items():
            self.assertEqual(hashlib.sha1(open(p, "rb").read()).hexdigest(), h, "source card changed: " + p)
        shutil.rmtree(self.d)

    def mcs(self, name, **kw):
        p = os.path.join(self.d, name)
        open(p, "wb").write(fake_save(**kw))
        return p

    def clear_slot0(self):
        """a real card might already hold slot 00: drop it so the test starts from a known state"""
        for path in (self.c0, self.c1):
            if os.path.exists(path):
                c = S.read_card(path)
                b = S.find_save(c, "BESCES-01331TOMBA-00")
                if b is not None:
                    S.delete_save(c, b)
                    open(path, "wb").write(c)

    def test_import_conflict_overwrite(self):
        self.clear_slot0()
        a = self.mcs("a.mcs", slot=0, area=5, minutes=83, fill=1)
        lines, _ = S.import_file(a, self.c0)
        self.assertIn("Ranura 00 · Ciudad Baco · 1:23 h", lines[0])
        check_card(self, self.c0)
        b = self.mcs("b.mcs", slot=0, area=9, minutes=200, fill=2)
        with self.assertRaises(S.Conflict) as cm:
            S.import_file(b, self.c0)
        e = cm.exception
        self.assertEqual((e.existing, e.incoming), ("Ranura 00 · Ciudad Baco · 1:23 h",
                                                    "Ranura 00 · Bosque de setas · 3:20 h"))
        self.assertIn("en la tarjeta. ¿Reemplazarla?", e.question())
        self.assertIn("The memory card already holds", e.question(es=False))
        self.assertIsNotNone(e.new_slot)                  # the card has room and a free slot
        before = open(self.c0, "rb").read()
        blk = S.find_save(before, "BESCES-01331TOMBA-00")
        self.assertEqual(open(self.c0, "rb").read(), before)   # a refused import writes nothing
        lines, _ = S.import_file(b, self.c0, overwrite=True)
        self.assertIn("Sustituye a «Ranura 00 · Ciudad Baco", lines[1])
        c = check_card(self, self.c0)
        self.assertEqual(S.find_save(c, "BESCES-01331TOMBA-00"), blk)   # same place
        self.assertEqual(save_bytes(c, "BESCES-01331TOMBA-00"), fake_save(slot=0, area=9, minutes=200, fill=2)[S.FRAME:])
        self.assertEqual(sum(1 for _, n, _ in S.list_saves(c) if n == "BESCES-01331TOMBA-00"), 1)
        bk = os.listdir(os.path.join(self.saves, "backup"))
        self.assertTrue(any(f.startswith("tombi_mc0-") for f in bk))
        # the backup made before the overwrite is the card as it was
        newest = max((os.path.join(self.saves, "backup", f) for f in bk), key=os.path.getmtime)
        self.assertEqual(open(newest, "rb").read(), before)
        card = T.Card(self.c0)
        s = [x for x in card.saves() if x.name == "BESCES-01331TOMBA-00"][0]
        self.assertTrue(s.checksum_ok())
        self.assertEqual((s.get("area"), s.get("time")), (9, 200 * 3000))

    def test_block_counts(self):
        # card: slot 1 (1 block), slot 0 (3 blocks), slot 2 (1 block); slot 0 -> 1 block -> 4 blocks
        self.clear_slot0()
        for n in ("BESCES-01331TOMBA-01", "BESCES-01331TOMBA-02"):
            if os.path.exists(self.c0):
                c = S.read_card(self.c0)
                if S.find_save(c, n) is not None:
                    S.delete_save(c, S.find_save(c, n)); open(self.c0, "wb").write(c)
        S.import_file(self.mcs("s1.mcs", slot=1, fill=11), self.c0)
        S.import_file(self.mcs("s0.mcs", slot=0, blocks=3, fill=3), self.c0)
        S.import_file(self.mcs("s2.mcs", slot=2, fill=12), self.c0)
        c = check_card(self, self.c0)
        free0 = len(S.free_blocks(c))
        S.import_file(self.mcs("s0b.mcs", slot=0, blocks=1, fill=4), self.c0, overwrite=True)
        c = check_card(self, self.c0)
        self.assertEqual(len(S.free_blocks(c)), free0 + 2)
        self.assertEqual(save_bytes(c, "BESCES-01331TOMBA-00"), fake_save(slot=0, blocks=1, fill=4)[S.FRAME:])
        S.import_file(self.mcs("s0c.mcs", slot=0, blocks=4, fill=5), self.c0, overwrite=True)
        c = check_card(self, self.c0)
        self.assertEqual(len(S.free_blocks(c)), free0 - 1)
        self.assertEqual(save_bytes(c, "BESCES-01331TOMBA-00"), fake_save(slot=0, blocks=4, fill=5)[S.FRAME:])
        # the neighbours are untouched
        self.assertEqual(save_bytes(c, "BESCES-01331TOMBA-01"), fake_save(slot=1, fill=11)[S.FRAME:])
        self.assertEqual(save_bytes(c, "BESCES-01331TOMBA-02"), fake_save(slot=2, fill=12)[S.FRAME:])
        # export of the replaced save round-trips
        self.assertEqual(S.export_save(c, S.find_save(c, "BESCES-01331TOMBA-00"))[S.FRAME:],
                         fake_save(slot=0, blocks=4, fill=5)[S.FRAME:])

    def test_no_room_even_after_replacing(self):
        c = S.blank_card()
        S.insert_save(c, fake_save(slot=0)[:S.FRAME], fake_save(slot=0)[S.FRAME:])
        for k in range(1, 15):
            fs = fake_save(slot=k)
            S.insert_save(c, fs[:S.FRAME], fs[S.FRAME:])
        open(self.c0, "wb").write(c)
        with self.assertRaises(S.ImportError_) as cm:
            S.import_file(self.mcs("big.mcs", slot=0, blocks=2), self.c0, overwrite=True)
        self.assertNotIsInstance(cm.exception, S.Conflict)
        self.assertEqual(open(self.c0, "rb").read(), bytes(c))   # nothing written
        check_card(self, self.c0)

    def test_area_names(self):
        """Spanish / English: the map names of the improved translation / the English disc (by area and section)"""
        self.assertEqual(S.area_name(0), "Ciudad de todo principio")
        self.assertEqual(S.area_name(10, 3), "Ciudad de trucos")
        self.assertEqual(S.area_name(10, 30), "Jungla profunda")     # past the table: section 0's
        self.assertEqual(S.area_name(14, 0), "Área Cerdiablo uno")
        self.assertEqual(S.area_name(6, 2), "Motocross sucio")
        self.assertIsNone(S.area_name(7))
        self.assertEqual(S.area_name(14, 6), "Bosque de setas")
        # English: the English disc's map names (D0SPSUB, as the panel shows them with --lang 0)
        self.assertEqual(S.area_name(5, 0, es=False), "Baccus Village")
        self.assertEqual(S.area_name(4, 3, es=False), "Haunted Mansion")
        self.assertEqual(S.area_name(10, 3, es=False), "Trick Village")
        self.assertEqual(S.area_name(11, 0, es=False), "Village of Civilization")
        self.assertEqual(S.area_name(13, 0, es=False), "Dwarf Forest")
        self.assertEqual(S.area_name(14, 0, es=False), "Evil Pig Area One")
        self.assertEqual(S.area_name(14, 6, es=False), "Mushroom Forest")
        self.assertEqual(S.area_name(19, 2, es=False), "Phoenix's Mountain")
        self.assertIsNone(S.area_name(7, 0, es=False))
        self.assertEqual(len(S.REGIONS_EN), len(S.REGIONS_ES))
        g = bytearray(fake_save(slot=3, area=10, minutes=61))
        struct.pack_into("<H", g, S.FRAME + 0x202, 7)
        g[S.FRAME + 0xBFF] = S.xor(g[S.FRAME + 0x200:S.FRAME + 0xBFF])
        self.assertEqual(S.save_info("BESCES-01331TOMBA-03", g[S.FRAME:]), "Ranura 03 · Ciudad de trucos · 1:01 h")
        self.assertEqual(S.save_info("BESCES-01331TOMBA-03", g[S.FRAME:], es=False), "Slot 03 · Trick Village · 1:01 h")

    def test_new_slot(self):
        """Save to another slot: the save goes in as the first free slot of the same card (file name and title digits),
        the original stays as it was, the card stays valid and tombi_save.py reads the new save"""
        self.clear_slot0()
        for k in (1, 2):
            if os.path.exists(self.c0):
                c = S.read_card(self.c0)
                b = S.find_save(c, "BESCES-01331TOMBA-%02d" % k)
                if b is not None:
                    S.delete_save(c, b); open(self.c0, "wb").write(c)
        S.import_file(self.mcs("a.mcs", slot=0, area=5, minutes=10, fill=1), self.c0)
        S.import_file(self.mcs("s1.mcs", slot=1, fill=11), self.c0)
        b = self.mcs("b.mcs", slot=0, area=9, minutes=20, fill=2)
        with self.assertRaises(S.Conflict) as cm:
            S.import_file(b, self.c0)
        self.assertEqual(cm.exception.new_slot, "02")
        self.assertIn("Ranura 02", cm.exception.question())
        self.assertIn("Slot 02", cm.exception.question(es=False))
        before = open(self.c0, "rb").read()
        lines, _ = S.import_file(b, self.c0, new_slot=True)
        self.assertIn("Ranura 02 · Bosque de setas · 0:20 h", lines[0])
        c = check_card(self, self.c0)
        self.assertEqual(save_bytes(c, "BESCES-01331TOMBA-00"), save_bytes(before, "BESCES-01331TOMBA-00"))
        self.assertEqual(save_bytes(c, "BESCES-01331TOMBA-01"), save_bytes(before, "BESCES-01331TOMBA-01"))
        new = save_bytes(c, "BESCES-01331TOMBA-02")
        exp = bytearray(fake_save(slot=0, area=9, minutes=20, fill=2)[S.FRAME:])
        exp[0x16:0x1A] = "０２".encode("shift_jis")
        self.assertEqual(new, bytes(exp))
        self.assertIn("[02]", S.title(new))
        s = [x for x in T.Card(self.c0).saves() if x.name == "BESCES-01331TOMBA-02"][0]
        self.assertEqual((s.slot, s.get("area"), s.checksum_ok()), (2, 9, True))
        self.assertTrue(os.listdir(os.path.join(self.saves, "backup")))
        # CLI
        r = subprocess.run([sys.executable, os.path.join(HERE, "save_import.py"), "import", b, self.c0, "--new-slot"],
                           capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertIn("Ranura 03", r.stdout)
        check_card(self, self.c0)

    def test_new_slot_not_offered(self):
        c = S.blank_card()
        for k in range(15):
            fs = fake_save(slot=k)
            S.insert_save(c, fs[:S.FRAME], fs[S.FRAME:])
        open(self.c0, "wb").write(c)
        with self.assertRaises(S.Conflict) as cm:            # a full card: no room, no free slot
            S.import_file(self.mcs("b.mcs", slot=0, fill=2), self.c0)
        self.assertIsNone(cm.exception.new_slot)
        self.assertNotIn("otra ranura", cm.exception.question())
        with self.assertRaises(S.ImportError_):
            S.import_file(os.path.join(self.d, "b.mcs"), self.c0, new_slot=True)
        self.assertEqual(open(self.c0, "rb").read(), bytes(c))

    def test_backups_keep_last_5(self):
        open(self.c0, "wb").write(S.blank_card())
        for k in range(8):
            S.import_file(self.mcs("x.mcs", slot=0, fill=k), self.c0, overwrite=True)
        bk = [f for f in os.listdir(os.path.join(self.saves, "backup")) if f.startswith("tombi_mc0-")]
        self.assertEqual(len(bk), 5)
        self.assertTrue(all(f.endswith(".mcd") for f in bk))

    def test_whole_card_backup(self):
        open(self.c0, "wb").write(S.blank_card())
        full = os.path.join(self.d, "full.mcr")
        c = S.blank_card()
        fs = fake_save(slot=4)
        S.insert_save(c, fs[:S.FRAME], fs[S.FRAME:])
        open(full, "wb").write(c)
        lines, _ = S.import_file(full, self.c0)
        self.assertIn("backup/tombi_mc0-", lines[0])
        self.assertEqual(open(self.c0, "rb").read(), bytes(c))

    def test_cli_overwrite(self):
        self.clear_slot0()
        a = self.mcs("a.mcs", slot=0, fill=1)
        S.import_file(a, self.c0)
        b = self.mcs("b.mcs", slot=0, fill=2)
        tool = os.path.join(HERE, "save_import.py")
        r = subprocess.run([sys.executable, tool, "import", b, self.c0], capture_output=True, text=True)
        self.assertNotEqual(r.returncode, 0)
        self.assertIn("Reemplazarla", r.stderr)
        r = subprocess.run([sys.executable, tool, "import", b, self.c0, "--overwrite"], capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertIn("Sustituye a", r.stdout)
        c = check_card(self, self.c0)
        self.assertEqual(save_bytes(c, "BESCES-01331TOMBA-00"), fake_save(slot=0, fill=2)[S.FRAME:])

    def test_usa_save(self):
        """a USA (SCUS-94236) save (test_tombi_save.fake_usa_card: 1/60 s play time, slot digits at +0x14): its play
        time in the info line, and a new slot renumbers the title at the USA offset"""
        import test_tombi_save as TT
        card = TT.fake_usa_card(slot=0, time=60 * (2 * 3600 + 5 * 60), lives=4, level=2)
        mcs = card[T.FRAME:2 * T.FRAME] + card[T.BLOCK:2 * T.BLOCK]
        p = os.path.join(self.d, "usa.mcs")
        open(p, "wb").write(mcs)
        dest = os.path.join(self.d, "usa", "tombi_mc0.mcd")
        os.makedirs(os.path.dirname(dest))
        self.assertEqual(S.save_info("BASCUS-94236TOMBA-00", mcs[S.FRAME:], es=False),
                         "Slot 00 · Village of All Beginnings · 2:05 h")
        S.import_file(p, dest)
        with self.assertRaises(S.Conflict) as cm:
            S.import_file(p, dest)
        self.assertEqual(cm.exception.new_slot, "01")
        S.import_file(p, dest, new_slot=True)
        c = check_card(self, dest)
        new = save_bytes(c, "BASCUS-94236TOMBA-01")
        exp = bytearray(mcs[S.FRAME:])
        exp[0x14:0x18] = "０１".encode("shift_jis")
        self.assertEqual(new, bytes(exp))
        self.assertIn("[01]", S.title(new))
        s = [x for x in T.Card(dest).saves() if x.name == "BASCUS-94236TOMBA-01"][0]
        self.assertEqual((s.slot, s.hz, s.hms(), s.checksum_ok()), (1, 60, (2, 5, 0), True))


if __name__ == "__main__":
    unittest.main()

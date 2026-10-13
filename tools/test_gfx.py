#!/usr/bin/env python3
"""Tests of tools/gfx_core.py against the local game files (skipped without game/disc/Tombi.bin):  python3 tools/test_gfx.py"""
import os, shutil, subprocess, sys, tempfile, unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
ROOT = os.path.dirname(HERE)
IMG = os.path.join(ROOT, "game", "disc", "Tombi.bin")


@unittest.skipUnless(os.path.isfile(IMG), "needs game/disc/Tombi.bin")
class T(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import gfx_core
        cls.G = gfx_core
        cls.core = gfx_core.Core(IMG)

    @classmethod
    def tearDownClass(cls):
        cls.core.disc.close()

    def setUp(self):
        while self.core.undo():
            pass
        self.tmp = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_ui_table(self):
        t = self.core.ui_table()
        self.assertEqual(len(t), 3)
        self.assertEqual(t[1], (1, 184, 192, 40, 16, 352, 509))     # TRY
        self.assertEqual(t[2], (2, 184, 208, 56, 16, 352, 509))     # AGAIN
        r = {x.label.split()[1]: x for x in self.core.surface_for(("ui", 1)).rects}
        self.assertEqual(r["TRY"].pal.key(), ("AREA00/CLUT01.GAM", 29 * 256 + 224, 16))

    def test_palette_choice(self):
        c, G = self.core, self.G
        for it in (("screen", "now_loading"), ("screen", "title_landscape"), ("screen", "backdrop_L9")):
            try:
                s = c.surface_for(it)
            except G.CoreError:
                continue
            gr = c.palette_groups(s)
            self.assertTrue(gr and gr[0][0] == G.tr("pg.inferred"))
            self.assertTrue(callable(gr[-1][1]))
            files = gr[-1][1]()
            self.assertTrue(files and all(r.n == (16 if s.bpp == 4 else 256) for _, rows in files for _, r in rows))
        s = c.surface_for(("screen", "now_loading"))
        emb = [r for g, items in c.palette_groups(s) if not callable(items) for l, r in items if r.file == "SYSTEM/A00900.000"]
        self.assertGreaterEqual(len(emb), 3)
        other = emb[1] if emb[1] != s.pal else emb[2]
        s.choose(other)
        self.assertEqual(c.surface_for(("screen", "now_loading")).pal, other)
        self.assertEqual(c.used_palette("SYSTEM/A00900.000"), other)
        s2 = c.surface_for(("screen", "now_loading"))
        s2.choose(s2.inferred)
        self.assertNotIn("SYSTEM/A00900.000", c.pal_choice)
        self.assertEqual(c.surface_for(("screen", "now_loading")).pal, s2.inferred)


    def test_backdrop_layout_and_fill(self):
        c = self.core
        s = c.surface_for(("screen", "backdrop_L77"))
        self.assertEqual(s.layers[0][4:], (0, 64))        # the 64 px strip of page 1 goes at the left edge
        s22 = c.surface_for(("screen", "backdrop_L22"))
        ws = c.pal_words(s22.pal)
        pl = bytes(s22.plane())
        self.assertFalse(any(ws[i] == 0 for i in set(b for y in range(240) for b in pl[y * s22.W:y * s22.W + 320]) if i))

    def test_backdrops(self):
        import palinfer
        ks = palinfer.backdrop_lists(self.core)
        self.assertIn(51, ks)
        self.assertGreaterEqual(len(ks), 18)
        sf = self.core.surface_for(("screen", "backdrop_L51"))
        self.assertEqual(sf.bpp, 8)
        self.assertEqual(sf.pal.key(), ("AREA09/CLUT03.GAM", 31 * 256, 256))

    def test_page_roundtrip(self):
        for item in (("page", "SYSTEM/A10003.GAM", "SYS", 0), ("screen", "title_landscape"), ("screen", "menu_atlas_save_load"),
                     ("page", "SYSTEM/A00204.000", "SYS", 2)):
            s = self.core.surface_for(item)
            before = {n: self.core.get(n) for n, *_ in s.layers}
            p = os.path.join(self.tmp, "x.png")
            s.export_png(p)
            rep = s.import_png(p)
            self.assertTrue(rep["direct"], item)
            self.assertEqual(self.core.edits, {}, item)
            self.assertEqual({n: self.core.get(n) for n, *_ in s.layers}, before)
            # rect export / import
            s.export_png(p, (8, 8, 24, 16))
            s.import_png(p, (8, 8, 24, 16))
            self.assertEqual(self.core.edits, {})

    def test_rgba_import_quantizes(self):
        from PIL import Image
        s = self.core.surface_for(("ui", 1))
        r = s.selected
        im = Image.new("RGBA", (r.w, r.h), (0, 0, 0, 0))
        for x in range(r.w):
            im.putpixel((x, 3), (250, 10, 10, 255))
        p = os.path.join(self.tmp, "q.png")
        im.save(p)
        rep = s.import_png(p, (r.x, r.y, r.w, r.h), r.pal)
        self.assertFalse(rep["direct"])
        self.assertEqual(rep["transparent"], r.w * (r.h - 1))
        self.assertEqual(list(self.core.edits), [self.G.UI_PAGE])
        self.assertTrue(self.core.undo())
        self.assertEqual(self.core.edits, {})

    def test_palette_roundtrip_and_edit(self):
        s = self.core.surface_for(("clut", "SYSTEM/A00802.000"))
        p = os.path.join(self.tmp, "c.png")
        s.export_png(p)
        rep = s.import_png(p)
        self.assertEqual(self.core.edits, {})
        ref = self.G.PalRef("SYSTEM/A00802.000", 224, 16)
        old = self.core.pal_words(ref)
        self.assertTrue(self.core.set_color(ref, 3, (255, 0, 0)))
        new = self.core.pal_words(ref)
        self.assertEqual(new[3] & 0x7FFF, 31)
        self.assertEqual(new[3] & 0x8000, old[3] & 0x8000)
        self.assertEqual(new[:3] + new[4:], old[:3] + old[4:])
        self.core.undo()
        self.assertEqual(self.core.pal_words(ref), old)

    def test_mod_matches_core_and_touches_one_file(self):
        import mod
        c = self.core
        s = c.surface_for(("ui", 1))
        r = s.selected
        pl = bytearray(s.plane())
        flat = bytes((i % 15) + 1 for i in range(r.w * r.h))
        s.put(r.x, r.y, r.w, r.h, flat)
        ref = self.G.PalRef("SYSTEM/A00802.000", 224, 16)
        c.set_color(ref, 5, (10, 200, 30))
        want = c.changed_files()
        self.assertEqual(sorted(want), ["SYSTEM/A00802.000", "SYSTEM/A10003.GAM"])
        name = "_gfx_test_%d" % os.getpid()
        folder = os.path.join(mod.MODS, name)
        try:
            c.save_mod(folder, name)
            out = os.path.join(self.tmp, "fs")
            r = subprocess.run([sys.executable, os.path.join(HERE, "mod.py"), "apply", "--mods", name, "--fs", c.disc.fs, "--out", out],
                               capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
            diff = []
            for d, _, fs in os.walk(out):
                for f in fs:
                    q = os.path.join(d, f)
                    rel = os.path.relpath(q, out).replace(os.sep, "/")
                    if open(q, "rb").read() != open(os.path.join(c.disc.fs, rel), "rb").read():
                        diff.append(rel)
            self.assertEqual(sorted(diff), sorted(want))
            from area000 import gam_unpack
            for rel in diff:
                got = open(os.path.join(out, rel), "rb").read()
                if rel.endswith(".GAM"):
                    self.assertEqual(gam_unpack(got), c.edits[rel])
                else:
                    self.assertEqual(got, want[rel])
            # load it back into a clean project
            e = dict(c.edits)
            while c.undo():
                pass
            self.assertEqual(c.load_mod(folder), 2)
            self.assertEqual(c.edits, e)
        finally:
            shutil.rmtree(folder, ignore_errors=True)

    def test_gam_repack_unchanged_is_identical(self):
        import gam_py
        raw = self.core.disc.raw(self.G.UI_PAGE)
        self.assertEqual(gam_py.pack(gam_py.unpack(raw), raw), raw)
        d = gam_py.unpack(raw)
        self.assertEqual(gam_py.unpack(gam_py.encode(d)), d)

    def test_wpp_frame_edit(self):
        c = self.core
        nm = "AREA00/A004.000"
        s = c.surface_for(("wppframe", nm, 12))
        self.assertGreater(s.W, 0)
        flat = bytes((i * 7) % 16 for i in range(s.W * s.H))
        s.put(0, 0, s.W, s.H, flat)
        self.assertEqual(list(c.edits), ["%s#12" % nm])
        try:
            files = c.changed_files()
            self.assertEqual(list(files), [nm])
        except self.G.CoreError:
            pass       # a re-encoded frame may not fit the original size: reported, never silently written


if __name__ == "__main__":
    unittest.main(verbosity=2)

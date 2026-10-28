#!/usr/bin/env python3
"""Tests of tools/text_patch.py: text patch make/merge/apply round trip and PPF3 make/apply (must reproduce the rebuilt image
byte for byte).  Needs game/disc/Tombi.bin and mkpsxiso (skipped otherwise).  python3 tools/test_text_patch.py"""
import filecmp, json, os, shutil, sys, tempfile, unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import text_core as C
import text_patch as P

DISC = os.path.join(os.path.dirname(HERE), "game", "disc", "Tombi.bin")


class Offline(unittest.TestCase):
    def test_merge_and_ppf_roundtrip(self):
        a = {"format": P.FORMAT, "version": 1, "edits": {"X/A.WFM": {"0": "a", "1": "b"}}}
        b = {"format": P.FORMAT, "version": 1, "verbFirst": True, "edits": {"X/A.WFM": {"1": "c"}, "Y/B.WFM": {"2": "d"}}}
        m, w = P.merge([a, b])
        self.assertEqual(m["edits"], {"X/A.WFM": {"0": "a", "1": "c"}, "Y/B.WFM": {"2": "d"}})
        self.assertTrue(m["verbFirst"]) ; self.assertEqual(len(w), 1)
        tmp = tempfile.mkdtemp()
        try:
            o, n, p, r = (os.path.join(tmp, x) for x in ("o.bin", "n.bin", "p.ppf", "r.bin"))
            data = bytearray(os.urandom(3000))
            open(o, "wb").write(data)
            data[10:20] = b"x" * 10; data[500:900] = b"y" * 400; data += b"tail"
            open(n, "wb").write(data)
            P.ppf_write(o, n, p, "test sha1 " + P.sha1_file(o)[:24])
            P.ppf_check(p, o)
            P.ppf_apply(o, p, r)
            self.assertTrue(filecmp.cmp(n, r, shallow=False))
        finally:
            shutil.rmtree(tmp)


def free_gb():
    return shutil.disk_usage(tempfile.gettempdir()).free / 2 ** 30


@unittest.skipUnless(os.path.isfile(DISC), "needs game/disc/Tombi.bin")
class Files(unittest.TestCase):
    def test_make_apply_files(self):
        tmp = tempfile.mkdtemp(prefix="tp_")
        try:
            proj = C.Project(C.Source(DISC))
            n = next(x for x in proj.src.names() if x.upper().endswith("CFNT999S.WFM"))
            proj.set_text(n, 0, "¡LOGRADO!")
            patch = P.make(proj, "me", "test")
            self.assertEqual(patch["edits"], {n: {"0": "¡LOGRADO!"}})
            self.assertEqual(patch["disc"]["sha1"], P.sha1_file(DISC))
            pj = os.path.join(tmp, "fix.tombipatch.json")
            P.save(patch, pj)
            fo = os.path.join(tmp, "files")
            res = P.apply(DISC, P.load(pj), None, files_out=fo)
            self.assertIn(n, res["files"])
            self.assertEqual(C.Project(C.Source(fo)).original(n, 0), "¡LOGRADO!")
            bad = dict(patch, disc={"sha1": "0" * 40})
            self.assertRaises(C.CoreError, P.apply, DISC, bad, None, files_out=fo)
        finally:
            shutil.rmtree(tmp)


@unittest.skipUnless(os.path.isfile(DISC) and C.have_mkpsxiso() and free_gb() > 3, "needs game/disc/Tombi.bin, mkpsxiso, 3 GB free")
class Disc(unittest.TestCase):
    def test_make_apply(self):
        tmp = tempfile.mkdtemp(prefix="tp_")
        try:
            proj = C.Project(C.Source(DISC))
            n = next(x for x in proj.src.names() if x.upper().endswith("CFNT999S.WFM"))
            proj.set_text(n, 0, "¡LOGRADO!")
            patch = P.make(proj, "me", "test")
            self.assertEqual(patch["edits"], {n: {"0": "¡LOGRADO!"}})
            self.assertEqual(patch["disc"]["sha1"], P.sha1_file(DISC))
            pj = os.path.join(tmp, "fix.tombipatch.json")
            P.save(patch, pj)
            out = os.path.join(tmp, "A.bin")
            res = P.apply(DISC, P.load(pj), out)
            self.assertEqual(C.Project(C.Source(out)).original(n, 0), "¡LOGRADO!")
            self.assertTrue(os.path.isfile(os.path.join(tmp, "A.cue")))
            print("rebuilt", os.path.getsize(DISC), "->", os.path.getsize(out))
            # PPF of the same edits reproduces the rebuilt image
            ppf = os.path.join(tmp, "fix.ppf")
            P.make_ppf(proj, ppf)
            print("ppf bytes", os.path.getsize(ppf))
            self.assertLess(os.path.getsize(ppf), 4 << 20)
            P.ppf_check(ppf, DISC)
            out2 = os.path.join(tmp, "B.bin")
            P.ppf_apply(DISC, ppf, out2)
            self.assertTrue(filecmp.cmp(out, out2, shallow=False))
            # wrong disc is refused
            bad = dict(patch, disc={"sha1": "0" * 40})
            self.assertRaises(C.CoreError, P.apply, DISC, bad, os.path.join(tmp, "C.bin"))
            fo = os.path.join(tmp, "files")
            self.assertIn(n, P.apply(DISC, bad, None, force=True, files_out=fo)["files"])
        finally:
            shutil.rmtree(tmp)


if __name__ == "__main__":
    unittest.main()

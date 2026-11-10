#!/usr/bin/env python3
"""Headless test of the PC launcher (tools/port_launcher.py, Qt): the tombi-pc command line built for a set of saved
settings (the expected lines are the ones the earlier Tk launcher built for the same tombi-port.json, so the Qt
rewrite passes the port exactly the same arguments), the settings written back, a few widget interactions (key
capture, segmented buttons, controller table, frame rate) and the save editor opened in a window of the launcher.
Skipped when PySide6 is not installed.

  QT_QPA_PLATFORM=offscreen python3 tools/test_port_launcher.py
"""
import json, os, shutil, sys, tempfile, unittest
os.environ.setdefault("QT_QPA_PLATFORM", "offscreen")
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

try:
    import PySide6  # noqa: F401
except ImportError:
    PySide6 = None
if PySide6 is not None:
    from PySide6.QtCore import Qt
    from PySide6.QtTest import QTest
    import i18n
    import port_launcher as L

# settings ({APP} = the launcher folder of the test) -> the expected arguments after the binary
CASES = {
    'defaults': {'cfg': {}, 'expect': ['--disc', '', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--skip-intro', '--hz', '60']},
    'defaults_es': {'cfg': {}, 'ui': 'es', 'expect': ['--disc', '', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--lang', '5', '--skip-intro', '--hz', '60']},
    'disc_only': {'cfg': {'disc': '{APP}/game.cue', 'savedir': '{APP}/saves'}, 'expect': ['--disc', '{APP}/game.cue', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--save-dir', '{APP}/saves', '--skip-intro', '--hz', '60']},
    'everything': {'cfg': {'disc': '{APP}/tombi.bin', 'savedir': '{APP}/my saves', 'scale': 3, 'fullscreen': True, 'renderer': 'gl', 'internal_res': 4, 'aspect': '16:9', 'lang': 5, 'cut_content': True, 'early_title': '{APP}/TOMBA', 'texpack': '{APP}', 'experimental': True, 'show_fps': True, 'dualshock': True, 'rumble': 50, 'pad_map': {'cross': 'b', 'circle': 'a', 'start': 'guide', 'select': ''}, 'prompts_style': 'custom', 'prompt_colors': 'nintendo:ff0000/ffffff,00ff00/000000,0000ff/ffffff,ffff00/000000', 'keys_on': True, 'keys': {'cross': 'Space', 'circle': 'Return', 'up': 'W'}, 'areasel': True, 'area': 4, 'section': 7, 'ap_on': True, 'ap': 12345, 'lives_on': True, 'lives': 9, 'inflives': True, 'infhealth': True, 'events': True, 'debugmenu': True, 'titledebug': True, 'skipintro': False, 'hz60': True, 'autostart': True, 'civ_route': True, 'civ_collision': True, 'fps': '120'}, 'expect': ['--disc', '{APP}/tombi.bin', '--window', '--scale', '3', '--log-fps', '2', '--renderer', 'gl', '--aspect', '16:9', '--internal-res', '4', '--fps', '120', '--lang', '5', '--cut-content', '--early-title', '{APP}/TOMBA', '--save-dir', '{APP}/my saves', '--texture-pack', '{APP}', '--fullscreen', '--show-fps', '--dualshock', '--rumble', '50', '--pad-map', 'cross=b,circle=a,start=guide,select=', '--prompts', 'custom', '--prompt-colors', 'nintendo:ff0000/ffffff,00ff00/000000,0000ff/ffffff,ffff00/000000', '--hz', '60', '--poke', '900:IS_DEBUG_MODE_ENABLED:1', '--poke', '1250:GAME:4:2', '--poke', '1250:GAME+2:7:2', '--pad-script', '1000:start,1300:circle:3', '--title-debug', '--poke', 'g1:GAME+C:3039:4', '--poke', 'g1:GAME+20:9:1', '--poke-every', '50:GAME+20:63:1', '--poke-every', '1:GAME+10:4:1', '--poke', 'g1:SCRIPT_FLAGS:FF:1x130', '--debug-menu', '--keys', 'up=W,cross=Space,circle=Return', '--civ-route', '--civ-maps', '{APP}/civ_cache/maps', '--civ-wfm', '{APP}/civ_cache/wfm', '--civ-collision']},
    'soft_odd': {'cfg': {'renderer': 'soft', 'internal_res': 3, 'aspect': 'weird', 'prompts_style': 'thor_abxy', 'rumble': 'abc', 'fps': 'unlimited', 'texpack': '{APP}', 'experimental': False, 'scale': '1'}, 'expect': ['--disc', '', '--window', '--scale', '1', '--log-fps', '2', '--renderer', 'soft', '--aspect', '4:3', '--fps', 'unlimited', '--skip-intro', '--hz', '60']},
    'fps60_legacy': {'cfg': {'fps60': True, 'experimental': True}, 'expect': ['--disc', '', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--fps', 'auto', '--skip-intro', '--hz', '60']},
    'fps_auto': {'cfg': {'fps': 'auto', 'renderer': 'soft'}, 'expect': ['--disc', '', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'soft', '--aspect', '4:3', '--fps', 'auto', '--skip-intro', '--hz', '60']},
    'data_es': {'cfg': {'lang': 5, 'patch_on': True}, 'data': True, 'expect': ['--data', '{APP}/data', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--lang', '5', '--patch-tex', '{APP}/data/port_gfx_es.tbtx', '--skip-intro', '--hz', '60']},
    'data_en_gfx': {'cfg': {'lang': 0, 'cut_content': True}, 'data': True, 'expect': ['--data', '{APP}/data', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--cut-content', '--skip-intro', '--hz', '60']},
    'civ_town': {'cfg': {'civ_collision': True}, 'data': True, 'civ_play': 16, 'expect': ['--data', '{APP}/data', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--skip-intro', '--hz', '60', '--civ-town', '16', '--civ-maps', '{APP}/civ_cache/maps', '--civ-wfm', '{APP}/civ_cache/wfm', '--civ-collision']},
    'keys_default_on': {'cfg': {'keys_on': True, 'keys': {'up': 'Up'}, 'prompts_style': 'xbox', 'lang': '6', 'rumble': 0}, 'expect': ['--disc', '', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--lang', '6', '--rumble', '0', '--prompts', 'xbox', '--skip-intro', '--hz', '60']},
    'keys_off_custom': {'cfg': {'keys_on': False, 'keys': {'up': 'W'}, 'pad_map': {'up': 'foo', 'down': 'leftx-'}, 'prompts_style': 'generic'}, 'expect': ['--disc', '', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--pad-map', 'up=foo,down=leftx-', '--prompts', 'generic', '--skip-intro', '--hz', '60']},
    'area_door': {'cfg': {'areasel': True, 'area': 17, 'section': 0, 'ap': 500, 'lives': 3}, 'expect': ['--disc', '', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--skip-intro', '--hz', '60', '--poke', '900:IS_DEBUG_MODE_ENABLED:1', '--poke', '1250:GAME:11:2', '--poke', '1250:GAME+2:1:2', '--pad-script', '1000:start,1300:circle:3']},
    'area_bad': {'cfg': {'areasel': True, 'area': 99, 'section': 2}, 'expect': ['--disc', '', '--window', '--scale', '2', '--log-fps', '2', '--renderer', 'gl', '--aspect', '4:3', '--skip-intro', '--hz', '60', '--poke', '900:IS_DEBUG_MODE_ENABLED:1', '--poke', '1250:GAME:0:2', '--poke', '1250:GAME+2:2:2', '--pad-script', '1000:start,1300:circle:3']},
}


def subst(v, d):
    if isinstance(v, str):
        return v.replace("{APP}", d)
    if isinstance(v, dict):
        return {k: subst(x, d) for k, x in v.items()}
    if isinstance(v, list):
        return [subst(x, d) for x in v]
    return v


@unittest.skipIf(PySide6 is None, "PySide6 not installed")
class TestLauncher(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.app = L.make_app(["test"])
        cls.app_dir = L.app_dir

    @classmethod
    def tearDownClass(cls):
        L.app_dir = cls.app_dir

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.w = None

    def tearDown(self):
        if self.w is not None:
            self.w.close()
            self.w.deleteLater()
            self.app.processEvents()
        shutil.rmtree(self.tmp, ignore_errors=True)

    def launcher(self, name, cfg, ui="en", data=False):
        d = os.path.join(self.tmp, name)
        os.makedirs(d)
        with open(os.path.join(d, "tombi-port.json"), "w") as f:
            json.dump(subst(cfg, d), f)
        if data:          # a "complete extraction" of one file, with the Spanish redraws next to it
            dd = os.path.join(d, "data")
            os.makedirs(os.path.join(dd, "EXE"))
            open(os.path.join(dd, "EXE", "MAIN0.EXE"), "wb").write(b"x" * 10)
            open(os.path.join(dd, "manifest.txt"), "w").write(
                "tombi-data 1\nmain0_sha1 %s\nfile 0 0 0 10 EXE/MAIN0.EXE\n" % L.MAIN0_SHA1)
            open(os.path.join(dd, "port_gfx_es.tbtx"), "wb").write(b"t")
        L.app_dir = lambda: d
        i18n.set_language(ui, save=False)
        self.w = L.App()
        self.w.show()
        self.app.processEvents()
        return d

    def args(self):
        return self.w.command()[1:]

    def test_command_lines_as_the_tk_launcher(self):
        for name, c in CASES.items():
            with self.subTest(name):
                d = self.launcher(name, c["cfg"], c.get("ui", "en"), c.get("data", False))
                self.w.civ_play = c.get("civ_play")
                norm = lambda l: [x.replace("\\", "/") if isinstance(x, str) else x for x in l]   # Windows paths
                got = self.args()
                # no translation.tombipatch.json / .tpatch installed: tombi2 is told not to look for one (it would load it)
                self.assertIn("--no-translation-file", got)
                got = [x for x in got if x != "--no-translation-file"]
                self.assertEqual(norm(got), norm(subst(c["expect"], d)))
                self.w.close()
                self.w.deleteLater()
                self.w = None

    def test_import_translation(self):
        """Import translation...: the file and its compiled twin get the fixed names next to the launcher, "Your translation"
        is chosen, and tombi2 loads it by itself (no --no-translation-file) when data/ cannot take the JSON."""
        src = os.path.join(self.tmp, "src")
        os.makedirs(src)
        js = os.path.join(src, "mine.tombipatch.json")
        json.dump({"format": "tombi-text-patch", "version": 1, "title": "Mi traducción", "edits": {}}, open(js, "w", encoding="utf-8"))
        open(os.path.join(src, "mine.tpatch"), "wb").write(b"TPCH" + bytes(60))
        d = self.launcher("imp", {"disc": "", "lang": 5})
        self.assertEqual(L.translation_files(d), (None, None))
        self.w._open_file = lambda *a: js
        self.w.import_translation()
        j, t = L.translation_files(d)
        self.assertTrue(j and t)
        self.assertEqual(L.translation_title(d), "Mi traducción")
        self.assertEqual(self.w._patch_mode(), "translation")
        self.assertTrue(L.translation_native(self.w.s))
        self.assertNotIn("--no-translation-file", self.args())
        # the bundled improved translation chosen again: tombi2 must not pick the file up
        self.w.s["patch"] = L.bundled_patch() or ""
        self.assertNotEqual(self.w._patch_mode(), "translation")
        self.assertIn("--no-translation-file", self.args())
        # a .tpatch alone replaces the JSON (an older JSON would win over it)
        tp = os.path.join(src, "other.tpatch")
        open(tp, "wb").write(b"TPCH" + bytes(60))
        L.import_translation(tp, d)
        self.assertEqual(L.translation_files(d), (None, os.path.join(d, L.TRANSLATION_TPATCH)))
        bad = os.path.join(src, "bad.tombipatch.json")
        with open(bad, "w") as f:
            f.write("{}")
        with self.assertRaises(ValueError):
            L.import_translation(bad, d)
        L.remove_translation(d)
        self.assertEqual(L.translation_files(d), (None, None))

    def test_settings_written_back(self):
        d = self.launcher("saved", {"keys_on": True, "keys": {"up": "W"}, "prompts_style": "thor_abxy", "rumble": "abc",
                                    "scale": "3", "lang": "6"})
        self.w.collect()
        s = json.load(open(os.path.join(d, "tombi-port.json")))
        self.assertEqual(s["keys"]["up"], "W")
        self.assertTrue(s["keys_on"])
        self.assertEqual(s["prompts_style"], "auto")         # the Thor styles are Android only
        self.assertEqual(s["rumble"], "abc")                 # kept as saved (the combobox shows 100 %)
        self.assertEqual((s["scale"], s["lang"]), (3, 6))
        self.assertEqual(s["pad_map"], L.PAD_DEFAULTS)

    def test_key_capture(self):
        self.launcher("keys", {})
        b = self.w.key_btns["cross"]
        QTest.mouseClick(b, Qt.LeftButton)
        self.assertEqual(self.w.key_capture, "cross")
        QTest.keyClick(b, Qt.Key_Space)
        self.assertIsNone(self.w.key_capture)
        self.assertEqual(self.w.key_vals["cross"], "Space")
        QTest.mouseClick(b, Qt.LeftButton)
        QTest.keyClick(b, Qt.Key_Escape)                       # cancels
        self.assertEqual(self.w.key_vals["cross"], "Space")
        b = self.w.key_btns["circle"]
        QTest.mouseClick(b, Qt.LeftButton)
        QTest.keyClick(b, Qt.Key_5, Qt.KeypadModifier)
        QTest.mouseClick(self.w.key_btns["l1"], Qt.LeftButton)
        QTest.keyClick(self.w.key_btns["l1"], Qt.Key_F7)
        a = self.args()
        self.assertEqual(a[a.index("--keys") + 1], "cross=Space,circle=Keypad 5,l1=F7")
        self.w._keys_restore()
        self.assertNotIn("--keys", self.args())

    def test_widgets_change_the_command(self):
        self.launcher("widgets", {})
        w = self.w
        seg = [b for b in w.pages["gfx"].findChildren(L.QPushButton) if b.text() == "3x"][0]
        QTest.mouseClick(seg, Qt.LeftButton)
        fps = [b for b in w.pages["gfx"].findChildren(L.QPushButton) if b.text() == "90"][0]
        QTest.mouseClick(fps, Qt.LeftButton)
        soft = [b for b in w.pages["gfx"].findChildren(L.QRadioButton) if b.text() == L.tr("r.soft")][0]
        soft.setChecked(True)
        self.assertFalse(w.ires_btns[1].isEnabled())
        cb = w.pad_cbs["cross"]
        cb.setCurrentIndex(cb.findText(L.tr("src.b")))
        cb.activated.emit(cb.currentIndex())
        a = self.args()
        self.assertEqual(a[a.index("--scale") + 1], "3")
        self.assertEqual(a[a.index("--fps") + 1], "90")
        self.assertEqual(a[a.index("--renderer") + 1], "soft")
        self.assertEqual(a[a.index("--pad-map") + 1], "cross=b")
        self.assertIn(L.tr("gp.clash", L.tr("src.b"), L.tr("btn.cross") + ", " + L.tr("btn.circle")), w.pad_warn.text())

    def test_language_rebuild_keeps_settings(self):
        self.launcher("rebuild", {"scale": 3})
        i18n.set_language("es", save=False)
        self.w.rebuild()
        self.assertEqual(self.w.nav["play"].text().split()[-1], "Jugar")
        self.assertEqual(self.args()[self.args().index("--scale") + 1], "3")
        i18n.set_language("en", save=False)

    def test_save_editor_window(self):
        try:
            import tombi_save_editor as SE
            import save_import as SI
        except ImportError as e:
            self.skipTest(str(e))
        d = self.launcher("editor", {})
        SE.CFG = os.path.join(d, "editor.json")
        open(os.path.join(d, "tombi_mc0.mcd"), "wb").write(bytes(SI.blank_card()))
        self.w.s["savedir"] = d
        self.w.edit_save()
        self.app.processEvents()
        self.assertTrue(self.w.editor_open())
        self.assertEqual(self.app.styleSheet(), "")          # each window keeps its own look
        self.assertIn("#sidebar", self.w.styleSheet())
        self.w.editor.close()
        self.app.processEvents()
        self.assertFalse(self.w.editor_open())

    def test_import_save_same_name(self):
        """Import save... of a save already on the card: the Replace / another slot / Cancel question, then each answer"""
        import save_import as SI
        import test_save_import as TS
        d = self.launcher("saveimp", {})
        self.w.s["savedir"] = d
        card = os.path.join(d, "tombi_mc0.mcd")
        a, b = os.path.join(d, "a.mcs"), os.path.join(d, "b.mcs")
        open(a, "wb").write(TS.fake_save(slot=0, fill=1))
        open(b, "wb").write(TS.fake_save(slot=0, fill=2))
        shown, asked, answer = [], [], [None]
        self.w.ask = lambda t: True
        self.w.info = self.w.warning = lambda t: shown.append(t)
        self.w.error = lambda t: self.fail(t)
        self.w.ask_save_conflict = lambda c: asked.append(c) or answer[0]
        self.w._open_file = lambda *x: a
        self.w.import_save()
        self.assertEqual(asked, [])
        self.w._open_file = lambda *x: b
        self.w.import_save()                                   # Cancel: card unchanged
        self.assertEqual(asked[-1].new_slot, "01")
        self.assertEqual(TS.save_bytes(open(card, "rb").read(), "BESCES-01331TOMBA-00"), TS.fake_save(slot=0, fill=1)[SI.FRAME:])
        answer[0] = "replace"
        self.w.import_save()
        self.assertEqual(TS.save_bytes(open(card, "rb").read(), "BESCES-01331TOMBA-00"), TS.fake_save(slot=0, fill=2)[SI.FRAME:])
        self.assertTrue(os.listdir(os.path.join(d, "backup")))
        answer[0] = "slot"
        self.w.import_save()
        c = open(card, "rb").read()
        self.assertEqual(TS.save_bytes(c, "BESCES-01331TOMBA-00"), TS.fake_save(slot=0, fill=2)[SI.FRAME:])
        self.assertEqual(TS.save_bytes(c, "BESCES-01331TOMBA-01")[0x200:], TS.fake_save(slot=0, fill=2)[SI.FRAME + 0x200:])
        self.assertFalse(os.path.exists(os.path.join(d, "tombi_mc1.mcd")))

    # ---- the game library (tools/game_library.py)
    def fake_data(self, d, name, sha, boot=None, serial=None):
        """A complete one-file extraction in d/name whose manifest says it is the game with this sha1."""
        dd = os.path.join(d, name)
        os.makedirs(os.path.join(dd, "EXE"), exist_ok=True)
        open(os.path.join(dd, "EXE", "MAIN0.EXE"), "wb").write(b"x" * 10)
        lines = ["tombi-data 1", "main0_sha1 %s" % (sha if boot is None else "")]
        if boot:
            lines += ["serial %s" % serial, "boot %s" % boot, "boot_sha1 %s" % sha]
        open(os.path.join(dd, "manifest.txt"), "w").write("\n".join(lines + ["file 0 0 0 10 EXE/MAIN0.EXE", ""]))
        return dd

    def library(self, name, game, extra=None):
        """A launcher whose library holds PAL (data/), USA (data-scus94236/) and JP (no data), game selected."""
        d = os.path.join(self.tmp, name)
        os.makedirs(d)
        self.fake_data(d, "data", L.MAIN0_SHA1)
        self.fake_data(d, "data-scus94236", L.GL.USA_SHA1, "SCUS_942.36", "SCUS-94236")
        games = [dict(serial="SCES-01331", sha1=L.MAIN0_SHA1, image="", data="data"),
                 dict(serial="SCUS-94236", sha1=L.GL.USA_SHA1, image="", data="data-scus94236"),
                 dict(serial="SLPS-01144", sha1=L.GL.JP_SHA1, image="", data="data-slps01144")]
        cfg = dict({"games": games, "game": game, "lang": 5, "patch_on": True, "cut_content": True, "hz60": False,
                    "areasel": True, "debugmenu": True, "prompts_style": "xbox", "titledebug": True, "skipintro": True,
                    "autostart": True, "subscreen_labels": "item=Cosas", "hz60_default": True}, **(extra or {}))
        with open(os.path.join(d, "tombi-port.json"), "w") as f:
            json.dump(cfg, f)
        L.app_dir = lambda: d
        i18n.set_language("en", save=False)
        self.w = L.App()
        self.w.show()
        self.app.processEvents()
        return d

    def test_library_migrates_an_old_setup(self):
        """An older launcher's data/ (PAL) and disc path become the library's first game, data/ kept as it is."""
        d = self.launcher("migrate", {"disc": "{APP}/tombi.bin", "lang": 5}, data=True)
        games = [dict(g, image=os.path.normpath(g["image"])) for g in self.w.s["games"]]   # "{APP}/x" keeps a '/' on Windows
        self.assertEqual(games, [dict(serial="SCES-01331", sha1=L.MAIN0_SHA1, image=os.path.normpath(os.path.join(d, "tombi.bin")),
                                      data="data")])
        self.assertEqual(self.w.s["game"], "SCES-01331")
        self.assertEqual(L.data_dir(), os.path.join(d, "data"))
        self.assertEqual(json.load(open(os.path.join(d, "tombi-port.json")))["games"][0]["data"], "data")
        self.assertEqual(self.args()[:2], ["--data", os.path.join(d, "data")])
        self.assertEqual(len(self.w.tiles), 1)

    def test_usa_game_gets_its_own_options(self):
        """USA: its own data folder and binary, no --lang / --hz / text patch / cut content / title DEBUG / prompts,
        no language box, no Cut content page; the portable options stay, the area select and debug menu included (their
        pokes name the field: GAME, IS_DEBUG_MODE_ENABLED, resolved by the USA build)."""
        d = self.library("usa", "SCUS-94236")
        a = self.args()
        self.assertEqual(a[:2], ["--data", os.path.join(d, "data-scus94236")])
        for flag in ("--lang", "--hz", "--cut-content", "--title-debug", "--prompts",
                     "--subscreen-labels", "--patch-tex", "--civ-maps"):
            self.assertNotIn(flag, a)
        self.assertIn("--skip-intro", a)
        self.assertIn("--debug-menu", a)
        self.assertEqual(a[a.index("--poke") + 1], "900:IS_DEBUG_MODE_ENABLED:1")
        self.assertFalse([x for x in a if x[:1].isdigit() and ":800" in x])   # no raw PAL address
        self.assertEqual(a[a.index("--pad-script") + 1], "1000:start,1300:circle:3")      # the area select
        # its own memory card folder (saves_usa/, as files/saves_usa on Android), for the game and the save tools
        self.assertEqual(a[a.index("--save-dir") + 1], os.path.join(d, "saves_usa"))
        self.assertTrue(os.path.isdir(os.path.join(d, "saves_usa")))
        self.assertEqual(self.w.card_file(), os.path.join(d, "saves_usa", "tombi_mc0.mcd"))
        exe = self.w.command()[0].replace("\\", "/")
        self.assertTrue(exe.endswith(("port2-scus94236/tombi2", "port2-scus94236-win/tombi2.exe", "tombi-pc-usa", "tombi-pc-usa.exe")), exe)
        self.assertFalse(hasattr(self.w, "lang_cb"))
        self.assertTrue(self.w.nav["cut"].isHidden())
        self.assertEqual([t.d["selected"] for t in self.w.tiles], [False, True, False])
        self.assertIn("60 Hz", self.w.summary.text())
        # back to PAL: everything is there again, the settings untouched
        self.w.select_game("SCES-01331")
        a = self.args()
        self.assertEqual(a[:2], ["--data", os.path.join(d, "data")])
        for flag in ("--lang", "--cut-content", "--poke", "--debug-menu", "--title-debug", "--prompts", "--subscreen-labels"):
            self.assertIn(flag, a)
        self.assertEqual(a[a.index("--hz") + 1], "50")
        self.assertNotIn("--save-dir", a)                                 # PAL: the port's default card, as before
        self.assertEqual(self.w.card_file(), os.path.join(d, "tombi_mc0.mcd"))
        self.assertFalse(self.w.nav["cut"].isHidden())
        self.assertEqual(json.load(open(os.path.join(d, "tombi-port.json")))["game"], "SCES-01331")

    def test_unsupported_game(self):
        """JP: recognised (a card with its name), but Play is off and says why."""
        self.library("jp", "SLPS-01144")
        self.assertFalse(self.w.play_btn.isEnabled())
        self.assertIn("Ore! Tomba", self.w.status_msg)
        shown = []
        self.w.error = shown.append
        self.w.play()
        self.assertTrue(shown and "does not run it yet" in shown[0])
        self.assertIsNone(self.w.proc)

    def test_add_disc_identified_and_extracted(self):
        """Add game...: a disc image is identified by its SYSTEM.CNF boot exe (sha1), gets data-<serial>/, is extracted
        and selected; another release (Tomba! 2) is refused."""
        import unittest.mock
        exe = b"PS-X EXE" + bytes(2040)
        sha = __import__("hashlib").sha1(exe).hexdigest()
        img = os.path.join(self.tmp, "usa.iso")
        open(img, "wb").write(make_iso({"SYSTEM.CNF": b"BOOT = cdrom:\\SCUS_942.36;1\r\nTCB = 4\r\n", "SCUS_942.36": exe,
                                        "SYSTEM/X00.BIN": b"overlay"}))
        with unittest.mock.patch.dict(L.GL.VERSIONS, {sha: L.GL.VERSIONS[L.GL.USA_SHA1]}):
            d = self.launcher("add", {}, data=True)            # (an older PAL data/ is there)
            self.assertEqual([e["serial"] for e in self.w.entries()], ["SCES-01331"])
            self.w.add_image(img, wait=True)
            self.assertEqual([e["serial"] for e in self.w.entries()], ["SCES-01331", "SCUS-94236"])
            e = self.w.cur_entry()
            self.assertEqual((e["serial"], e["sha1"], e["data"], e["image"]), ("SCUS-94236", sha, "data-scus94236", img))
            for _ in range(200):                              # check_disc, then the extraction thread
                self.app.processEvents()
                if L.GL.data_complete(os.path.join(d, "data-scus94236")) == sha and not self.w.extracting:
                    break
                __import__("time").sleep(0.02)
            self.assertEqual(L.GL.data_complete(os.path.join(d, "data-scus94236")), sha)
            self.assertEqual(open(os.path.join(d, "data-scus94236", "SYSTEM", "X00.BIN"), "rb").read(), b"overlay")
            self.assertEqual(self.args()[:2], ["--data", os.path.join(d, "data-scus94236")])
            self.assertEqual(self.w.game_status(e)[1], "ok")
        # a disc of another game: not added
        other = os.path.join(self.tmp, "t2.iso")
        open(other, "wb").write(make_iso({"SYSTEM.CNF": b"BOOT = cdrom:\\SCUS_944.54;1\r\n", "SCUS_944.54": b"PS-X EXE2"}))
        shown = []
        self.w.error = shown.append
        self.w.add_image(other, wait=True)
        self.assertEqual(len(self.w.entries()), 2)
        self.assertIn("Tomba! 2 (USA)", shown[0])

    def test_add_disc_from_archives(self):
        """Add game... with a .zip / .7z: the image inside (in a folder; the .cue's data track among several .bin) is
        unpacked next to the launcher, identified, extracted, and the copy deleted; a zip of the loose files, a .rar, a
        file that is no disc image, an archive without an image get their own message (with the file's name)."""
        import unittest.mock, zipfile
        exe = b"PS-X EXE" + bytes(2040)
        sha = __import__("hashlib").sha1(exe).hexdigest()
        files = {"SYSTEM.CNF": b"BOOT = cdrom:\\SCUS_942.36;1\r\nTCB = 4\r\n", "SCUS_942.36": exe, "SYSTEM/X00.BIN": b"overlay"}
        iso = make_iso(files).ljust(5 << 20, b"\0")             # (an image is at least 4 MB)
        other = bytes(6 << 20)                                   # a larger second "track" that is not the data track
        z1 = os.path.join(self.tmp, "Tomba (folder).zip")
        with zipfile.ZipFile(z1, "w", zipfile.ZIP_DEFLATED) as z:
            z.writestr("Tomba! (USA)/Tomba! (USA) (Track 1).bin", iso)
            z.writestr("Tomba! (USA)/Tomba! (USA) (Track 2).bin", other)
            z.writestr("Tomba! (USA)/Tomba! (USA).cue", 'FILE "Tomba! (USA) (Track 1).bin" BINARY\r\n  TRACK 01 MODE2/2352\r\n')
        z2 = os.path.join(self.tmp, "Tomba.7z")
        open(z2, "wb").write(make_7z({"Tomba/Tomba.cue": b'FILE "TOMBA.BIN" BINARY\n', "Tomba/TOMBA.BIN": iso, "Tomba/readme.txt": b"hi"}))
        loose = os.path.join(self.tmp, "loose files.zip")
        with zipfile.ZipFile(loose, "w") as z:
            for k, v in files.items():
                z.writestr("disc/" + k, v)
        with unittest.mock.patch.dict(L.GL.VERSIONS, {sha: L.GL.VERSIONS[L.GL.USA_SHA1]}):
            for arc in (z1, z2):
                with self.subTest(os.path.basename(arc)):
                    d = self.launcher("arc" + os.path.splitext(arc)[1][1:], {}, data=True)
                    shown = []
                    self.w.error = shown.append
                    self.w.add_image(arc, wait=True)
                    self.assertEqual(shown, [])
                    e = self.w.cur_entry()
                    self.assertEqual((e["serial"], e["sha1"], e["data"], e["image"]), ("SCUS-94236", sha, "data-scus94236", arc))
                    for _ in range(300):
                        self.app.processEvents()
                        if L.GL.data_complete(os.path.join(d, "data-scus94236")) == sha and not self.w.extracting:
                            break
                        __import__("time").sleep(0.02)
                    self.assertEqual(L.GL.data_complete(os.path.join(d, "data-scus94236")), sha)
                    self.assertEqual(open(os.path.join(d, "data-scus94236", "SYSTEM", "X00.BIN"), "rb").read(), b"overlay")
                    self.assertFalse(os.path.exists(os.path.join(d, ".unpack")))      # the unpacked copy is gone
                    self.assertEqual(self.w.game_status(e)[1], "ok")
            # what cannot be added: each says why, with the file's name, and nothing is added
            rar = os.path.join(self.tmp, "Tomba.rar")
            open(rar, "wb").write(b"Rar!\x1a\x07\x00" + bytes(100))
            junk = os.path.join(self.tmp, "notes.bin")
            open(junk, "wb").write(os.urandom(1 << 16))
            empty = os.path.join(self.tmp, "readme.zip")
            with zipfile.ZipFile(empty, "w") as z:
                z.writestr("readme.txt", "hello")
            for f, want in ((loose, "loose files (Tomba! (USA))"), (rar, ".rar archive"), (junk, "not a PlayStation disc image"),
                            (empty, "holds no disc image")):
                with self.subTest(os.path.basename(f)):
                    n = len(self.w.entries())
                    shown = []
                    self.w.error = shown.append
                    self.w.add_image(f, wait=True)
                    self.assertEqual(len(self.w.entries()), n)
                    self.assertTrue(shown and os.path.basename(f) in shown[0] and want in shown[0], shown)

    def test_remove_game(self):
        d = self.library("rm", "SCUS-94236")
        answers = [True, True]
        self.w.ask = lambda t: answers.pop(0)
        self.w.remove_game()
        self.assertEqual([e["serial"] for e in self.w.entries()], ["SCES-01331", "SLPS-01144"])
        self.assertFalse(os.path.exists(os.path.join(d, "data-scus94236")))
        self.assertTrue(os.path.isdir(os.path.join(d, "data")))
        self.assertEqual(self.w.s["game"], "SCES-01331")

    def test_library_views(self):
        """The three looks build (grid / list / banner) with a card per game and Add game."""
        for view, n in (("grid", 3), ("list", 3), ("banner", 3)):
            with self.subTest(view):
                self.library("view_" + view, "SCES-01331", {"library_view": view})
                self.assertEqual(len(self.w.tiles), n)
                self.w.close()
                self.w.deleteLater()
                self.w = None


def make_7z(files):
    """A minimal .7z (one solid LZMA2 folder, names in UTF-16) holding files {"DIR/NAME": bytes}, as 7-Zip writes it."""
    import lzma, struct, zlib

    def num(v):                                   # 7z NUMBER: the 9-byte form is enough here
        return b"\xff" + struct.pack("<Q", v)
    names = sorted(files)
    data = b"".join(files[n] for n in names)
    packed = lzma.compress(data, format=lzma.FORMAT_RAW, filters=[dict(id=lzma.FILTER_LZMA2, dict_size=1 << 24)])
    h = bytearray(b"\x01\x04")                                       # Header, MainStreamsInfo
    h += b"\x06" + num(0) + num(1) + b"\x09" + num(len(packed)) + b"\x00"            # PackInfo
    h += b"\x07\x0b" + num(1) + b"\x00" + num(1) + b"\x21\x21\x01\x14"               # one folder: LZMA2, dict 2^24 (props 20)
    h += b"\x0c" + num(len(data)) + b"\x00"
    h += b"\x08\x0d" + num(len(names)) + b"\x09" + b"".join(num(len(files[n])) for n in names[:-1]) + b"\x00"   # SubStreams
    h += b"\x00"
    nm = b"\x00" + b"".join(n.encode("utf-16-le") + b"\0\0" for n in names)
    h += b"\x05" + num(len(names)) + b"\x11" + num(len(nm)) + nm + b"\x00"           # FilesInfo: names
    h += b"\x00"
    start = struct.pack("<QQI", len(packed), len(h), zlib.crc32(h))
    return b"7z\xbc\xaf\x27\x1c\x00\x04" + struct.pack("<I", zlib.crc32(start)) + start + packed + bytes(h)


def make_iso(files):
    """A minimal cooked ISO9660 image (2048-byte sectors) holding files {"DIR/NAME": bytes}, one directory level."""
    S = 2048
    dirs = sorted({p.split("/")[0] for p in files if "/" in p})
    root_files = sorted(p for p in files if "/" not in p)
    lba = 18 + 1 + len(dirs)
    place = {}
    for p in sorted(files):
        place[p] = lba
        lba += max(1, (len(files[p]) + S - 1) // S)
    total = lba

    def rec(name, at, size, isdir):
        n = name.encode()
        r = bytearray(33 + len(n) + (1 - len(n) % 2))
        r[0] = len(r)
        r[2:10] = at.to_bytes(4, "little") + at.to_bytes(4, "big")
        r[10:18] = size.to_bytes(4, "little") + size.to_bytes(4, "big")
        r[25] = 2 if isdir else 0
        r[28:32] = (1).to_bytes(2, "little") + (1).to_bytes(2, "big")
        r[32] = len(n)
        r[33:33 + len(n)] = n
        return bytes(r)

    def dir_sector(me, parent, entries):
        b = rec("\0", me, S, True) + rec("\1", parent, S, True) + b"".join(entries)
        return b.ljust(S, b"\0")
    img = bytearray(total * S)
    root = 18
    ents = [rec(d, 19 + i, S, True) for i, d in enumerate(dirs)]
    ents += [rec(p + ";1", place[p], len(files[p]), False) for p in root_files]
    img[root * S:(root + 1) * S] = dir_sector(root, root, ents)
    for i, d in enumerate(dirs):
        sub = sorted(p for p in files if p.startswith(d + "/"))
        img[(19 + i) * S:(20 + i) * S] = dir_sector(19 + i, root, [rec(p.split("/")[1] + ";1", place[p], len(files[p]), False) for p in sub])
    pvd = bytearray(S)
    pvd[0:6] = b"\x01CD001"
    pvd[80:88] = total.to_bytes(4, "little") + total.to_bytes(4, "big")
    pvd[156:190] = rec("\0", root, S, True)
    img[16 * S:17 * S] = pvd
    img[17 * S:17 * S + 6] = b"\xffCD001"
    for p, data in files.items():
        img[place[p] * S:place[p] * S + len(data)] = data
    return bytes(img)


class TestGameLibrary(unittest.TestCase):
    """tools/game_library.py without Qt: identification, data folder names, the art from the overlay's sprite list."""

    def test_describe(self):
        import game_library as GL
        pal = GL.describe("SCES_013.31", "1" * 40, GL.PAL_SHA1)
        self.assertEqual((pal["serial"], pal["version"]["key"], pal["game_sha1"]), ("SCES-01331", "pal", GL.PAL_SHA1))
        self.assertEqual(GL.describe("SCES_013.30", "2" * 40, GL.PAL_SHA1)["serial"], "SCES-01330")
        usa = GL.describe("SCUS_942.36", GL.USA_SHA1)
        self.assertEqual((usa["serial"], usa["version"]["binary"], usa["name"]), ("SCUS-94236", "tombi-pc-usa", "Tomba! (USA)"))
        jp = GL.describe("SLPS_011.44", GL.JP_SHA1)
        self.assertFalse(jp["version"]["supported"])
        self.assertIsNone(GL.describe("SCUS_944.54", "3" * 40)["version"])
        self.assertTrue(GL.has(GL.PAL_SHA1, "lang") and not GL.has(GL.USA_SHA1, "lang"))
        self.assertEqual(GL.text_patches(GL.PAL_SHA1), ("es_mejorada.tombipatch.json",))
        self.assertEqual(GL.text_patches(GL.USA_SHA1), ())          # the hook for a USA retranslation: none yet
        self.assertEqual(GL.binary_name(GL.USA_SHA1, windows=True), "tombi-pc-usa.exe")

    def test_data_names(self):
        import game_library as GL
        es = []
        for serial, sha in (("SCUS-94236", GL.USA_SHA1), ("SCES-01331", GL.PAL_SHA1), ("SCES-01330", GL.PAL_SHA1)):
            GL.add_entry(es, dict(serial=serial, game_sha1=sha), "/x/" + serial)
        self.assertEqual([e["data"] for e in es], ["data-scus94236", "data", "data-sces01330"])
        GL.add_entry(es, dict(serial="SCES-01331", game_sha1=GL.PAL_SHA1), "/y/again.cue")    # the same disc again
        self.assertEqual(len(es), 3)
        self.assertEqual(es[1]["image"], "/y/again.cue")

    def test_logo_from_the_sprite_list(self):
        """The logo is drawn as the title overlay draws it: its sprite records (CLUT 128,510) on the logo page."""
        import struct
        import game_library as GL

        class Src:
            def __init__(self, files):
                self.files = files

            def read(self, n):
                return self.files[n]
        page = bytearray(256 * 256)
        for y in range(10, 20):
            page[y * 256 + 5:y * 256 + 15] = b"\x01" * 10             # piece A: 10x10 at 5,10
        for y in range(120, 125):
            page[y * 256 + 0:y * 256 + 4] = b"\x02" * 4               # piece B: 4x5 at 0,120
        clut = struct.pack("<256H", *([0, 0x001F, 0x7C00] + [0] * 253))
        ovl = bytes(6) + GL.SPR.pack(16, 24, 5, 10, 10, 10, 0x84, 128, 510) + GL.SPR.pack(40, 24, 0, 120, 4, 5, 0x84, 128, 510) \
            + struct.pack("<h", -1) + bytes(20)
        land = b"GAM\0" + struct.pack("<I", 512 * 256) + (b"\0\0" + b"\x01" * 16) * (512 * 256 // 16)
        src = Src({"SYSTEM/A00012.000": bytes(page), "SYSTEM/A00013.000": clut, "SYSTEM/OPTSUB00.BIN": ovl,
                   "SYSTEM/A00010.GAM": land, "SYSTEM/A00011.000": clut})
        w, h, rgba = GL.logo_rgba(src, GL.USA_SHA1)
        self.assertEqual((w, h), (28, 10))                           # x 16..43, y 24..33
        self.assertEqual(rgba[0:4], bytes((255, 0, 0, 255)))         # colour 1 = red (0x001F)
        self.assertEqual(rgba[(0 * w + 24) * 4:(0 * w + 24) * 4 + 4], bytes((0, 0, 255, 255)))   # piece B at x 40
        self.assertEqual(rgba[(0 * w + 12) * 4 + 3], 0)              # between the pieces: transparent
        w, h, rgba = GL.title_rgba(src, GL.USA_SHA1)
        self.assertEqual((w, h), (320, 240))
        self.assertEqual(rgba[(24 * 320 + 16) * 4:(24 * 320 + 16) * 4 + 4], bytes((255, 0, 0, 255)))
        self.assertEqual(rgba[0:4], bytes((255, 0, 0, 255)))         # the landscape: colour 1 everywhere

if __name__ == "__main__":
    unittest.main()

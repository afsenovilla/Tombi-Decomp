package io.github.tombi.port;

import android.app.Application;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.view.View;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.Robolectric;
import org.robolectric.RobolectricTestRunner;
import org.robolectric.RuntimeEnvironment;
import org.robolectric.annotation.Config;
import org.robolectric.annotation.GraphicsMode;
import org.robolectric.android.controller.ActivityController;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

/** Renders the launcher's screens (the real LauncherActivity code, Robolectric native graphics = Android's Skia) to
 *  PNGs at the AYN Thor's top screen size, and checks the library's migration, key / touch navigation and the
 *  arguments per version (printed).  port2/android/render/render.py runs it: -Dout=DIR, -Dpal=FS -Dusa=FS (folders
 *  with SYSTEM/ of the player's own discs: the logos are cut from them as on the device; without them the cards show
 *  the names), -Djp=IMAGE (the Japanese disc image: its logo read from it, Library.logoFromImage), -Duilang=N (LauncherActivity.UI_LANGS index, 2 = es). */
@RunWith(RobolectricTestRunner.class)
@Config(sdk = 34, qualifiers = "w1920dp-h1080dp-land-mdpi")
@GraphicsMode(GraphicsMode.Mode.NATIVE)
public class RenderTest {
    static final int W = 1920, H = 1080;

    static void copy(File a, File b) throws Exception {
        b.getParentFile().mkdirs();
        try (InputStream in = new FileInputStream(a); OutputStream o = new FileOutputStream(b)) {
            byte[] buf = new byte[1 << 16]; int n;
            while ((n = in.read(buf)) > 0) o.write(buf, 0, n);
        }
    }
    /** an extracted game: its manifest and, from fs, the files of its logo (Library.Version logoPage / logoClut / logoOvl) */
    static void game(Application app, String id, String fs, String ver) throws Exception {
        File d = new File(new File(new File(app.getFilesDir(), "games"), id), "data");
        d.mkdirs();
        try (OutputStream o = new FileOutputStream(new File(d, "manifest.txt"))) { o.write("tombi-data 1\n".getBytes()); }
        Library.Version v = Library.byKey(ver);
        if (fs != null) for (String f : new String[]{v.logoPage, v.logoClut, v.logoOvl}) copy(new File(fs, f), new File(d, f));
    }
    static void shot(LauncherActivity a, String name) throws Exception {
        Bitmap b = draw(a);
        File out = new File(System.getProperty("out"), name + ".png");
        try (OutputStream o = new FileOutputStream(out)) { b.compress(Bitmap.CompressFormat.PNG, 100, o); }
        System.out.println("wrote " + out);
    }
    /** one frame of the launcher (and its dialog, when one is up) at W x H */
    static Bitmap draw(LauncherActivity a) {
        View v = a.view;
        v.measure(View.MeasureSpec.makeMeasureSpec(W, View.MeasureSpec.EXACTLY), View.MeasureSpec.makeMeasureSpec(H, View.MeasureSpec.EXACTLY));
        v.layout(0, 0, W, H);
        Bitmap b = Bitmap.createBitmap(W, H, Bitmap.Config.ARGB_8888);
        Canvas c = new Canvas(b);
        v.draw(c);
        v.draw(c);    // twice: the spatial navigation's rectangles come from the previous frame
        if (a.dlg != null && a.dlg.showing()) {
            a.dlg.measure(View.MeasureSpec.makeMeasureSpec(W, View.MeasureSpec.EXACTLY), View.MeasureSpec.makeMeasureSpec(H, View.MeasureSpec.EXACTLY));
            a.dlg.layout(0, 0, W, H);
            a.dlg.draw(c);
        }
        return b;
    }
    static int indexOfPlay(LauncherActivity a) {
        a.view.build();
        for (int i = 0; i < a.view.items.size(); i++) if (a.view.items.get(i).primary) return i;
        return 0;
    }

    /** an install from before the library (files/data of the PAL disc) becomes the PAL entry; the keys move between
     *  the cards and the selection follows; the arguments name the version's library */
    @Test
    public void migrateAndKeys() throws Exception {
        Application app = RuntimeEnvironment.getApplication();
        File old = new File(app.getFilesDir(), "data");
        old.mkdirs();
        try (OutputStream o = new FileOutputStream(new File(old, "manifest.txt"))) { o.write("tombi-data 1\nsectors 1\nmain0_sha1 bec8\n".getBytes()); }
        try (OutputStream o = new FileOutputStream(new File(old, "SYSTEM.CNF"))) { o.write("BOOT = cdrom:\\SCES_013.31;1\r\nTCB = 4\r\n".getBytes()); }
        String pal = System.getProperty("pal");
        if (pal != null) for (String f : new String[]{"SYSTEM/A00202.000", "SYSTEM/A00203.000", "SYSTEM/O0PTSUB0.BIN"})
            copy(new File(pal, f), new File(old, f));
        SharedPreferences prefs = app.getSharedPreferences("launcher", 0);
        prefs.edit().putString("disc_name", "Tombi.bin").putString("disc_info", "old info").putBoolean("disc_ok", true).commit();
        ActivityController<LauncherActivity> c = Robolectric.buildActivity(LauncherActivity.class).setup();
        LauncherActivity a = c.get();
        System.out.println("migrate: games=" + prefs.getString("games", "") + " game=" + prefs.getString("game", "")
            + " old data gone=" + !old.exists() + " new manifest=" + new File(app.getFilesDir(), "games/sces01331/data/manifest.txt").isFile()
            + " name=" + prefs.getString("g.sces01331.name", "") + " disc_name left=" + prefs.contains("disc_name"));
        System.out.println("args PAL: " + java.util.Arrays.toString(a.buildArgs()));
        // a USA entry too
        game(app, "scus94236", System.getProperty("usa"), "usa");
        prefs.edit().putString("games", "sces01331,scus94236").putString("g.scus94236.ver", "usa").putString("g.scus94236.serial", "SCUS-94236").commit();
        a.view.setScreen(LauncherActivity.LauncherView.MAIN);
        draw(a);
        System.out.println("focus " + a.view.focus + " (" + a.view.items.get(a.view.focus).label + ")");
        int[] keys = {android.view.KeyEvent.KEYCODE_DPAD_UP, android.view.KeyEvent.KEYCODE_DPAD_RIGHT, android.view.KeyEvent.KEYCODE_DPAD_RIGHT,
                      android.view.KeyEvent.KEYCODE_DPAD_LEFT, android.view.KeyEvent.KEYCODE_DPAD_DOWN, android.view.KeyEvent.KEYCODE_DPAD_RIGHT};
        for (int k : keys) {
            a.view.key(k);
            draw(a);
            System.out.println("key " + android.view.KeyEvent.keyCodeToString(k) + " -> focus " + a.view.focus + " (" + a.view.items.get(a.view.focus).label
                + "), selected " + a.game().id);
        }
        // touch: a tap on the other card selects it only; a second tap starts the game with its library
        prefs.edit().putString("game", "sces01331").commit();
        a.view.setScreen(LauncherActivity.LauncherView.MAIN);
        draw(a);
        android.graphics.RectF r = a.view.items.get(a.view.railCount() + 1).r;   // the USA card (after the rail's icons)
        float sc = a.view.s;
        for (int t = 0; t < 2; t++) {
            float x = a.view.ox + r.centerX() * sc, y = a.view.oy + r.centerY() * sc;
            a.view.onTouchEvent(android.view.MotionEvent.obtain(0, 0, android.view.MotionEvent.ACTION_DOWN, x, y, 0));
            a.view.onTouchEvent(android.view.MotionEvent.obtain(0, 10, android.view.MotionEvent.ACTION_UP, x, y, 0));
            draw(a);
            android.content.Intent in = org.robolectric.Shadows.shadowOf(app).getNextStartedActivity();
            System.out.println("tap " + t + ": selected " + a.game().id + ", started " + (in == null ? "nothing" : java.util.Arrays.toString(in.getStringArrayExtra(TombiActivity.EXTRA_ARGS))));
        }
        prefs.edit().putString("game", "scus94236").commit();
        System.out.println("args USA: " + java.util.Arrays.toString(a.buildArgs()));
        System.out.println("logo PAL " + (Library.logo(a.lib.get("sces01331")) != null) + ", USA " + (Library.logo(a.lib.get("scus94236")) != null));
        a.removeGame();
        System.out.println("after remove: games=" + prefs.getString("games", "") + " dir gone=" + !new File(app.getFilesDir(), "games/scus94236").exists() + " status=" + a.importStatus);
        c.pause().stop().destroy();
    }

    /** "Add game" with real picked files (-Ddiscs=DIR: each file of DIR in name order, as a file:// document; the
     *  disc check is the real disccheck.c when render.py could build it for the PC, see --native): the result, the
     *  library and the Disc screen after each.  Without -Ddiscs only the add screens (picking, unzipping) are drawn. */
    @Test
    public void addFlow() throws Exception {
        Application app = RuntimeEnvironment.getApplication();
        SharedPreferences prefs = app.getSharedPreferences("launcher", 0);
        prefs.edit().putInt("uilang", Integer.parseInt(System.getProperty("uilang", "2"))).commit();
        ActivityController<LauncherActivity> c = Robolectric.buildActivity(LauncherActivity.class).setup();
        LauncherActivity a = c.get();
        System.out.println("native disc check: " + LauncherActivity.nativeOk);
        a.view.setScreen(LauncherActivity.LauncherView.MAIN);
        a.view.setScreen(LauncherActivity.LauncherView.DISC); a.addFromLib = true; a.pickDisc();
        shot(a, "add_0_picking");
        a.addName = "Tomba! (USA).zip"; a.phase = LauncherActivity.UNZIPPING; a.copied = 120L << 20; a.copyTotal = 190L << 20;
        shot(a, "add_0_unzipping");
        a.phase = LauncherActivity.IDLE;
        String dir = System.getProperty("discs");
        File[] picks = dir == null ? new File[0] : new File(dir).listFiles();
        if (picks == null) picks = new File[0];
        java.util.Arrays.sort(picks);
        for (File f : picks) {
            a.view.setScreen(LauncherActivity.LauncherView.MAIN);
            a.view.setScreen(LauncherActivity.LauncherView.DISC); a.addFromLib = true; a.pickDisc();
            long t0 = System.currentTimeMillis();
            a.importDisc(java.util.Collections.singletonList(android.net.Uri.fromFile(f)));
            Library.Game g = a.game();
            String man = g == null ? null : Library.readSmall(new File(g.data(), "manifest.txt"));
            String mserial = null, msrc = null;
            if (man != null) for (String l : man.split("\n")) { if (l.startsWith("serial ")) mserial = l.substring(7); if (l.startsWith("source ")) msrc = l.substring(7); }
            String[] left = new File(a.getCacheDir(), "import").list();
            System.out.println("ADD " + f.getName() + " (" + (System.currentTimeMillis() - t0) / 1000 + " s): result " + a.result + ", adding " + a.adding
                + ", status: " + a.importStatus + "\n    games " + prefs.getString("games", "") + ", selected " + (g == null ? "-" : g.id + " name=" + g.name)
                + ", manifest serial " + mserial + " source " + msrc + ", logo.png " + (g != null && g.logoFile().isFile()) + ", cache left " + (left == null ? "none" : java.util.Arrays.toString(left)));
            a.view.focus = 0;
            shot(a, "add_" + f.getName().replaceAll("[^A-Za-z0-9]+", "_"));
        }
        if (picks.length > 0) {
            a.view.setScreen(LauncherActivity.LauncherView.MAIN);
            a.view.focus = indexOfPlay(a);
            shot(a, "add_lib_after");
        }
        c.pause().stop().destroy();
    }

    @Test
    public void render() throws Exception {
        Application app = RuntimeEnvironment.getApplication();
        String pal = System.getProperty("pal"), usa = System.getProperty("usa");
        SharedPreferences prefs = app.getSharedPreferences("launcher", 0);
        String lang = System.getProperty("uilang", "2");   // 2 = es (LauncherActivity.UI_LANGS)
        prefs.edit().putInt("uilang", Integer.parseInt(lang)).putInt("lang", 1).commit();

        // 1. the empty library (first start)
        ActivityController<LauncherActivity> c0 = Robolectric.buildActivity(LauncherActivity.class).setup();
        LauncherActivity a0 = c0.get();
        shot(a0, "lib_empty");
        c0.pause().stop().destroy();

        // 2. a library of three discs: PAL (ready), USA (ready), Japan (recognised, not supported)
        game(app, "sces01331", pal, "pal");
        game(app, "scus94236", usa, "usa");
        prefs.edit().putString("games", "sces01331,scus94236,slps01144").putString("game", "sces01331")
            .putString("g.sces01331.ver", "pal").putString("g.sces01331.serial", "SCES-01331").putString("g.sces01331.name", "Tombi! (Europe) (En,Es,Pt,Sv,Da).bin")
            .putString("g.sces01331.info", "1108 files, 209358 sectors (2352-byte source sectors), main0 bec8dc5f…")
            .putString("g.scus94236.ver", "usa").putString("g.scus94236.serial", "SCUS-94236").putString("g.scus94236.name", "Tomba! (USA).chd")
            .putString("g.scus94236.info", "1112 files, 119130 sectors (2352-byte source sectors, chd), SCUS-94236 SCUS_942.36 81cbc79f…")
            .putString("g.slps01144.ver", "jp").putString("g.slps01144.serial", "SLPS-01144").putString("g.slps01144.name", "Ore! Tomba.bin")
            .commit();
        // the Japanese disc is not extracted: its logo comes straight from the image (-Djp=IMAGE), as when it is added
        String jp = System.getProperty("jp");
        if (jp != null) {
            Library lb = new Library(app, prefs);
            System.out.println("logo from the JP image: " + (Library.logoFromImage(lb.get("slps01144"), jp) != null)
                + " (native " + LauncherActivity.nativeOk + ")");
        }
        ActivityController<LauncherActivity> c = Robolectric.buildActivity(LauncherActivity.class).setup();
        LauncherActivity a = c.get();
        // each card's logo.png as the launcher cached it (to compare with tools/game_library.py --art)
        for (Library.Game g : a.lib.games())
            if (Library.logo(g) != null) copy(g.logoFile(), new File(System.getProperty("out"), "logo_" + g.id + ".png"));
        prefs.edit().putString("game", "sces01331").commit();
        a.view.page = LauncherActivity.LauncherView.PG_PLAY;
        a.view.setScreen(LauncherActivity.LauncherView.MAIN);
        a.view.focus = indexOfPlay(a);
        shot(a, "lib_pal");
        prefs.edit().putString("game", "scus94236").commit();
        a.view.focus = a.view.railCount() + 1;   // the USA card focused (and selected)
        shot(a, "lib_usa_focus");
        prefs.edit().putString("game", "slps01144").commit();
        a.view.focus = indexOfPlay(a);
        shot(a, "lib_jp");
        // four discs (PAL SCES-01331 and SCES-01330, USA, Japan): their languages as flags beside the serial
        game(app, "sces01330", pal, "pal");
        prefs.edit().putString("games", "sces01331,sces01330,scus94236,slps01144").putString("g.sces01330.ver", "pal")
            .putString("g.sces01330.serial", "SCES-01330").putString("g.sces01330.name", "Tombi! (Europe) (En,Fr,De,It,Nl).bin").commit();
        prefs.edit().putString("game", "sces01331").commit();
        a.view.setScreen(LauncherActivity.LauncherView.MAIN);
        a.view.focus = indexOfPlay(a);
        shot(a, "lib_four");
        prefs.edit().putString("games", "sces01331,scus94236,slps01144").commit();
        // the pages of the USA game (no languages / translations / panel / extras) and of the PAL one
        prefs.edit().putString("game", "scus94236").commit();
        int[] pgs = {LauncherActivity.LauncherView.PG_IMAGE, LauncherActivity.LauncherView.PG_PAD, LauncherActivity.LauncherView.PG_EXTRAS,
                     LauncherActivity.LauncherView.PG_DISC, LauncherActivity.LauncherView.PG_APP};
        String[] pgNames = {"image", "pad", "extras", "disc", "app"};
        a.view.setScreen(LauncherActivity.LauncherView.MAIN); a.view.openPage(LauncherActivity.LauncherView.PG_IMAGE);
        shot(a, "page_usa_image");
        a.view.openPage(LauncherActivity.LauncherView.PG_PAD);
        shot(a, "page_usa_pad");
        prefs.edit().putString("game", "sces01331").commit();
        for (int pi = 0; pi < pgs.length; pi++) {   // every page of the PAL game
            a.view.setScreen(LauncherActivity.LauncherView.MAIN); a.view.openPage(pgs[pi]);
            shot(a, "page_pal_" + pgNames[pi]);
        }
        a.view.openPage(LauncherActivity.LauncherView.PG_PLAY);
        // the game screen (Disc) of the PAL game: the long status of a real extraction (the device report: it overlapped
        // the help paragraph)
        prefs.edit().putString("game", "sces01331").putString("g.sces01331.info",
            "1108 files, 209358 sectors (2352-byte source sectors), main0 bec8dc5f4116f5e497f2337eef8c523ff6dee8fc, SCES-01331 "
            + "EXE/MAIN0.EXE bec8dc5f4116f5e497f2337eef8c523ff6dee8fc, content sha1 33d5f861ed02909236e9887376d80deaadbd234f (= reference PAL disc)").commit();
        a.result = 0; a.importStatus = null;
        a.view.setScreen(LauncherActivity.LauncherView.DISC); a.view.focus = 0;
        shot(a, "disc_pal");
        // the game screen (Disc) of the USA game after adding it, and the Japanese one
        prefs.edit().putString("game", "scus94236").commit();
        a.view.setScreen(LauncherActivity.LauncherView.DISC); a.view.focus = 0;
        a.result = 1; a.importStatus = a.getString(R.string.disc_added, "Tomba! (EE. UU.)");
        shot(a, "disc_usa_added");
        prefs.edit().putString("game", "slps01144").commit();
        a.result = -1; a.importStatus = a.getString(R.string.lib_unsupported_long, "Ore! Tomba (Japón)");
        shot(a, "disc_jp");
        // the dialog when Play is pressed on the Japanese game
        a.view.setScreen(LauncherActivity.LauncherView.MAIN);
        a.play();
        shot(a, "lib_jp_play");
        c.pause().stop().destroy();
    }
}

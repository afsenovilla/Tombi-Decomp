package io.github.tombi.port;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.widget.OverScroller;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/**
 * The launcher shown before the game ("Selva" design, docs/PORT.md "4. Android"): main menu, options (persisted in
 * SharedPreferences, turned into port flags for TombiActivity), disc status with the disc image picker (SAF), quit.
 * Drawn on a Canvas in a 1280x720 virtual space scaled to the screen; gamepad (D-pad/A/B/Start) and touch.
 * Options > Controls / Button mapping: the 14 PS1 buttons and the panel's actions, each bound by pressing the controller
 * button / trigger / stick (PadMap; saved as "padmap", handed to TombiActivity as EXTRA_PADMAP).
 * Fonts (OFL, assets/fonts) are fetched by the CI build; without them the system bold face is used.
 */
public class LauncherActivity extends Activity {
    static final int BG = 0xff2f6b3a, CIRCLE_BL = 0xff275a31, CIRCLE_TR = 0xff3a7c45, ACCENT = 0xffff6aa8,
            ACCENT_SH = 0xff7a2e00, ACCENT_TEXT = 0xff4a1530, CREAM = 0xfffff8e6, CREAM_SH = 0xff1d4424,
            SUB = 0xffffe8a8, CHIP = 0x40000000;
    static final int[] LANG_N = {0, 5, 6, 7, 8};
    static final String[] LANG_NAME = {"English", "Español", "Português", "Svenska", "Dansk"};
    static final String[] ASPECTS = {"auto", "4:3", "16:9", "21:9"};
    static final String[] PROMPTS = {"auto", "playstation", "xbox", "nintendo", "generic", "thor"};   // button icons (--prompts); thor last: stored indices keep their meaning
    static final int REQ_DISC = 2, REQ_EXPORT = 3, REQ_SAVE_IMPORT = 4, REQ_SAVE_EXPORT = 5;
    static final String MAIN0_SHA1 = "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc";   // EXE/MAIN0.EXE of SCES-01331
    static final String TEXT_PATCH = "es_mejorada.tombipatch.json";

    SharedPreferences prefs;
    LauncherView view;
    volatile String importStatus;

    @Override
    protected void onCreate(Bundle b) {
        // a launcher crash (menus, button mapping...) into tombi.log + the "crashed" marker: the crash card next time
        final android.content.Context app = getApplicationContext();
        final Thread.UncaughtExceptionHandler prev = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override public void uncaughtException(Thread t, Throwable e) {
                CrashLog.append(app, "LAUNCHER UNCAUGHT EXCEPTION in thread \"" + t.getName() + "\" (" + version() + "): " + CrashLog.stack(e));
                CrashLog.writeMarker(app, CrashLog.CRASHED, "launcher exception: " + e + "\n");
                if (prev != null) prev.uncaughtException(t, e); else System.exit(2);
            }
        });
        super.onCreate(b);
        prefs = getSharedPreferences("launcher", MODE_PRIVATE);
        view = new LauncherView(this);
        setContentView(view);
        view.setFocusable(true);
        view.setFocusableInTouchMode(true);
        view.requestFocus();
    }

    @Override
    protected void onResume() {
        super.onResume();
        immersive();
        // the last game run ended abnormally (CrashLog markers): the crash card with the log, once
        try {
            if (CrashLog.markersStale(this)) CrashLog.clearMarkers(this);   // left by the build before an update
            else if (view.screen != LauncherView.CRASH && CrashLog.crashedLastRun(this)) view.setScreen(LauncherView.CRASH);
        } catch (Throwable ignored) { }
        view.invalidate();
    }

    // ---- the game's log (CrashLog): copy / share / export ----
    void copyReport() {
        try {
            android.content.ClipboardManager cm = (android.content.ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
            cm.setPrimaryClip(android.content.ClipData.newPlainText("Tombi! log", CrashLog.report(this, 48 * 1024)));
            Toast.makeText(this, R.string.log_copied, Toast.LENGTH_SHORT).show();
        } catch (Throwable t) { Toast.makeText(this, String.valueOf(t), Toast.LENGTH_LONG).show(); }
    }
    void shareReport() {
        try {
            Intent i = new Intent(Intent.ACTION_SEND);
            i.setType("text/plain");
            i.putExtra(Intent.EXTRA_SUBJECT, "Tombi! Android log");
            i.putExtra(Intent.EXTRA_TEXT, CrashLog.report(this, 96 * 1024));
            startActivity(Intent.createChooser(i, getString(R.string.log_share)));
        } catch (Throwable t) { Toast.makeText(this, String.valueOf(t), Toast.LENGTH_LONG).show(); }
    }
    void exportReport() {
        Intent i = new Intent(Intent.ACTION_CREATE_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType("text/plain");
        i.putExtra(Intent.EXTRA_TITLE, "tombi-log-" + new java.text.SimpleDateFormat("yyyyMMdd-HHmmss", Locale.ROOT).format(new java.util.Date()) + ".txt");
        try { startActivityForResult(i, REQ_EXPORT); }
        catch (ActivityNotFoundException e) { Toast.makeText(this, getString(R.string.log_export_fail, String.valueOf(e.getMessage())), Toast.LENGTH_LONG).show(); }
    }
    void writeExport(Uri u) {
        try {
            OutputStream o = getContentResolver().openOutputStream(u);
            if (o == null) throw new Exception("openOutputStream");
            try { o.write(CrashLog.report(this, 8 * 1024 * 1024).getBytes("UTF-8")); } finally { o.close(); }
            Toast.makeText(this, R.string.log_exported, Toast.LENGTH_SHORT).show();
        } catch (Throwable t) { Toast.makeText(this, getString(R.string.log_export_fail, String.valueOf(t.getMessage())), Toast.LENGTH_LONG).show(); }
    }

    @Override
    public void onWindowFocusChanged(boolean f) {
        super.onWindowFocusChanged(f);
        if (f) immersive();
    }

    void immersive() {
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

    // ---- settings ----
    int langIdx() {
        int def = Locale.getDefault().getLanguage().equals("es") ? 1 : 0;
        return clamp(prefs.getInt("lang", def), LANG_N.length);
    }
    int aspectIdx() { return clamp(prefs.getInt("aspect", 0), ASPECTS.length); }
    int promptIdx() { return clamp(prefs.getInt("prompts", 0), PROMPTS.length); }
    boolean flag(String k, boolean def) { return prefs.getBoolean(k, def); }
    /* second screen panel (runtime/panel.c, TombiActivity): default on when the device has a secondary display */
    boolean hasSecondDisplay() {
        try {
            android.hardware.display.DisplayManager dm = (android.hardware.display.DisplayManager) getSystemService(DISPLAY_SERVICE);
            android.view.Display[] ds = dm != null ? dm.getDisplays(android.hardware.display.DisplayManager.DISPLAY_CATEGORY_PRESENTATION) : null;
            return ds != null && ds.length > 0;
        } catch (Throwable t) { return false; }
    }
    boolean secondScreenOn() { return flag("secondscreen", hasSecondDisplay()); }
    /* the game's HUD on the panel instead of the top screen (--hud-bottom): default on with a secondary display */
    boolean hudBottomOn() { return flag("hudbottom", hasSecondDisplay()); }
    static int clamp(int v, int n) { return v < 0 || v >= n ? 0 : v; }

    File filesDir() {
        File f = getExternalFilesDir(null);
        return f != null ? f : getFilesDir();
    }

    File discFile() {
        String d = prefs.getString("disc", null);
        return d != null && new File(d).isFile() ? new File(d) : null;
    }
    boolean ready() { return dataReady() || discFile() != null; }
    boolean textPatchOn() { return flag("textpatch", LANG_N[langIdx()] == 5); }

    /** files/patches/<name>, (re)extracted from the APK's assets/patches (the files are small; copying each time keeps
     *  them in step with an updated APK); null without it. */
    File patchAsset(String name) {
        File f = new File(new File(filesDir(), "patches"), name);
        try {
            InputStream in = getAssets().open("patches/" + name);
            f.getParentFile().mkdirs();
            OutputStream out = new FileOutputStream(f);
            byte[] buf = new byte[1 << 15]; int n;
            try { while ((n = in.read(buf)) > 0) out.write(buf, 0, n); } finally { in.close(); out.close(); }
            return f;
        } catch (Exception e) { return f.isFile() ? f : null; }
    }

    /** the text patch and its compiled twin (tombi2 --text-patch X.tombipatch.json reads X.tpatch: tools/tpatch_build.py) */
    File textPatch() {
        patchAsset(TEXT_PATCH.replace(".tombipatch.json", ".tpatch"));
        return patchAsset(TEXT_PATCH);
    }

    String[] buildArgs() {
        List<String> a = new ArrayList<>();
        File disc = discFile();
        if (dataReady()) { a.add("--data"); a.add(dataDir().getAbsolutePath()); }   // extracted on the device
        else if (disc != null) { a.add("--disc"); a.add(disc.getAbsolutePath()); }  // an image copied by an older version
        if (textPatchOn() && LANG_N[langIdx()] == 5) {
            File tp = textPatch();
            if (tp != null) { a.add("--text-patch"); a.add(tp.getAbsolutePath()); }
        }
        a.add("--lang"); a.add(String.valueOf(LANG_N[langIdx()]));
        // Spanish: the graphic redraws (CARGANDO, CARGAR / GUARDAR, VUELVE A INTENTARLO), built by tombi2 itself from
        // the disc data at startup (shims/gfx_es.c; the PC launcher's tools/gfx_es file is not needed)
        if (LANG_N[langIdx()] == 5) a.add("--gfx-es");
        a.add("--aspect"); a.add(ASPECTS[aspectIdx()]);
        a.add("--renderer"); a.add(flag("gl", false) ? "gl" : "soft");
        if (flag("cut", false)) {
            a.add("--cut-content");
            // English cut lines: tombi2 picks en_cut_content.tpatch from --patch-dir by itself (as the PC launcher does)
            if (LANG_N[langIdx()] == 0 && patchAsset("en_cut_content.tpatch") != null) { a.add("--patch-dir"); a.add(new File(filesDir(), "patches").getAbsolutePath()); }
        }
        if (flag("debugmenu", false)) a.add("--debug-menu");
        // the panel only with the option on AND a secondary display present now (off: no Presentation, no HUD move)
        boolean panel = secondScreenOn() && hasSecondDisplay();
        if (panel) a.add("--second-screen");
        if (panel && hudBottomOn()) a.add("--hud-bottom");
        a.add("--files-dir"); a.add(filesDir().getAbsolutePath());   // the loader's files dir (taken out before port_main)
        if (flag("dualshock", false)) a.add("--dualshock");
        a.add("--prompts"); a.add(PROMPTS[promptIdx()]);
        if (flag("skipintro", false)) a.add("--skip-intro");
        if (flag("hz60", false)) { a.add("--hz"); a.add("60"); }
        if (flag("fps60", false)) a.add("--fps60");
        if (flag("showfps", false)) a.add("--show-fps");
        if (flag("inflives", false)) { a.add("--poke-every"); a.add("50:8009C980:63:1"); }
        if (flag("infhealth", false)) { a.add("--poke-every"); a.add("1:8009C970:4:1"); }
        return a.toArray(new String[0]);
    }

    void play() {
        if (!ready()) {
            Toast.makeText(this, R.string.need_disc, Toast.LENGTH_LONG).show();
            view.setScreen(LauncherView.DISC);
            return;
        }
        String[] args;
        try { args = buildArgs(); }
        catch (Throwable t) { Toast.makeText(this, String.valueOf(t), Toast.LENGTH_LONG).show(); return; }
        // markers (CrashLog): "running" until TombiActivity finishes cleanly; a stale "crashed" is cleared
        CrashLog.clearMarkers(this);
        CrashLog.writeMarker(this, CrashLog.RUNNING, "launcher " + version() + " " + System.currentTimeMillis() + "\n");
        try {
            Intent i = new Intent(this, TombiActivity.class);
            i.putExtra(TombiActivity.EXTRA_ARGS, args);
            String pm = "";
            try { pm = prefs.getString("padmap", ""); } catch (Throwable t) { CrashLog.append(this, "launcher: padmap pref: " + t); }
            i.putExtra(TombiActivity.EXTRA_PADMAP, pm);
            startActivity(i);
        }
        catch (Throwable t) {
            CrashLog.append(this, "launcher: startActivity(TombiActivity) failed: " + CrashLog.stack(t));
            CrashLog.writeMarker(this, CrashLog.CRASHED, "startActivity failed: " + t + "\n");
            view.setScreen(LauncherView.CRASH);
        }
    }

    String version() {
        try { return "v" + getPackageManager().getPackageInfo(getPackageName(), 0).versionName; }
        catch (Exception e) { return "v?"; }
    }

    // ---- SAF disc picker ----
    void pickDisc() {
        Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType("*/*");
        i.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true);   // a .cue together with its .bin
        try { startActivityForResult(i, REQ_DISC); }
        catch (ActivityNotFoundException e) { status(getString(R.string.disc_nosaf)); }
    }

    String displayName(Uri u) {
        Cursor c = getContentResolver().query(u, new String[]{android.provider.OpenableColumns.DISPLAY_NAME}, null, null, null);
        if (c != null) try { if (c.moveToFirst() && c.getString(0) != null) return c.getString(0); } finally { c.close(); }
        String l = u.getLastPathSegment();
        return l != null ? l.replaceAll(".*[/:]", "") : "disc.bin";
    }

    // ---- import state (written by the "tombi-disc" thread, drawn by the view at ~10 Hz while busy) ----
    static final int IDLE = 0, COPYING = 1, CHECKING = 2, EXTRACTING = 3;
    /* nativeExtract's content SHA-1 (every ISO9660 file as stored in data/, by LBA) for a raw .bin of the reference PAL disc */
    static final String CONTENT_SHA1 = "33d5f861ed02909236e9887376d80deaadbd234f";
    volatile int phase = IDLE;
    volatile long copied, copyTotal;          // bytes; copyTotal <= 0: size unknown (no OpenableColumns.SIZE)
    volatile boolean cancelImport;
    volatile int result;                       // 0 none, 1 success, -1 failure (after an import)

    static boolean nativeOk;
    static {
        try { System.loadLibrary("tombidisc"); nativeOk = true; } catch (Throwable t) { nativeOk = false; }
    }
    /** "sha1:<hex>" or "err:<reason>" ("err:nomain0"): port2/android/disccheck.c, the same for .bin/.iso/.chd */
    static native String nativeMain0Sha1(String path);
    static native int nativeCheckProgress();
    /** extracts the image into outDir for the port's --data mode (manifest.txt last): "ok:<summary>" or "err:<reason>" */
    static native String nativeExtract(String image, String outDir);
    static native int nativeExtractCount();   // files done << 16 | files total
    static native void nativeCancel();

    void requestCancel() { cancelImport = true; if (nativeOk) try { nativeCancel(); } catch (Throwable ignored) { } }

    long sizeOf(Uri u) {
        try (Cursor c = getContentResolver().query(u, new String[]{android.provider.OpenableColumns.SIZE}, null, null, null)) {
            if (c != null && c.moveToFirst() && !c.isNull(0)) return c.getLong(0);
        } catch (Exception e) { /* unknown */ }
        return -1;
    }

    /** The game's data, extracted on the device (docs/PORT.md "4. Android"): internal files/data with the manifest the
     *  port's --data mode reads; files/data.tmp while extracting (renamed when complete). */
    File dataDir() { return new File(getFilesDir(), "data"); }
    boolean dataReady() { return new File(dataDir(), "manifest.txt").isFile(); }

    /** The picked .bin/.iso/.chd (a .cue alongside is ignored: the data track is the image) is read in place through
     *  its file descriptor when the provider gives a real file, else copied to the cache first; then MAIN0.EXE's SHA-1,
     *  then every file extracted into files/data.tmp (each size-checked, any unreadable sector fails), renamed to
     *  files/data.  Cancel (B / "Cancel") stops and deletes the partial files. */
    void importDisc(List<Uri> uris) {
        cancelImport = false; result = 0; copied = 0; copyTotal = -1;
        phase = CHECKING;
        status(null);
        File tmpDir = new File(getCacheDir(), "import"), tmp = new File(getFilesDir(), "data.tmp");
        deleteTree(tmpDir);
        List<android.os.ParcelFileDescriptor> fds = new ArrayList<>();
        try {
            Uri img = null; String name = null;
            for (Uri u : uris) {
                String nm = displayName(u);
                if (!nm.toLowerCase(Locale.ROOT).endsWith(".cue") && img == null) { img = u; name = nm; }
            }
            if (img == null) { fail(getString(R.string.disc_needbin)); return; }
            long size = sizeOf(img);
            CrashLog.append(this, "disc import: " + name + " (" + (size >= 0 ? size + " bytes" : "size unknown") + "), " + uris.size() + " file(s) picked");
            long free = getFilesDir().getUsableSpace(), need = (size > 0 ? size : 700L << 20) + (64L << 20);
            if (free < need) { fail(getString(R.string.disc_space, need >> 20, free >> 20)); return; }
            String path = null;
            try {
                android.os.ParcelFileDescriptor pfd = getContentResolver().openFileDescriptor(img, "r");
                if (pfd != null) {
                    fds.add(pfd);
                    long st = pfd.getStatSize();
                    CrashLog.append(this, "disc import: provider fd " + pfd.getFd() + ", stat size " + st);
                    if (st > 0 && (size < 0 || st == size)) path = "/proc/self/fd/" + pfd.getFd();
                }
            } catch (Throwable t) { CrashLog.append(this, "disc import: no file descriptor (" + t + "): copying"); }
            if (path == null) {   // a stream only (cloud providers ...): copy to the cache, checked against the size
                if (free < need + (size > 0 ? size : 700L << 20)) { fail(getString(R.string.disc_space, (need + Math.max(size, 0)) >> 20, free >> 20)); return; }
                phase = COPYING; copyTotal = size;
                tmpDir.mkdirs();
                String low = name.toLowerCase(Locale.ROOT);
                File t = new File(tmpDir, "image" + (low.endsWith(".chd") ? ".chd" : low.endsWith(".iso") ? ".iso" : ".bin"));
                InputStream in = getContentResolver().openInputStream(img);
                if (in == null) throw new Exception(name);
                FileOutputStream out = new FileOutputStream(t);
                byte[] buf = new byte[1 << 18]; long got = 0; int n;
                try {
                    while ((n = in.read(buf)) >= 0) {
                        if (cancelImport) throw new InterruptedException();
                        if (n == 0) continue;
                        out.write(buf, 0, n); copied += n; got += n;
                    }
                    out.flush(); out.getFD().sync();
                } finally { in.close(); out.close(); }
                CrashLog.append(this, "disc import: copied " + got + " bytes" + (size >= 0 ? " of " + size : ""));
                if (size >= 0 && got != size) { fail(getString(R.string.disc_short, name, got, size)); return; }
                path = t.getAbsolutePath();
            }
            if (!nativeOk) { fail(getString(R.string.disc_fail, "libtombidisc.so missing")); return; }
            phase = CHECKING;
            view.postInvalidate();
            String r = nativeMain0Sha1(path);
            CrashLog.append(this, "disc import: MAIN0.EXE: " + r);
            String sha = r.startsWith("sha1:") ? r.substring(5) : null;
            if (sha == null) { fail(r.equals("err:nomain0") ? getString(R.string.disc_nomain0) : getString(R.string.disc_fail, r.substring(4))); return; }
            boolean ok = MAIN0_SHA1.equals(sha);
            if (cancelImport) throw new InterruptedException();
            phase = EXTRACTING;
            view.postInvalidate();
            deleteTree(tmp);
            String v = nativeExtract(path, tmp.getAbsolutePath());
            CrashLog.append(this, "disc import: extraction: " + v);
            if (v.equals("err:cancelled")) throw new InterruptedException();
            if (!v.startsWith("ok:")) { deleteTree(tmp); fail(getString(R.string.disc_damaged, v.startsWith("err:") ? v.substring(4) : v)); return; }
            deleteTree(dataDir());
            if (!tmp.renameTo(dataDir())) { deleteTree(tmp); fail(getString(R.string.disc_fail, "rename data.tmp")); return; }
            File old = discFile();   // an image copied by an older version: no longer needed
            if (old != null && "disc".equals(old.getParentFile().getName())) deleteTree(old.getParentFile());
            prefs.edit().remove("disc").putBoolean("disc_ok", ok).putString("disc_name", name)
                    .putString("disc_info", v.substring(3) + (v.endsWith(CONTENT_SHA1) ? " (= reference PAL disc)" : "")).apply();
            phase = IDLE;
            result = ok ? 1 : -1;
            status(ok ? getString(R.string.disc_ok) : getString(R.string.disc_badsha, sha));
        } catch (InterruptedException e) {
            deleteTree(tmp);
            phase = IDLE; result = -1;
            status(getString(R.string.disc_cancelled));
            CrashLog.append(this, "disc import: cancelled");
        } catch (Throwable e) {
            deleteTree(tmp);
            CrashLog.append(this, "disc import: " + CrashLog.stack(e));
            fail(getString(R.string.disc_fail, String.valueOf(e.getMessage())));
        } finally {
            for (android.os.ParcelFileDescriptor f : fds) try { f.close(); } catch (Throwable ignored) { }
            deleteTree(tmpDir);
        }
    }

    void fail(String msg) { phase = IDLE; result = -1; status(msg); CrashLog.append(this, "disc import failed: " + msg); }

    static void deleteTree(File f) {
        if (f == null || !f.exists()) return;
        File[] k = f.listFiles();
        if (k != null) for (File c : k) deleteTree(c);
        f.delete();
    }
    /** "Delete data" (Disc screen): the copied disc, the extracted patches, the caches and the markers; the saves only
     *  when asked; the disc selection reset */
    void wipeData(boolean saves) {
        File fd = filesDir();
        deleteTree(dataDir());
        deleteTree(new File(getFilesDir(), "data.tmp"));
        deleteTree(new File(fd, "disc"));
        deleteTree(new File(fd, "patches"));
        File[] cache = getCacheDir().listFiles();
        if (cache != null) for (File c : cache) deleteTree(c);
        if (getExternalCacheDir() != null) { File[] ec = getExternalCacheDir().listFiles(); if (ec != null) for (File c : ec) deleteTree(c); }
        if (saves) deleteTree(new File(fd, "saves"));
        CrashLog.clearMarkers(this);
        prefs.edit().remove("disc").remove("disc_ok").remove("disc_info").remove("disc_name").apply();
        CrashLog.append(this, "launcher: data deleted (saves " + (saves ? "deleted" : "kept") + ")");
        result = 1;
        status(getString(saves ? R.string.disc_wiped_saves : R.string.disc_wiped));
    }

    // ---- save import / export (SaveImport.java; the game's single card, files/saves/tombi_mc0.mcd) ----
    File cardFile() { File d = new File(filesDir(), "saves"); d.mkdirs(); return new File(d, "tombi_mc0.mcd"); }
    void pickSaveImport() {
        Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType("*/*");
        try { startActivityForResult(i, REQ_SAVE_IMPORT); }
        catch (ActivityNotFoundException e) { Toast.makeText(this, String.valueOf(e.getMessage()), Toast.LENGTH_LONG).show(); }
    }
    void pickSaveExport() {
        Intent i = new Intent(Intent.ACTION_CREATE_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType("application/octet-stream");
        i.putExtra(Intent.EXTRA_TITLE, "tombi.mcd");
        try { startActivityForResult(i, REQ_SAVE_EXPORT); }
        catch (ActivityNotFoundException e) { Toast.makeText(this, String.valueOf(e.getMessage()), Toast.LENGTH_LONG).show(); }
    }
    void saveDialog(String title, String msg) {
        new android.app.AlertDialog.Builder(this).setTitle(title).setMessage(msg).setPositiveButton(android.R.string.ok, null).show();
    }
    void doSaveImport(final Uri u) {
        final boolean es = getResources().getConfiguration().locale.getLanguage().equals("es");
        new android.app.AlertDialog.Builder(this)
            .setTitle(R.string.save_import)
            .setMessage(R.string.save_import_confirm)
            .setNegativeButton(R.string.cancel, null)
            .setPositiveButton(android.R.string.ok, new android.content.DialogInterface.OnClickListener() {
                public void onClick(android.content.DialogInterface d, int w) {
                    try {
                        InputStream in = getContentResolver().openInputStream(u);
                        if (in == null) throw new java.io.IOException("openInputStream");
                        byte[] raw;
                        try { raw = SaveImport.readAll(in, 4 << 20); } finally { in.close(); }
                        SaveImport.Result r = SaveImport.importBytes(raw, cardFile(), es);
                        StringBuilder sb = new StringBuilder();
                        for (String l : r.lines) sb.append(l).append('\n');
                        if (r.warn != null) sb.append('\n').append(r.warn);
                        CrashLog.append(LauncherActivity.this, "launcher: save imported: " + r.lines.get(0));
                        saveDialog(getString(R.string.save_import), sb.toString().trim());
                    } catch (Exception e) {
                        saveDialog(getString(R.string.save_import), getString(R.string.save_fail, String.valueOf(e.getMessage())));
                    }
                }
            }).show();
    }
    void doSaveExport(Uri u) {
        try {
            byte[] c = SaveImport.exportCard(cardFile());
            OutputStream o = getContentResolver().openOutputStream(u);
            if (o == null) throw new java.io.IOException("openOutputStream");
            try { o.write(c); } finally { o.close(); }
            Toast.makeText(this, R.string.save_exported, Toast.LENGTH_LONG).show();
        } catch (Exception e) {
            saveDialog(getString(R.string.save_export), getString(R.string.save_fail, String.valueOf(e.getMessage())));
        }
    }

    @Override
    protected void onActivityResult(int req, int res, Intent data) {
        super.onActivityResult(req, res, data);
        if (req == REQ_EXPORT && res == RESULT_OK && data != null && data.getData() != null) { writeExport(data.getData()); return; }
        if (req == REQ_SAVE_IMPORT && res == RESULT_OK && data != null && data.getData() != null) { doSaveImport(data.getData()); return; }
        if (req == REQ_SAVE_EXPORT && res == RESULT_OK && data != null && data.getData() != null) { doSaveExport(data.getData()); return; }
        if (req == REQ_DISC && res == RESULT_OK && data != null) {
            final List<Uri> uris = new ArrayList<>();
            if (data.getClipData() != null)
                for (int i = 0; i < data.getClipData().getItemCount(); i++) uris.add(data.getClipData().getItemAt(i).getUri());
            else if (data.getData() != null) uris.add(data.getData());
            if (!uris.isEmpty()) new Thread(new Runnable() { public void run() { importDisc(uris); } }, "tombi-disc").start();
        }
    }

    void status(String s) { importStatus = s; if (view != null) view.postInvalidate(); }

    @Override
    public boolean onKeyDown(int code, KeyEvent e) {
        if (view.capture >= 0) return view.captureKey(code, e, true);
        return view.key(code) || super.onKeyDown(code, e);
    }
    @Override
    public boolean onKeyUp(int code, KeyEvent e) {
        if (view.capture >= 0 || view.swallowUp == code) return view.captureKey(code, e, false);
        return super.onKeyUp(code, e);
    }
    @Override
    public boolean onGenericMotionEvent(MotionEvent e) {
        if (view.capture >= 0 && view.captureMotion(e)) return true;
        if (e.getActionMasked() == MotionEvent.ACTION_SCROLL && view.wheel(e.getAxisValue(MotionEvent.AXIS_VSCROLL))) return true;
        return super.onGenericMotionEvent(e);
    }

    // ---------------------------------------------------------------------------------------------------------------
    static class Item {
        String label; String value; String sub; RectF r = new RectF(); boolean primary;   // sub: category summary
        Runnable act; Adjust adj;
        int check = -1;   // on/off option: 0/1, drawn as a tick box instead of a value
        Item(String l, Runnable a) { label = l; act = a; }
    }
    interface Adjust { void by(int d); }

    class LauncherView extends View {
        static final int MAIN = 0, OPTIONS = 1, DISC = 2, CRASH = 3, LOG = 4, PADMAP = 5, OPTCAT = 6;
        /* Options = a list of categories; OPTCAT = the page of category `cat` (CAT_GAME..CAT_DEBUG).
         *  discFromOptions = the Disc screen was opened from an Options page (B/Back return there) */
        static final int CAT_GAME = 0, CAT_GFX = 1, CAT_DUAL = 2, CAT_PAD = 3, CAT_DATA = 4, CAT_DEBUG = 5;
        int screen = MAIN, focus = 0, cat = CAT_GAME;
        int[] lastFocus = new int[7];
        /* Scrolling list (only the pad-mapping list overflows now; the 1280x720 canvas is scaled to fit any screen,
         *  so Options needs none): listScroll/listMax in canvas units. Keys keep the focused row visible
         *  (followFocus); a drag moves the list, with fling from the release velocity; a touch that moved less than
         *  the slop (8dp) is a tap. */
        float listScroll, listMax;
        boolean followFocus = true, dragging;
        float downX, downY, lastY;
        final float slop = 8 * getResources().getDisplayMetrics().density;
        VelocityTracker vt;
        final OverScroller scroller = new OverScroller(getContext());

        boolean scrolls() { return screen == PADMAP && listMax > 0; }

        boolean wheel(float v) {
            if (!scrolls() || v == 0) return false;
            scroller.forceFinished(true);
            followFocus = false;
            listScroll = Math.max(0, Math.min(listMax, listScroll - v * 60));
            invalidate();
            return true;
        }

        @Override
        public void computeScroll() {
            if (scroller.computeScrollOffset()) {
                listScroll = Math.max(0, Math.min(listMax, scroller.getCurrY() / s));
                postInvalidateOnAnimation();
            }
        }
        boolean discFromOptions;
        List<String> logLines = new ArrayList<>();
        List<Item> items = new ArrayList<>();
        final Paint p = new Paint(Paint.ANTI_ALIAS_FLAG);
        final TextPaint tp = new TextPaint(Paint.ANTI_ALIAS_FLAG);
        Typeface lilita, nunito;
        float s = 1, ox, oy;
        final RectF tmp = new RectF();

        LauncherView(Context c) {
            super(c);
            lilita = font("fonts/LilitaOne-Regular.ttf", 0);
            nunito = font("fonts/Nunito.ttf", 800);
            if (lilita == null) lilita = Typeface.create(Typeface.SANS_SERIF, Typeface.BOLD);
            if (nunito == null) nunito = Typeface.create(Typeface.SANS_SERIF, Typeface.BOLD);
        }

        Typeface font(String path, int weight) {
            try {
                if (weight > 0 && Build.VERSION.SDK_INT >= 26)
                    return new Typeface.Builder(getAssets(), path).setFontVariationSettings("'wght' " + weight).build();
                return Typeface.createFromAsset(getAssets(), path);
            } catch (Exception e) { return null; }
        }

        boolean confirmWipe, wipeSaves;

        void setScreen(int sc) {
            confirmWipe = false;
            padStatus = null; capture = -1;
            lastFocus[screen] = focus;
            if (sc == DISC) discFromOptions = screen == OPTCAT;
            screen = sc;
            focus = lastFocus[sc];
            listScroll = 0; followFocus = true; scroller.forceFinished(true);
            if (sc != DISC && phase == IDLE) { importStatus = null; result = 0; }
            if (sc == CRASH || sc == LOG) {
                try { logLines = CrashLog.tail(LauncherActivity.this, 40); } catch (Throwable t) { logLines = new ArrayList<>(); }
                focus = 0;
            }
            invalidate();
        }

        String onoff(boolean b) { return getString(b ? R.string.on : R.string.off); }

        Item toggle(int label, final String key) {
            final Item it = new Item(getString(label), null);
            it.check = flag(key, false) ? 1 : 0;
            it.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean(key, !flag(key, false)).apply(); } };
            return it;
        }

        void build() {
            items.clear();
            if (screen == MAIN) {
                Item a = new Item(getString(R.string.play), new Runnable() { public void run() { play(); } });
                a.primary = true; items.add(a);
                items.add(new Item(getString(R.string.options), new Runnable() { public void run() { setScreen(OPTIONS); } }));
                items.add(new Item(getString(R.string.disc), new Runnable() { public void run() { setScreen(DISC); } }));
                items.add(new Item(getString(R.string.quit), new Runnable() { public void run() { finish(); } }));
            } else if (screen == OPTIONS) {   // category list, one column (drawOptions)
                category(CAT_GAME, R.string.cat_game, LANG_NAME[langIdx()]);
                category(CAT_GFX, R.string.cat_gfx, aspectName(aspectIdx()) + " · " + getString(flag("gl", false) ? R.string.renderer_gl_short : R.string.renderer_soft));
                category(CAT_DUAL, R.string.cat_dual, onoff(secondScreenOn()));
                category(CAT_PAD, R.string.cat_pad, getResources().getStringArray(R.array.prompt_names)[promptIdx()]);
                category(CAT_DATA, R.string.cat_data, null);
                category(CAT_DEBUG, R.string.cat_debug, null);
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(MAIN); } }));
            } else if (screen == OPTCAT) {   // one category page, same row style (drawOptions)
                if (cat == CAT_GAME) {
                    Item l = new Item(getString(R.string.opt_lang), null);
                    l.value = LANG_NAME[langIdx()];
                    l.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("lang", (langIdx() + d + LANG_N.length) % LANG_N.length).apply(); } };
                    items.add(l);
                    Item tpi = new Item(getString(R.string.opt_textpatch), null);
                    tpi.check = textPatchOn() ? 1 : 0;
                    tpi.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("textpatch", !textPatchOn()).apply(); } };
                    items.add(tpi);
                    items.add(toggle(R.string.opt_cut, "cut"));
                    items.add(toggle(R.string.opt_skipintro, "skipintro"));
                } else if (cat == CAT_GFX) {
                    Item as = new Item(getString(R.string.opt_aspect), null);
                    as.value = aspectName(aspectIdx());
                    as.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("aspect", (aspectIdx() + d + ASPECTS.length) % ASPECTS.length).apply(); } };
                    items.add(as);
                    Item r = new Item(getString(R.string.opt_renderer), null);
                    r.value = getString(flag("gl", false) ? R.string.renderer_gl : R.string.renderer_soft);
                    r.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("gl", !flag("gl", false)).apply(); } };
                    items.add(r);
                    items.add(toggle(R.string.opt_hz60, "hz60"));
                    items.add(toggle(R.string.opt_fps60, "fps60"));
                    items.add(toggle(R.string.opt_showfps, "showfps"));
                } else if (cat == CAT_DUAL) {
                    Item ss = new Item(getString(R.string.opt_secondscreen), null);
                    ss.check = secondScreenOn() ? 1 : 0;
                    ss.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("secondscreen", !secondScreenOn()).apply(); } };
                    items.add(ss);
                    Item hb = new Item(getString(R.string.opt_hudbottom), null);
                    hb.check = hudBottomOn() ? 1 : 0;
                    hb.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("hudbottom", !hudBottomOn()).apply(); } };
                    items.add(hb);
                } else if (cat == CAT_PAD) {
                    items.add(new Item(getString(R.string.opt_padmap), new Runnable() { public void run() { setScreen(PADMAP); } }));
                    items.add(toggle(R.string.opt_dualshock, "dualshock"));   // --dualshock (shims/pad.c)
                    Item pr = new Item(getString(R.string.opt_prompts), null);   // --prompts (shims/prompts.c; TombiActivity resolves auto)
                    pr.value = getResources().getStringArray(R.array.prompt_names)[promptIdx()];
                    pr.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("prompts", (promptIdx() + d + PROMPTS.length) % PROMPTS.length).apply(); } };
                    items.add(pr);
                } else if (cat == CAT_DATA) {
                    items.add(new Item(getString(R.string.save_import), new Runnable() { public void run() { pickSaveImport(); } }));
                    items.add(new Item(getString(R.string.save_export), new Runnable() { public void run() { pickSaveExport(); } }));
                    items.add(new Item(getString(R.string.opt_disc), new Runnable() { public void run() { setScreen(DISC); } }));
                    items.add(new Item(getString(R.string.disc_wipe), new Runnable() { public void run() {
                        setScreen(DISC); confirmWipe = true; wipeSaves = false; focus = 0; } }));
                } else {
                    items.add(toggle(R.string.opt_inflives, "inflives"));
                    items.add(toggle(R.string.opt_infhealth, "infhealth"));
                    items.add(toggle(R.string.opt_debug, "debugmenu"));
                    items.add(new Item(getString(R.string.opt_viewlog), new Runnable() { public void run() { setScreen(LOG); } }));
                }
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(OPTIONS); } }));
            } else if (screen == PADMAP) {
                PadMap pm = new PadMap(prefs.getString("padmap", ""));
                String[] names = getResources().getStringArray(R.array.pad_names);
                for (int t = 0; t < PadMap.NAMES.length; t++) {
                    final int target = t;
                    Item it = new Item(t < names.length ? names[t] : PadMap.NAMES[t], new Runnable() { public void run() { startCapture(target); } });
                    it.value = pm.describe(t, spanish());
                    items.add(it);
                }
                items.add(new Item(getString(R.string.pad_reset), new Runnable() { public void run() {
                    prefs.edit().remove("padmap").apply(); padStatus = getString(R.string.pad_reset_done); } }));
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(OPTCAT); } }));
            } else if (screen == CRASH || screen == LOG) {
                items.add(new Item(getString(R.string.log_copy), new Runnable() { public void run() { copyReport(); } }));
                items.add(new Item(getString(R.string.log_share), new Runnable() { public void run() { shareReport(); } }));
                items.add(new Item(getString(R.string.log_export), new Runnable() { public void run() { exportReport(); } }));
                items.add(new Item(getString(screen == CRASH ? R.string.log_close : R.string.back), new Runnable() { public void run() { closeLog(); } }));
            } else if (phase != IDLE) {   // importing: the only action is Cancel (also B)
                items.add(new Item(getString(R.string.cancel), new Runnable() { public void run() { requestCancel(); } }));
            } else if (confirmWipe) {   // in-page confirmation of "Delete data"
                items.add(new Item(getString(R.string.disc_wipe_yes), new Runnable() { public void run() { wipeData(wipeSaves); confirmWipe = false; } }));
                final Item sv = new Item(getString(R.string.disc_wipe_saves), null);
                sv.value = onoff(wipeSaves);
                sv.adj = new Adjust() { public void by(int d) { wipeSaves = !wipeSaves; } };
                items.add(sv);
                items.add(new Item(getString(R.string.cancel), new Runnable() { public void run() { confirmWipe = false; focus = 0; } }));
            } else {
                items.add(new Item(getString(R.string.disc_pick), new Runnable() { public void run() { pickDisc(); } }));
                items.add(new Item(getString(R.string.disc_wipe), new Runnable() { public void run() { confirmWipe = true; wipeSaves = false; focus = 0; } }));
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(discFromOptions ? OPTCAT : MAIN); } }));
            }
            if (focus >= items.size()) focus = 0;
        }

        /* leaving the crash card clears the markers (the log stays: Options > View last log) */
        void closeLog() {
            if (screen == CRASH) { CrashLog.clearMarkers(LauncherActivity.this); setScreen(MAIN); }
            else setScreen(OPTCAT);
        }

        /** a row of the Options list: opens page c (focus on its first row) */
        void category(final int c, int label, String sub) {
            Item it = new Item(getString(label), new Runnable() { public void run() {
                cat = c; setScreen(OPTCAT); focus = 0; } });
            it.sub = sub;
            items.add(it);
        }

        String aspectName(int i) { return i == 0 ? getString(R.string.aspect_auto) : ASPECTS[i]; }

        // ---- input ----
        boolean key(int code) {
            build();
            int n = items.size();
            switch (code) {
                case KeyEvent.KEYCODE_DPAD_UP: focus = (focus + n - 1) % n; break;
                case KeyEvent.KEYCODE_DPAD_DOWN: focus = (focus + 1) % n; break;
                case KeyEvent.KEYCODE_DPAD_LEFT: case KeyEvent.KEYCODE_DPAD_RIGHT: {
                    if (screen == CRASH || screen == LOG) { focus = (focus + (code == KeyEvent.KEYCODE_DPAD_LEFT ? n - 1 : 1)) % n; break; }
                    Item it = items.get(focus);
                    if (it.adj != null) it.adj.by(code == KeyEvent.KEYCODE_DPAD_LEFT ? -1 : 1);
                    break;
                }
                case KeyEvent.KEYCODE_BUTTON_A: case KeyEvent.KEYCODE_DPAD_CENTER: case KeyEvent.KEYCODE_ENTER:
                case KeyEvent.KEYCODE_NUMPAD_ENTER: case KeyEvent.KEYCODE_SPACE:
                    activate(items.get(focus)); break;
                case KeyEvent.KEYCODE_BUTTON_B: case KeyEvent.KEYCODE_BACK: case KeyEvent.KEYCODE_ESCAPE:
                    if (phase != IDLE) requestCancel();
                    else if (screen == CRASH || screen == LOG) closeLog();
                    else if (screen == MAIN) finish(); else if (screen == OPTCAT) setScreen(OPTIONS);
                    else setScreen(screen == PADMAP || (screen == DISC && discFromOptions) ? OPTCAT : MAIN);
                    break;
                case KeyEvent.KEYCODE_BUTTON_START:
                    if (phase != IDLE) break;
                    if (screen == CRASH || screen == LOG) closeLog();
                    else if (screen == MAIN) play(); else setScreen(MAIN);
                    break;
                default: return false;
            }
            followFocus = true;
            invalidate();
            return true;
        }

        void activate(Item it) {
            if (it.adj != null) it.adj.by(1);
            else if (it.act != null) it.act.run();
            invalidate();
        }

        @Override
        public boolean onTouchEvent(MotionEvent e) {
            if (capture >= 0) { if (e.getActionMasked() == MotionEvent.ACTION_UP) endCapture(null); return true; }
            float x = (e.getX() - ox) / s, y = (e.getY() - oy) / s;
            int hit = -1;
            for (int i = 0; i < items.size(); i++) if (items.get(i).r.contains(x, y)) hit = i;
            int act = e.getActionMasked();
            if (vt == null) vt = VelocityTracker.obtain();
            vt.addMovement(e);
            if (act == MotionEvent.ACTION_DOWN) {
                boolean flinging = !scroller.isFinished();
                scroller.forceFinished(true);
                downX = e.getX(); downY = lastY = e.getY(); dragging = flinging;   // a touch that stops a fling is no tap
                if (hit >= 0 && !flinging) { focus = hit; invalidate(); }
                return true;
            }
            if (act == MotionEvent.ACTION_MOVE) {
                if (!dragging && Math.hypot(e.getX() - downX, e.getY() - downY) > slop) dragging = true;
                if (dragging && scrolls()) {
                    followFocus = false;
                    listScroll = Math.max(0, Math.min(listMax, listScroll - (e.getY() - lastY) / s));
                    invalidate();
                }
                lastY = e.getY();
                return true;
            }
            if (act == MotionEvent.ACTION_UP || act == MotionEvent.ACTION_CANCEL) {
                boolean wasDrag = dragging;
                dragging = false;
                if (wasDrag && scrolls() && act == MotionEvent.ACTION_UP) {
                    vt.computeCurrentVelocity(1000);
                    float vy = vt.getYVelocity();
                    if (Math.abs(vy) > 50 * getResources().getDisplayMetrics().density) {
                        scroller.fling(0, Math.round(listScroll * s), 0, Math.round(-vy), 0, 0, 0, Math.round(listMax * s));
                        postInvalidateOnAnimation();
                    }
                }
                vt.recycle(); vt = null;
                if (wasDrag || act == MotionEvent.ACTION_CANCEL) return true;
            }
            if (act == MotionEvent.ACTION_UP && hit >= 0 && hit == focus) {
                build();
                if (screen == OPTCAT && hit < items.size()) {   // value button: left half = previous, right half = next
                    Item it = items.get(hit);
                    if (it.adj != null && it.check < 0) { it.adj.by(x < it.r.centerX() ? -1 : 1); invalidate(); return true; }
                }
                if (hit < items.size()) activate(items.get(hit));
            }
            return true;
        }

        // ---- drawing ----
        void text(Typeface tf, float size, int color) {
            p.setTypeface(tf); p.setTextSize(size); p.setColor(color); p.setLetterSpacing(0);
            p.setTextAlign(Paint.Align.CENTER); p.setStyle(Paint.Style.FILL);
        }

        float midBase(RectF r) { return r.centerY() - (p.descent() + p.ascent()) / 2; }

        void pill(RectF r, int bg, int shadow, float sh, boolean focused) {
            float rad = r.height() / 2;
            if (focused) {   // white focus ring around the pill and its shadow
                p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(5); p.setColor(Color.WHITE);
                tmp.set(r.left - 7, r.top - 7, r.right + 7, r.bottom + sh + 7);
                canvas.drawRoundRect(tmp, tmp.height() / 2, tmp.height() / 2, p);
                p.setStyle(Paint.Style.FILL);
            }
            p.setColor(shadow);
            tmp.set(r.left, r.top + sh, r.right, r.bottom + sh);
            canvas.drawRoundRect(tmp, rad, rad, p);
            p.setColor(bg);
            canvas.drawRoundRect(r, rad, rad, p);
        }

        void bigTitle(String t, float size, float base) {
            text(lilita, size, ACCENT_SH);
            canvas.drawText(t, 640, base + size * 6 / 108, p);
            p.setColor(ACCENT);
            canvas.drawText(t, 640, base, p);
        }

        Canvas canvas;

        @Override
        protected void onDraw(Canvas c) {
            canvas = c;
            int w = getWidth(), h = getHeight();
            s = Math.min(w / 1280f, h / 720f);
            ox = (w - 1280 * s) / 2; oy = (h - 720 * s) / 2;
            c.drawColor(BG);
            p.setStyle(Paint.Style.FILL);
            p.setColor(CIRCLE_BL); c.drawCircle(110 * s, h + 70 * s, 400 * s, p);
            p.setColor(CIRCLE_TR); c.drawCircle(w - 90 * s, -60 * s, 340 * s, p);
            build();
            c.save();
            c.translate(ox, oy);
            c.scale(s, s);
            if (screen == MAIN) drawMain();
            else if (screen == OPTIONS || screen == OPTCAT) drawOptions();
            else if (screen == PADMAP) drawPadMap();
            else if (screen == CRASH || screen == LOG) drawLog();
            else drawDisc();
            c.restore();
        }

        void drawMain() {
            bigTitle("TOMBI!", 108, 170);
            text(nunito, 20, SUB);
            p.setLetterSpacing(3f / 20f);
            canvas.drawText(getString(R.string.subtitle), 640, 214, p);
            float y = 256;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                float hh = it.primary ? 78 : 62;
                it.r.set(640 - 230, y, 640 + 230, y + hh);
                boolean f = i == focus;
                if (it.primary) pill(it.r, ACCENT, ACCENT_SH, 6, f);
                else pill(it.r, f ? Color.WHITE : CREAM, CREAM_SH, 5, f);
                text(nunito, it.primary ? 30 : 24, it.primary ? ACCENT_TEXT : BG);
                canvas.drawText(it.label, 640, midBase(it.r), p);
                y += hh + (it.primary ? 6 : 5) + 16;
            }
            String[] chips = {LANG_NAME[langIdx()], aspectName(aspectIdx()),
                    getString(flag("cut", false) ? R.string.chip_cut_on : R.string.chip_cut_off), version()};
            text(nunito, 15, Color.WHITE);
            float pad = 18, gap = 12, total = -gap;
            for (String t : chips) total += p.measureText(t) + 2 * pad + gap;
            float x = 640 - total / 2;
            for (String t : chips) {
                float cw = p.measureText(t) + 2 * pad;
                tmp.set(x, 650, x + cw, 684);
                p.setColor(CHIP);
                canvas.drawRoundRect(tmp, 17, 17, p);
                p.setColor(Color.WHITE);
                canvas.drawText(t, tmp.centerX(), midBase(tmp), p);
                x += cw + gap;
            }
        }

        final android.graphics.Path tri = new android.graphics.Path();
        /** a filled 12x16 triangle pointing right (dir 1) or left (-1) from x, in the current paint colour */
        void arrow(float x, float cy, int dir) {
            tri.reset();
            tri.moveTo(x + dir * 12, cy); tri.lineTo(x, cy - 8); tri.lineTo(x, cy + 8); tri.close();
            p.setStyle(Paint.Style.FILL);
            canvas.drawPath(tri, p);
        }

        void drawOptions() {
            int[] catNames = {R.string.cat_game, R.string.cat_gfx, R.string.cat_dual, R.string.cat_pad, R.string.cat_data, R.string.cat_debug};
            bigTitle(screen == OPTIONS ? getString(R.string.options) : getString(catNames[cat]), 50, 66);
            // one column, no scrolling: rows (at most 72 tall incl. a 12 gap) between 100 and 640; the help text
            // has its own band below (650..712), so it never overlaps a button
            float top = 100, bottom = 640, rowH = Math.min(72, (bottom - top + 12) / Math.max(1, items.size())), bh = rowH - 12;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                float y = top + i * rowH;
                it.r.set(640 - 400, y, 640 + 400, y + bh);
                pill(it.r, f ? ACCENT : CREAM, f ? ACCENT_SH : CREAM_SH, 4, f);
                int col = f ? ACCENT_TEXT : BG;
                text(nunito, 22, col);
                if (it.sub != null) {   // category with a summary on the right
                    p.setTextAlign(Paint.Align.LEFT);
                    canvas.drawText(it.label, it.r.left + 30, midBase(it.r), p);
                    p.setTextSize(17); p.setTextAlign(Paint.Align.RIGHT);
                    canvas.drawText(it.sub, it.r.right - 30, midBase(it.r), p);
                } else if (it.check >= 0) {   // on/off: label + tick box on the right
                    p.setTextAlign(Paint.Align.LEFT);
                    canvas.drawText(it.label, it.r.left + 26, midBase(it.r), p);
                    float bx = it.r.right - 22 - 30, by = it.r.centerY() - 15;
                    tmp.set(bx, by, bx + 30, by + 30);
                    if (it.check == 1) {
                        p.setColor(f ? ACCENT_TEXT : ACCENT_SH);
                        canvas.drawRoundRect(tmp, 8, 8, p);
                        p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(4.5f); p.setStrokeCap(Paint.Cap.ROUND);
                        p.setColor(f ? ACCENT : Color.WHITE);
                        canvas.drawLine(bx + 7, by + 16, bx + 13, by + 22, p);
                        canvas.drawLine(bx + 13, by + 22, bx + 24, by + 9, p);
                        p.setStrokeCap(Paint.Cap.BUTT);
                    } else {
                        p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(3.5f); p.setColor(col);
                        tmp.inset(2, 2);
                        canvas.drawRoundRect(tmp, 7, 7, p);
                    }
                    p.setStyle(Paint.Style.FILL);
                } else if (it.value == null) {   // submenu / action
                    canvas.drawText(it.label, it.r.centerX(), midBase(it.r), p);
                } else {   // value: label + "◀ value ▶"
                    p.setTextAlign(Paint.Align.LEFT);
                    canvas.drawText(it.label, it.r.left + 26, midBase(it.r), p);
                    p.setTextAlign(Paint.Align.RIGHT);
                    float vr = it.r.right - 50, cy = it.r.centerY();
                    canvas.drawText(it.value, vr, midBase(it.r), p);
                    arrow(vr + 14, cy, 1);
                    arrow(vr - p.measureText(it.value) - 14, cy, -1);
                }
            }
            tp.setTypeface(nunito); tp.setTextSize(14); tp.setColor(SUB);
            tp.setTextAlign(Paint.Align.LEFT);
            wrapped(getString(R.string.opt_hint), 656);
        }

        // ---- Options > Controls / Button mapping ----
        int capture = -1, swallowUp = -1;        // the target waiting for a button; a key whose release is eaten
        String padStatus;
        long bDownAt;
        final float[] axisBase = new float[64];
        final boolean[] axisSeen = new boolean[64];
        final Runnable holdCancel = new Runnable() { public void run() {
            if (capture >= 0 && bDownAt != 0) { swallowUp = KeyEvent.KEYCODE_BUTTON_B; endCapture(null); }
        } };

        boolean spanish() { return getResources().getConfiguration().locale.getLanguage().equals("es"); }
        void startCapture(int t) {
            capture = t; bDownAt = 0; padStatus = null;
            java.util.Arrays.fill(axisSeen, false);
            invalidate();
        }
        /* s: the source to bind ("k103", "a18+"), null: cancelled.  A source another target had moves there from it:
           that target gets this one's old first source instead (a swap), so no button does two things by accident */
        void endCapture(String s) {
            int t = capture;
            capture = -1; bDownAt = 0;
            removeCallbacks(holdCancel);
            String[] names = getResources().getStringArray(R.array.pad_names);
            if (s == null || t < 0) { padStatus = getString(R.string.pad_cancelled); invalidate(); return; }
            PadMap pm = new PadMap(prefs.getString("padmap", ""));
            String old = pm.src[t].length > 0 ? pm.src[t][0] : null;
            for (int u = 0; u < PadMap.NAMES.length; u++) {
                if (u == t) continue;
                List<String> l = new ArrayList<>();
                boolean had = false;
                for (String x : pm.src[u]) { if (x.equals(s)) { had = true; if (old != null && !l.contains(old)) l.add(old); } else l.add(x); }
                if (had) pm.src[u] = l.toArray(new String[0]);
            }
            pm.src[t] = new String[]{s};
            prefs.edit().putString("padmap", pm.save()).apply();
            padStatus = getString(R.string.pad_bound, t < names.length ? names[t] : PadMap.NAMES[t], PadMap.srcName(s, spanish()));
            invalidate();
        }
        /* a key while waiting: a controller button binds (B: on release, unless held 1 s = cancel); Esc / the
           system back cancel */
        boolean captureKey(int code, KeyEvent e, boolean down) {
            if (capture < 0) { if (!down && code == swallowUp) { swallowUp = -1; return true; } return false; }
            boolean pad = PadMap.fromController(e.getDevice(), e.getSource());
            if (down && e.getRepeatCount() > 0) return true;
            if (code == KeyEvent.KEYCODE_ESCAPE || (code == KeyEvent.KEYCODE_BACK && !pad)) { if (down) { swallowUp = code; endCapture(null); } return true; }
            if (PadMap.systemKey(code) || !pad) return false;
            if (code == KeyEvent.KEYCODE_BUTTON_B) {
                if (down) { bDownAt = android.os.SystemClock.uptimeMillis(); postDelayed(holdCancel, 1000); }
                else if (bDownAt != 0) endCapture("k" + code);
                return true;
            }
            if (down) { swallowUp = code; endCapture("k" + code); }
            return true;
        }
        /* an axis while waiting: moved past half way from where it rested binds that direction */
        boolean captureMotion(MotionEvent e) {
            if ((e.getSource() & android.view.InputDevice.SOURCE_JOYSTICK) != android.view.InputDevice.SOURCE_JOYSTICK || e.getDevice() == null) return false;
            for (android.view.InputDevice.MotionRange r : e.getDevice().getMotionRanges()) {
                int a = r.getAxis();
                if (a < 0 || a >= axisBase.length) continue;
                float v = e.getAxisValue(a);
                if (!axisSeen[a]) { axisSeen[a] = true; axisBase[a] = Math.abs(v) > 0.5f ? 0 : v; }
                if (Math.abs(v - axisBase[a]) > 0.6f && Math.abs(v) > 0.5f) { endCapture("a" + a + (v > 0 ? "+" : "-")); return true; }
            }
            return true;
        }

        void drawPadMap() {
            bigTitle(getString(R.string.pad_title), 50, 66);
            float top = 88, bottom = 640, rowH = 40;
            listMax = Math.max(0, items.size() * rowH - (bottom - top));
            if (followFocus)   // keyboard / pad: keep the focused row inside the window
                listScroll = Math.max(focus * rowH - (bottom - top) + rowH, Math.min(listScroll, focus * rowH));
            listScroll = Math.max(0, Math.min(listMax, listScroll));
            float y = top - listScroll;
            canvas.save();
            canvas.clipRect(0, top - 10, 1280, bottom + 4);
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 380, y, 640 + 380, y + 34);
                if (y + 34 < top - 10 || y > bottom) it.r.set(-1, -1, -1, -1);
                pill(it.r, f ? ACCENT : CREAM, f ? ACCENT_SH : CREAM_SH, 4, f);
                text(nunito, 18, f ? ACCENT_TEXT : BG);
                if (it.value == null) canvas.drawText(it.label, 640, midBase(it.r), p);
                else {
                    p.setTextAlign(Paint.Align.LEFT); canvas.drawText(it.label, it.r.left + 26, midBase(it.r), p);
                    p.setTextAlign(Paint.Align.RIGHT); canvas.drawText(it.value, it.r.right - 26, midBase(it.r), p);
                }
                y += rowH;
            }
            canvas.restore();
            tp.setTypeface(nunito); tp.setTextSize(14); tp.setColor(SUB); tp.setTextAlign(Paint.Align.LEFT);
            wrapped(padStatus != null ? padStatus : getString(R.string.pad_hint), 650);
            if (capture >= 0) {   // the waiting card over the list
                String[] names = getResources().getStringArray(R.array.pad_names);
                p.setStyle(Paint.Style.FILL); p.setColor(0xc0000000);
                canvas.drawRect(0, 0, 1280, 720, p);
                tmp.set(640 - 360, 260, 640 + 360, 460);
                pill(tmp, CREAM, CREAM_SH, 6, false);
                text(lilita, 34, ACCENT_SH);
                canvas.drawText(getString(R.string.pad_press, capture < names.length ? names[capture] : PadMap.NAMES[capture]), 640, 340, p);
                text(nunito, 18, BG);
                canvas.drawText(getString(R.string.pad_cancel), 640, 400, p);
            }
        }

        void wrapped(String t, float top) {
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout(t, tp, 900, Layout.Alignment.ALIGN_CENTER, 1.1f, 0, false);
            canvas.save();
            canvas.translate(640 - 450, top);
            l.draw(canvas);
            canvas.restore();
        }

        /** Selva pill bar: dark track, accent fill with its shadow edge (as the buttons), phase / % / MB labels. */
        void progressBar(int ph, float top) {
            long done = copied, tot = copyTotal;
            float frac;
            if (ph == CHECKING || ph == EXTRACTING) frac = nativeOk ? Math.max(0, Math.min(1, nativeCheckProgress() / 1000f)) : 0;
            else frac = tot > 0 ? Math.min(1f, done / (float) tot) : 0;
            String label = getString(ph == EXTRACTING ? R.string.disc_phase_extract : ph == CHECKING ? R.string.disc_phase_check : R.string.disc_phase_copy);
            text(nunito, 20, Color.WHITE);
            p.setTextAlign(Paint.Align.LEFT);
            canvas.drawText(label, 640 - 300, top + 6, p);
            p.setTextAlign(Paint.Align.RIGHT);
            String pct = ph == CHECKING || ph == EXTRACTING || tot > 0 ? Math.round(frac * 100) + " %" : "";
            canvas.drawText(pct, 640 + 300, top + 6, p);
            RectF track = new RectF(640 - 300, top + 20, 640 + 300, top + 50);
            float rad = track.height() / 2;
            p.setColor(0x40000000);
            canvas.drawRoundRect(track, rad, rad, p);
            if (frac > 0) {
                float wFill = Math.max(track.height(), track.width() * frac);
                RectF f = new RectF(track.left, track.top, track.left + wFill, track.bottom - 4);
                p.setColor(ACCENT_SH);
                tmp.set(f.left, f.top + 4, f.right, f.bottom + 4);
                canvas.drawRoundRect(tmp, rad, rad, p);
                p.setColor(ACCENT);
                canvas.drawRoundRect(f, rad, rad, p);
                p.setColor(0x40ffffff);   // highlight along the top edge
                tmp.set(f.left + rad / 2, f.top + 4, f.right - rad / 2, f.top + 9);
                if (tmp.width() > 0) canvas.drawRoundRect(tmp, 3, 3, p);
            }
            text(nunito, 17, SUB);
            String mb = ph == EXTRACTING ? extractDetail() : ph == CHECKING ? "MAIN0.EXE (SHA-1)"
                    : tot > 0 ? (done >> 20) + " / " + (tot >> 20) + " MB" : (done >> 20) + " MB";
            canvas.drawText(mb, 640, top + 78, p);
        }

        String extractDetail() {
            int c = nativeOk ? nativeExtractCount() : 0;
            return getString(R.string.disc_extract_files, (c >>> 16) & 0xFFFF, c & 0xFFFF);
        }

        void resultBox(boolean ok, String msg, float top) {
            tmp.set(640 - 320, top, 640 + 320, top + 80);
            p.setStyle(Paint.Style.FILL);
            p.setColor(ok ? 0x5527c25a : 0x55ff3a5c);
            canvas.drawRoundRect(tmp, 22, 22, p);
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT); tp.setTextSize(18); tp.setColor(Color.WHITE);
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout((ok ? "✓  " : "✗  ") + msg, tp, 600, Layout.Alignment.ALIGN_CENTER, 1.05f, 0, false);
            canvas.save();
            canvas.translate(640 - 300, tmp.centerY() - l.getHeight() / 2f);
            l.draw(canvas);
            canvas.restore();
        }

        /* the crash card / the last log: title, the last 40 lines (monospace), the four buttons in a row */
        void drawLog() {
            boolean crash = screen == CRASH;
            bigTitle(getString(crash ? R.string.crash_title : R.string.log_title), crash ? 44 : 50, 58);
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT); tp.setTextSize(15); tp.setColor(SUB);
            if (crash) wrapped(getString(R.string.crash_hint), 70);
            tmp.set(40, 110, 1240, 620);
            p.setStyle(Paint.Style.FILL); p.setColor(0xb0000000);
            canvas.drawRoundRect(tmp, 16, 16, p);
            p.setTypeface(Typeface.MONOSPACE); p.setTextSize(11.5f); p.setTextAlign(Paint.Align.LEFT); p.setLetterSpacing(0);
            canvas.save();
            canvas.clipRect(tmp.left + 8, tmp.top + 4, tmp.right - 8, tmp.bottom - 4);
            if (logLines.isEmpty()) { p.setColor(Color.WHITE); canvas.drawText(getString(R.string.log_none), 56, 140, p); }
            float ly = 126;
            for (String l : logLines) {
                boolean bad = l.contains("FATAL") || l.contains("FAILED") || l.contains("signal") || l.contains("EXCEPTION") || l.contains("blocked");
                p.setColor(bad ? 0xffff8a8a : 0xffe8f0e0);
                canvas.drawText(l, 52, ly, p);
                ly += 12.4f;
            }
            canvas.restore();
            float bw = 270, gap = 16, x = 640 - (items.size() * bw + (items.size() - 1) * gap) / 2;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(x, 640, x + bw, 692);
                pill(it.r, f ? ACCENT : CREAM, f ? ACCENT_SH : CREAM_SH, 4, f);
                text(nunito, 20, f ? ACCENT_TEXT : BG);
                canvas.drawText(it.label, it.r.centerX(), midBase(it.r), p);
                x += bw + gap;
            }
        }

        void drawDisc() {
            bigTitle(getString(R.string.disc), 60, 92);
            File disc = discFile();
            boolean have = dataReady() || disc != null;
            boolean ok = have && prefs.getBoolean("disc_ok", false);
            String head = getString(!have ? R.string.disc_missing : ok ? R.string.disc_ok : R.string.disc_unverified);
            tmp.set(640 - 300, 124, 640 + 300, 164);
            p.setColor(CHIP); p.setStyle(Paint.Style.FILL);
            canvas.drawRoundRect(tmp, 20, 20, p);
            text(nunito, 19, ok ? SUB : ACCENT);
            canvas.drawText((ok ? "✓  " : "✗  ") + head, 640, midBase(tmp), p);
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT);
            tp.setTextSize(16); tp.setColor(Color.WHITE);
            if (dataReady()) wrapped(getString(R.string.disc_data, prefs.getString("disc_name", "?"), prefs.getString("disc_info", "")), 172);
            else if (disc != null) wrapped(getString(R.string.disc_path, disc.getAbsolutePath()), 178);
            tp.setColor(SUB);
            wrapped(getString(R.string.disc_howto), 222);
            int ph = phase;
            if (ph != IDLE) { progressBar(ph, 300); postInvalidateDelayed(100); }   // ~10 Hz while busy
            else if (result != 0 && importStatus != null) resultBox(result > 0, importStatus, 300);
            else if (importStatus != null) { tp.setColor(Color.WHITE); tp.setTextSize(18); wrapped(importStatus, 330); }
            if (confirmWipe) { tp.setColor(ACCENT); tp.setTextSize(18); wrapped(getString(R.string.disc_wipe_confirm), 300); }
            float y = items.size() > 2 ? 384 : 400, bh = items.size() > 2 ? 52 : 58;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 260, y, 640 + 260, y + bh);
                pill(it.r, f ? Color.WHITE : CREAM, CREAM_SH, 5, f);
                text(nunito, 22, BG);
                if (it.value == null) canvas.drawText(it.label, 640, midBase(it.r), p);
                else {
                    p.setTextAlign(Paint.Align.LEFT); canvas.drawText(it.label, it.r.left + 26, midBase(it.r), p);
                    p.setTextAlign(Paint.Align.RIGHT); canvas.drawText("‹  " + it.value + "  ›", it.r.right - 26, midBase(it.r), p);
                }
                y += bh + 5 + 14;
            }
        }
    }
}

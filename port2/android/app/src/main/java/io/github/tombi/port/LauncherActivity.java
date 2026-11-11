package io.github.tombi.port;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.content.res.Resources;
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
 * The launcher shown before the game ("Selva" design, docs/PORT.md "4. Android"): the game library as its home (one card
 * per disc added, Library.java; three looks: carousel, list, cover), options (persisted in SharedPreferences, turned
 * into port flags for TombiActivity, per game version), the selected game's disc screen with the image picker (SAF), quit.
 * Drawn on a Canvas in a 1280x720 virtual space scaled to the screen; gamepad (D-pad/A/B/Start) and touch.
 * Options > Controls / Button mapping: the 14 PS1 buttons and the panel's actions, each bound by pressing the controller
 * button / trigger / stick (PadMap; saved as "padmap", handed to TombiActivity as EXTRA_PADMAP).
 * Fonts (OFL, assets/fonts) are fetched by the CI build; without them the system bold face is used.
 */
public class LauncherActivity extends Activity {
    static final int BG = 0xff2f6b3a, CIRCLE_BL = 0xff275a31, CIRCLE_TR = 0xff3a7c45, ACCENT = 0xffff6aa8,
            ACCENT_SH = 0xff7a2e00, ACCENT_TEXT = 0xff4a1530, CREAM = 0xfffff8e6, CREAM_SH = 0xff1d4424,
            SUB = 0xffffe8a8, CHIP = 0x40000000;
    static final int[] LANG_N = {0, 5, 6, 7, 8, 1, 2, 3, 4};   // --lang; appended, stored indices keep their meaning
    static final String[] LANG_NAME = {"English", "Español", "Português", "Svenska", "Dansk", "Français", "Deutsch", "Italiano", "Nederlands"};
    static final String[] ASPECTS = {"auto", "4:3", "16:9", "21:9"};
    static final String[] PROMPTS = {"auto", "playstation", "xbox", "nintendo", "generic", "thor", "thor-black", "thor-white", "custom"};   // button icons (--prompts); new ones appended: stored indices keep their meaning; custom last (pcSet)
    static final int REQ_DISC = 2, REQ_EXPORT = 3, REQ_SAVE_IMPORT = 4, REQ_SAVE_EXPORT = 5, REQ_TRANSLATION = 6, REQ_WINGS_JP = 7, REQ_WINGS_P9 = 8;
    static final String MAIN0_SHA1 = "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc";   // EXE/MAIN0.EXE of SCES-01331
    static final String REV0_SHA1 = "118459cef9b3747f57b13a498c475ffb8ab512ec";   // the July 1998 Rev 0 of SCES-01331 / SCES-01330: other overlays, not supported
    static final String TEXT_PATCH = "es_mejorada.tombipatch.json";

    SharedPreferences prefs;
    Library lib;          // the game library (Library.java): the discs added, the selected one
    LauncherView view;
    SelvaDialog dlg;
    volatile String importStatus;
    /** the Disc screen shows the add flow (a disc being picked / added: its file name, its progress and its error), not
     *  the selected game; addName = the picked file; addFromLib = opened by "Add game" (a cancelled picker goes back) */
    volatile boolean adding, addFromLib;
    volatile String addName;

    /* the launcher's own language (Options > Game > Launcher language), independent of the game language: "" = the
     * system's, else one of the translations in res/values-xx.  Applied without a restart: getResources() hands out
     * resources for that locale (every getString / getStringArray / Toast / dialog of this activity goes through it)
     * and the view rebuilds its rows on the next draw */
    static final String[] UI_LANGS = {"", "en", "es", "fr", "de", "it", "nl", "pt", "sv", "da"};
    static final String[] UI_LANG_NAME = {null, "English", "Español", "Français", "Deutsch", "Italiano", "Nederlands", "Português", "Svenska", "Dansk"};
    Resources uiRes;

    int uiLangIdx() { return prefs == null ? 0 : clamp(prefs.getInt("uilang", 0), UI_LANGS.length); }
    String uiLangName(int i) { return i == 0 ? getString(R.string.uilang_auto) : UI_LANG_NAME[i]; }
    void applyUiLang() {
        String c = UI_LANGS[uiLangIdx()];
        if (c.isEmpty()) { uiRes = null; return; }
        Configuration cfg = new Configuration(super.getResources().getConfiguration());
        cfg.setLocale(new Locale(c));
        uiRes = createConfigurationContext(cfg).getResources();
    }
    @Override
    public Resources getResources() { Resources r = uiRes; return r != null ? r : super.getResources(); }
    @Override
    public void onConfigurationChanged(Configuration c) {   // orientation / screen size (configChanges): rebuild for the new metrics
        super.onConfigurationChanged(c);
        if (uiRes != null) applyUiLang();
    }

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
        lib = new Library(this, prefs);
        try { lib.migrate(); } catch (Throwable t) { CrashLog.append(this, "launcher: library migration: " + CrashLog.stack(t)); }
        applyUiLang();
        view = new LauncherView(this);
        dlg = new SelvaDialog(this);   // the launcher's dialogs, drawn over the menus (no system AlertDialog)
        android.widget.FrameLayout root = new android.widget.FrameLayout(this);
        root.addView(view, new android.widget.FrameLayout.LayoutParams(-1, -1));
        root.addView(dlg, new android.widget.FrameLayout.LayoutParams(-1, -1));
        setContentView(root);
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
    static final int[] RUMBLE = { 0, 25, 50, 75, 100, 125, 150, 200 };   // --rumble steps, %
    int rumbleIdx() { return clamp(prefs.getInt("rumble", 4), RUMBLE.length); }
    String rumbleName(int i) { return RUMBLE[i] == 0 ? getString(R.string.rumble_off) : RUMBLE[i] + " %"; }
    // --fps (shims/hfr.inc): presents per second, the in-between ones interpolated; the game keeps its 50 Hz speed.
    // "off" = one frame per vblank (the default); the old "fps60" switch counts as auto
    static final String[] FPS = { "off", "auto", "60", "90", "120", "144", "unlimited" };
    int fpsIdx() { return clamp(prefs.getInt("fps", flag("fps60", false) ? 1 : 0), FPS.length); }
    String fpsName(int i) { return i == 0 ? "50 (PAL)" : i == 1 ? "Auto" : i == FPS.length - 1 ? getString(R.string.fps_unlimited) : FPS[i]; }
    /* a short test buzz at the chosen strength on the device's own vibrator (what the game does at full big motor) */
    void rumbleTest() {
        try {
            int amp = Math.min(255, 255 * RUMBLE[rumbleIdx()] / 100);
            if (amp <= 0) return;
            android.os.Vibrator v = (android.os.Vibrator) getSystemService(VIBRATOR_SERVICE);
            if (v != null && v.hasVibrator()) TombiActivity.vibrate(v, amp, 250);
        } catch (Throwable ignored) { }
    }
    int promptIdx() { return clamp(prefs.getInt("prompts", 0), PROMPTS.length); }

    // ---- custom button icons: --prompts custom --prompt-colors (shims/prompts.c; tools/prompt_custom.py is the PC copy) ----
    /* per position (south, east, west, north = a b x y) a disc and a letter colour: pc_c<2*pos> / pc_c<2*pos+1>;
       pc_sym: the symbols (Xbox letters, Nintendo / Thor letters, PlayStation shapes) */
    static final String[] PC_SYMS = {"xbox", "nintendo", "ps"};
    static final int[] PC_PRESET_SYM = {1, 1, 1, 0, 1, 2, 0, 1, 1, 1, 0};   // names: R.array.pc_presets
    static final int[][] PC_PRESET_COLS = {
        {0x181818, 0xFFFFFF, 0x181818, 0xFFFFFF, 0x181818, 0xFFFFFF, 0x181818, 0xFFFFFF},   // Thor negro
        {0xECECE8, 0x828282, 0xECECE8, 0x828282, 0xECECE8, 0x828282, 0xECECE8, 0x828282},   // Thor blanco
        {0xF2C230, 0xFFFFFF, 0xC8281E, 0xFFFFFF, 0x2E8B4E, 0xFFFFFF, 0x1E3FB0, 0xFFFFFF},   // Thor arcoíris
        {0x292929, 0x3CB043, 0x292929, 0xE03C31, 0x292929, 0x2F6FD6, 0x292929, 0xF5C518},   // Xbox
        {0x292929, 0xFFEFDE, 0x292929, 0xFFEFDE, 0x292929, 0xFFEFDE, 0x292929, 0xFFEFDE},   // Nintendo
        {0x292929, 0x6B94FF, 0x292929, 0xFF5A52, 0x292929, 0xFF8484, 0x292929, 0x00FF00},   // PlayStation
        {0x1FAE4B, 0xFFFFFF, 0xD8D8D8, 0x404040, 0xE03C31, 0xFFFFFF, 0xD8D8D8, 0x404040},   // GameCube
        {0x5B3F9E, 0xFFFFFF, 0x5B3F9E, 0xFFFFFF, 0xB8A8E0, 0x3A2A6E, 0xB8A8E0, 0x3A2A6E},   // SNES (US)
        {0xF0C020, 0x5A4000, 0xD0302A, 0xFFE0D8, 0x3A9A50, 0xE0FFE0, 0x3050C0, 0xE0E8FF},   // SNES PAL / Super Famicom
        {0xB02828, 0xFFFFFF, 0xB02828, 0xFFFFFF, 0xD8C8A0, 0x7A1E1E, 0xD8C8A0, 0x7A1E1E},   // Famicom
        {0xA8E6CF, 0x3A3A3A, 0xFFB7B2, 0x3A3A3A, 0xC7CEEA, 0x3A3A3A, 0xFFDAC1, 0x3A3A3A},   // Pastel
    };
    static final int[] PC_PALETTE = {0x000000, 0x181818, 0x404040, 0x828282, 0xC8C8C8, 0xFFFFFF, 0xFFEFDE, 0xC8281E,
        0x7A1E1E, 0xF08030, 0xF5C518, 0xF2C230, 0x8CD040, 0x3CB043, 0x2E8B4E, 0x20A0A0, 0x40C8F0, 0x2F6FD6, 0x1E3FB0,
        0x5B3F9E, 0xB8A8E0, 0xFF8484, 0xE040A0, 0x8B5A2B};
    int pcCol(int i) { return prefs.getInt("pc_c" + i, PC_PRESET_COLS[0][i]) & 0xFFFFFF; }
    int pcSym() { return clamp(prefs.getInt("pc_sym", 1), PC_SYMS.length); }
    int pcPreset() { return clamp(prefs.getInt("pc_preset", 0), PC_PRESET_COLS.length); }
    /* any change here selects the custom style ("Iconos de botones" = Personalizado) */
    void pcSet(int i, int rgb) { prefs.edit().putInt("pc_c" + i, rgb & 0xFFFFFF).putInt("prompts", PROMPTS.length - 1).apply(); }
    void pcSetSym(int s) { prefs.edit().putInt("pc_sym", s).putInt("prompts", PROMPTS.length - 1).apply(); }
    void pcApplyPreset(int n) {
        SharedPreferences.Editor e = prefs.edit().putInt("pc_preset", n).putInt("pc_sym", PC_PRESET_SYM[n]).putInt("prompts", PROMPTS.length - 1);
        for (int i = 0; i < 8; i++) e.putInt("pc_c" + i, PC_PRESET_COLS[n][i]);
        e.apply();
    }
    String pcArg() {
        StringBuilder b = new StringBuilder();
        for (int pos = 0; pos < 4; pos++)
            b.append("abxy".charAt(pos)).append('=').append(String.format(java.util.Locale.ROOT, "%06X:%06X", pcCol(2 * pos), pcCol(2 * pos + 1))).append(',');
        return b.append("sym=").append(PC_SYMS[pcSym()]).toString();
    }
    static final String[] PC_DISC = {"...BCDDCB...", ".EC666666CE.", ".C66666666C.", "B6666666666B", "C6666666666C", "D6666666666D",
        "D6666666666D", "C6666666666C", "B6666666666B", ".C66666666C.", ".EC666666CE.", "...BCDDCB..."};
    static final int[][] PC_FONT6 = {{0x0C, 0x12, 0x21, 0x21, 0x3F, 0x21, 0x21, 0x21}, {0x3E, 0x21, 0x21, 0x3E, 0x21, 0x21, 0x21, 0x3E},
        {0x21, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x33, 0x21}, {0x21, 0x33, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C}};   // A B X Y
    static int pcMix(int a, int b, int t) {
        int o = 0;
        for (int k = 0; k < 24; k += 8) { int x = a >> k & 255, y = b >> k & 255; o |= (x + (y - x) * t / 100) << k; }
        return o;
    }
    static int pcQ(int c) {   // 5 bits per channel, expanded as the port GPU does
        int o = 0;
        for (int k = 0; k < 24; k += 8) { int v = c >> k & 255; o |= ((v & 248) | (v >> 5)) << k; }
        return 0xFF000000 | o;
    }
    /** the 12 x 16 glyph of position pos as the game draws it (prompts.c draw(), PROMPT_CUSTOM): ARGB, 0 = transparent */
    static int[] pcGlyph(int pos, int sym, int disc, int letter) {
        char[] k = new char[16 * 12];   // '.', 'F' fill, 'S' shadow, 'Z' soft edge, or a rim letter B C D E
        boolean[] m = new boolean[16 * 12];
        java.util.Arrays.fill(k, '.');
        for (int y = 0; y < 12; y++) for (int x = 0; x < 12; x++) { char c = PC_DISC[y].charAt(x); k[(y + 2) * 12 + x] = c == '6' ? 'F' : c; }
        if (sym == 2) {
            if (pos == 0) for (int i = 0; i < 6; i++) { m[(5 + i) * 12 + 3 + i] = true; m[(5 + i) * 12 + 8 - i] = true; }
            else if (pos == 1) { int[] ring = {0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C};
                for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) if ((ring[y] >> (7 - x) & 1) != 0) m[(4 + y) * 12 + 2 + x] = true; }
            else if (pos == 2) for (int i = 0; i < 6; i++) { m[5 * 12 + 3 + i] = m[10 * 12 + 3 + i] = m[(5 + i) * 12 + 3] = m[(5 + i) * 12 + 8] = true; }
            else { for (int y = 0; y <= 6; y++) { m[(4 + y) * 12 + 5 - y / 2] = true; m[(4 + y) * 12 + 6 + y / 2] = true; }
                for (int x = 2; x <= 9; x++) m[10 * 12 + x] = true; }
        } else {
            String order = sym == 0 ? "ABXY" : "BAYX";
            int ch = "ABXY".indexOf(order.charAt(pos));
            for (int y = 0; y < 8; y++) for (int x = 0; x < 6; x++) if ((PC_FONT6[ch][y] >> (5 - x) & 1) != 0) m[(4 + y) * 12 + 3 + x] = true;
        }
        for (int y = 14; y >= 0; y--) for (int x = 10; x >= 0; x--)
            if (m[y * 12 + x] && !m[(y + 1) * 12 + x + 1] && k[(y + 1) * 12 + x + 1] == 'F') k[(y + 1) * 12 + x + 1] = 'S';
        for (int y = 0; y < 16; y++) for (int x = 0; x < 12; x++) {
            if (k[y * 12 + x] != 'F' || m[y * 12 + x]) continue;
            if ((x > 0 && m[y * 12 + x - 1]) || (x < 11 && m[y * 12 + x + 1]) || (y > 0 && m[(y - 1) * 12 + x]) || (y < 15 && m[(y + 1) * 12 + x]))
                k[y * 12 + x] = 'Z';
        }
        int[] out = new int[16 * 12];
        for (int y = 0; y < 15; y++) for (int x = 0; x < 12; x++) {   // 1 px lower (rows 3..14)
            int i = y * 12 + x, c;
            if (m[i]) c = pcQ(letter);
            else switch (k[i]) {
                case 'F': c = pcQ(disc); break;
                case 'S': c = pcQ(pcMix(disc, 0, 30)); break;
                case 'Z': c = pcQ(pcMix(disc, letter, 35)); break;
                case 'B': c = 0xFFCEC6A5; break; case 'C': c = 0xFFA59C8C; break; case 'D': c = 0xFF8C8473; break;
                case 'E': c = 0xFFFFEFDE; break;
                default: c = 0;
            }
            out[i + 12] = c;
        }
        return out;
    }
    static int pcHsv(int rgb, int dh, int ds, int dv) {
        float[] h = new float[3];
        Color.colorToHSV(0xFF000000 | rgb, h);
        h[0] = (h[0] + dh + 360) % 360; h[1] = Math.max(0, Math.min(1, h[1] + ds / 100f)); h[2] = Math.max(0, Math.min(1, h[2] + dv / 100f));
        return Color.HSVToColor(h) & 0xFFFFFF;
    }
    // 60 Hz (NTSC speed, the speed the game was made for) is the default; 50 Hz is the original PAL speed, slower
    boolean flag(String k, boolean def) { return prefs.getBoolean(k, "hz60".equals(k) || def); }

    // ---- cut content: one switch per item (port/fixes.txt, --cut-content=LIST; the PC launcher's CUT_ITEMS) ----
    static final String[] CUT_ITEMS = {"boatman", "letter", "board", "berries", "bubble", "titles"};
    static final int[] CUT_LABELS = {R.string.cut_boatman, R.string.cut_letter, R.string.cut_board, R.string.cut_berries, R.string.cut_bubble, R.string.cut_titles};
    /** the items switched on; settings saved with the old single switch ("cut") meant every item */
    List<String> cutItems() {
        if (prefs.contains("cut") && !prefs.contains("cut_boatman")) {
            boolean on = flag("cut", false);
            SharedPreferences.Editor e = prefs.edit();
            for (String k : CUT_ITEMS) e.putBoolean("cut_" + k, on);
            e.remove("cut").apply();
        }
        List<String> on = new ArrayList<>();
        for (String k : CUT_ITEMS) if (flag("cut_" + k, false)) on.add(k);
        return on;
    }

    // ---- the Charity Wings glide: the files it needs from the two demo discs, pulled out of the player's images ----
    static final String[][] WINGS_FILES = {{"AREA08/D002.000", "AREA08/D003.GAM", "SYSTEM/A00000.GAM"}, {"TOMBA/A00002.GAM", "TOMBA/AREA01/A007.000", "TOMBA/AREA01/A008.GAM"}};
    static final String[] WINGS_SERIAL = {"SLPM-80165", "PCPX-96093"};
    volatile String[] wingsStatus = new String[2];   // while checking an image (shown as the row's value)
    File wingsDir(int which) { return new File(new File(filesDir(), "wings"), which == 0 ? "jpdemo" : "pre9"); }
    boolean wingsReady(int which) {
        for (String n : WINGS_FILES[which]) if (!new File(wingsDir(which), n).isFile()) return false;
        return true;
    }
    /** the row's value: busy, the checked image's name, or "not set" */
    String wingsValue(int which) {
        if (wingsStatus[which] != null) return wingsStatus[which];
        if (wingsReady(which)) return getString(R.string.wings_ok, prefs.getString("wings_name" + which, "?"), WINGS_FILES[which].length);
        return getString(R.string.wings_notset);
    }
    void pickWings(int which) {
        Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType("*/*");
        try { startActivityForResult(i, which == 0 ? REQ_WINGS_JP : REQ_WINGS_P9); }
        catch (ActivityNotFoundException e) { status(getString(R.string.disc_nosaf)); }
    }
    /** Reads the files WINGS_FILES[which] out of the picked image (.bin/.iso/.chd, inside a .zip/.7z, or a .zip of the
     *  disc's loose files) into files/wings/<disc>/ after checking the disc's serial (SYSTEM.CNF); runs on its own thread. */
    void wingsImport(Uri u, final int which) {
        final String name = displayName(u);
        wingsStatus[which] = getString(R.string.wings_busy);
        if (view != null) view.postInvalidate();
        File tmpDir = new File(getCacheDir(), "wings-import");
        deleteTree(tmpDir);
        tmpDir.mkdirs();
        android.os.ParcelFileDescriptor pfd = null;
        String msg;
        try {
            byte[] head = new byte[DiscArchive.headBytes()]; int hn = 0;
            try (InputStream hin = getContentResolver().openInputStream(u)) {
                int r0;
                while (hin != null && hn < head.length && (r0 = hin.read(head, hn, head.length - hn)) > 0) hn += r0;
            }
            int kind = DiscArchive.kind(head, hn, name);
            CrashLog.append(this, "wings import " + which + ": " + name + ", kind " + kind);
            String path = null, serial = null;
            java.util.Map<String, byte[]> got = null;
            if (kind == DiscArchive.RAR || kind == DiscArchive.NOT_IMAGE) { msg = getString(R.string.disc_notimage, name); throw new Exception(msg); }
            if (kind == DiscArchive.ZIP) {
                DiscArchive.Result zr;
                try (InputStream zin = getContentResolver().openInputStream(u)) {
                    if (zin == null) throw new java.io.FileNotFoundException(name);
                    zr = DiscArchive.unzip(zin, tmpDir, 64L << 20, null);
                }
                if (zr.image != null) path = zr.image.getAbsolutePath();
                else if (zr.looseFiles) {   // the disc's files as they are: take the ones needed straight from the zip
                    try (InputStream zin = getContentResolver().openInputStream(u)) {
                        got = DiscArchive.members(zin, WINGS_FILES[which]);
                    }
                    serial = zr.serial;
                } else { msg = getString(R.string.disc_zip_bad, name, "no disc image"); throw new Exception(msg); }
            } else {
                try {
                    pfd = getContentResolver().openFileDescriptor(u, "r");
                    if (pfd != null && pfd.getStatSize() > 0) path = "/proc/self/fd/" + pfd.getFd();
                } catch (Throwable t) { CrashLog.append(this, "wings import: no file descriptor (" + t + "): copying"); }
                if (path == null) {   // a stream only: copied to the cache
                    long size = sizeOf(u), free = getCacheDir().getUsableSpace();
                    if (free < (size > 0 ? size : 700L << 20) + (64L << 20)) { msg = getString(R.string.disc_space, (size > 0 ? size : 700L << 20) >> 20, free >> 20); throw new Exception(msg); }
                    File t = new File(tmpDir, kind == DiscArchive.SEVENZ ? "archive.7z" : "image.bin");
                    try (InputStream in = getContentResolver().openInputStream(u); FileOutputStream out = new FileOutputStream(t)) {
                        if (in == null) throw new java.io.FileNotFoundException(name);
                        byte[] buf = new byte[1 << 18]; int n;
                        while ((n = in.read(buf)) > 0) out.write(buf, 0, n);
                    }
                    path = t.getAbsolutePath();
                }
                if (kind == DiscArchive.SEVENZ) {
                    if (!nativeOk) throw new Exception("libtombidisc.so missing");
                    File o7 = new File(tmpDir, "7z");
                    o7.mkdirs();
                    String r7 = nativeUn7z(path, o7.getAbsolutePath());
                    CrashLog.append(this, "wings import: 7z: " + r7.replace('\n', ' '));
                    DiscArchive.Result zr = r7.startsWith("ok") ? DiscArchive.fromNative(r7, o7) : null;
                    if (zr == null || zr.image == null) { msg = getString(R.string.wings_7z, name); throw new Exception(msg); }
                    path = zr.image.getAbsolutePath();
                }
            }
            if (got == null) {   // an image: its ISO9660 files by path (disccheck.c)
                if (!nativeOk) throw new Exception("libtombidisc.so missing");
                String[] want = new String[WINGS_FILES[which].length + 1];
                System.arraycopy(WINGS_FILES[which], 0, want, 0, WINGS_FILES[which].length);
                want[want.length - 1] = "SYSTEM.CNF";
                byte[][] rd = nativeReadFiles(path, want);
                if (rd == null) { msg = getString(R.string.disc_notimage, name); throw new Exception(msg); }
                got = new java.util.HashMap<>();
                for (int i = 0; i < WINGS_FILES[which].length; i++) if (rd[i] != null) got.put(WINGS_FILES[which][i], rd[i]);
                byte[] cnf = rd[want.length - 1];
                if (cnf != null) serial = DiscArchive.serialOf(DiscArchive.bootOf(new String(cnf, "ISO-8859-1")));
            }
            List<String> missing = new ArrayList<>();
            for (String n : WINGS_FILES[which]) if (got.get(n) == null) missing.add(n);
            CrashLog.append(this, "wings import " + which + ": serial " + serial + ", missing " + missing);
            if (!WINGS_SERIAL[which].equals(serial)) { msg = getString(R.string.wings_bad, name, WINGS_SERIAL[which], serial == null ? "?" : serial); throw new Exception(msg); }
            if (!missing.isEmpty()) { msg = getString(R.string.wings_missing, name, android.text.TextUtils.join(", ", missing)); throw new Exception(msg); }
            File dir = wingsDir(which);
            deleteTree(dir);
            for (String n : WINGS_FILES[which]) {
                File f = new File(dir, n);
                f.getParentFile().mkdirs();
                try (FileOutputStream o = new FileOutputStream(f)) { o.write(got.get(n)); }
            }
            prefs.edit().putString("wings_name" + which, name).apply();
            msg = null;
        } catch (Exception e) {
            msg = e.getMessage() != null && !e.getMessage().isEmpty() ? e.getMessage() : String.valueOf(e);
            CrashLog.append(this, "wings import " + which + " failed: " + e);
        } finally {
            if (pfd != null) try { pfd.close(); } catch (Exception ignored) { }
            deleteTree(tmpDir);
        }
        wingsStatus[which] = null;
        final String fm = msg;
        runOnUiThread(new Runnable() { public void run() {
            if (fm != null) saveDialog(getString(which == 0 ? R.string.wings_jp : R.string.wings_p9), fm);
            else Toast.makeText(LauncherActivity.this, getString(R.string.wings_ok, name, WINGS_FILES[which].length), Toast.LENGTH_LONG).show();
            if (view != null) view.invalidate();
        } });
    }
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

    // ---- the selected game of the library (Library.java) ----
    Library.Game game() { return lib == null ? null : lib.selected(); }
    Library.Version ver() { Library.Game g = game(); return g == null ? null : g.ver; }
    /** the PAL game (or none yet): every option; another version shows only the options its port has (no languages,
     *  translations, NTSC picture, panel, cut content, debug menu, PAL cheats, save tools) */
    boolean palGame() { Library.Version v = ver(); return v == null || v.pal(); }
    /** the version's game library is in this APK (libtombi_usa.so is only built when the CI has that disc) */
    boolean libPresent(Library.Version v) {
        if (v == null || v.lib == null) return false;
        String d = getApplicationInfo().nativeLibraryDir;
        return d == null || !new File(d).isDirectory() || new File(d, v.lib).isFile();
    }
    File discFile() { Library.Game g = game(); return g == null ? null : g.legacyDisc(); }
    boolean ready() { Library.Game g = game(); return g != null && g.ready() && libPresent(g.ver); }
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

    /** the player's own translation (Options > Data > Import translation: a .tpatch made with tombi-text, docs/PORT.md
        "Your own translation file"); passed first, tombi2 then skips a later text patch that rebuilds the same files */
    File translationFile() { return new File(filesDir(), "translation.tpatch"); }

    String[] buildArgs() {
        List<String> a = new ArrayList<>();
        File disc = discFile();
        Library.Version v = ver();
        if (dataReady()) { a.add("--data"); a.add(dataDir().getAbsolutePath()); }   // extracted on the device
        else if (disc != null) { a.add("--disc"); a.add(disc.getAbsolutePath()); }  // an image copied by an older version
        // the game library and saves folder of the version (loader.c; PAL: libtombi.so and files/saves as before)
        a.add("--game-lib"); a.add(v != null && v.lib != null ? v.lib : "libtombi.so");
        a.add("--save-sub"); a.add(v != null && v.saveSub != null ? v.saveSub : "saves");
        a.add("--files-dir"); a.add(filesDir().getAbsolutePath());   // the loader's files dir (taken out before port_main)
        if (!palGame()) return buildArgsOther(a);
        if (translationFile().isFile()) { a.add("--text-patch"); a.add(translationFile().getAbsolutePath()); }
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
        List<String> cut = cutItems();   // the pick list (--cut-content=LIST; every item = the plain switch)
        if (!cut.isEmpty()) {
            a.add(cut.size() == CUT_ITEMS.length ? "--cut-content" : "--cut-content=" + android.text.TextUtils.join(",", cut));
            // English cut lines: tombi2 picks en_cut_content.tpatch from --patch-dir by itself (as the PC launcher does)
            if (LANG_N[langIdx()] == 0 && (cut.contains("boatman") || cut.contains("letter") || cut.contains("board"))
                    && patchAsset("en_cut_content.tpatch") != null) { a.add("--patch-dir"); a.add(new File(filesDir(), "patches").getAbsolutePath()); }
        }
        // the lost Charity Wings glide (runtime/wings.c): its own switch, the files of the two demo discs (wingsImport)
        if (flag("wings", false) && wingsReady(0) && wingsReady(1)) {
            a.add("--wings-glide"); a.add(wingsDir(0).getAbsolutePath());
            a.add("--wings-pre9"); a.add(wingsDir(1).getAbsolutePath());
        }
        if (flag("debugmenu", false)) a.add("--debug-menu");
        // the panel only with the option on AND a secondary display present now (off: no Presentation, no HUD move)
        boolean panel = secondScreenOn() && hasSecondDisplay();
        if (panel) a.add("--second-screen");
        if (panel && hudBottomOn()) a.add("--hud-bottom");
        if (flag("dualshock", false)) a.add("--dualshock");
        a.add("--rumble"); a.add(String.valueOf(RUMBLE[rumbleIdx()]));   // runtime/sdl.c: vibration intensity
        a.add("--prompts"); a.add(PROMPTS[promptIdx()]);
        if ("custom".equals(PROMPTS[promptIdx()])) { a.add("--prompt-colors"); a.add(pcArg()); }
        if (flag("skipintro", false)) a.add("--skip-intro");
        if (flag("hz60", false)) { a.add("--hz"); a.add("60"); }
        if (fpsIdx() > 0) { a.add("--fps"); a.add(FPS[fpsIdx()]); }
        if (flag("showfps", false)) a.add("--show-fps");
        a.add("--fit");   // the game always fills the screen (aspect kept)
        cheatArgs(a);
        return a.toArray(new String[0]);
    }
    /** the cheats (tools/port_mods.py): the --poke NAME+OFF forms, which each build resolves to its own release's GAME
     *  (shims/psyq.c port_addr_of_name), so they are the same for PAL and USA */
    void cheatArgs(List<String> a) {
        if (flag("inflives", false)) { a.add("--poke-every"); a.add("50:GAME+20:63:1"); }
        if (flag("infhealth", false)) { a.add("--poke-every"); a.add("1:GAME+10:4:1"); }
    }
    /** another version (USA): English only and its own 60 Hz; the language, text, panel, NTSC picture, cut content and
     *  button icons are PAL-only in the port (docs/USA.md "Native port"); the debug menu and the cheats run in both */
    String[] buildArgsOther(List<String> a) {
        a.add("--aspect"); a.add(ASPECTS[aspectIdx()]);
        a.add("--renderer"); a.add(flag("gl", false) ? "gl" : "soft");
        if (flag("dualshock", false)) a.add("--dualshock");
        a.add("--rumble"); a.add(String.valueOf(RUMBLE[rumbleIdx()]));
        if (flag("skipintro", false)) a.add("--skip-intro");
        if (fpsIdx() > 0) { a.add("--fps"); a.add(FPS[fpsIdx()]); }
        if (flag("showfps", false)) a.add("--show-fps");
        a.add("--fit");
        if (flag("debugmenu", false)) a.add("--debug-menu");
        cheatArgs(a);
        return a.toArray(new String[0]);
    }

    /* a ":game" process left from an earlier game (still tearing down, or kept cached by Android) would be reused for the
       new TombiActivity, where the low range is already taken: kill it (same UID) and wait until it is gone */
    void killStaleGame() {
        try {
            android.app.ActivityManager am = (android.app.ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
            String name = getPackageName() + ":game";
            for (int tries = 0; tries < 40; tries++) {
                int pid = 0;
                java.util.List<android.app.ActivityManager.RunningAppProcessInfo> l = am.getRunningAppProcesses();
                if (l != null) for (android.app.ActivityManager.RunningAppProcessInfo p : l) if (name.equals(p.processName)) pid = p.pid;
                if (pid == 0) { if (tries > 0) CrashLog.append(this, "launcher: stale :game process gone"); return; }
                if (tries == 0) CrashLog.append(this, "launcher: a :game process (pid " + pid + ") is still there: killing it");
                android.os.Process.killProcess(pid);
                try { Thread.sleep(50); } catch (InterruptedException ignored) { }
            }
            CrashLog.append(this, "launcher: the stale :game process did not go away in 2 s; starting anyway");
        } catch (Throwable t) { CrashLog.append(this, "launcher: killStaleGame: " + t); }
    }

    void play() {
        Library.Game g = game();
        if (g != null && g.ver != null && (!g.ver.supported() || !libPresent(g.ver))) {
            saveDialog(versionName(g.ver), getString(g.ver.supported() ? R.string.lib_nolib_long : R.string.lib_unsupported_long, versionName(g.ver)));
            return;
        }
        if (!ready()) {
            if (g == null) { view.setScreen(LauncherView.DISC); addFromLib = true; pickDisc(); return; }   // (the progress is drawn there)
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
        CrashLog.append(this, "launcher: playing " + g.serial + " (" + (g.ver != null ? g.ver.key : "?") + ")");
        killStaleGame();
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

    /** "Tomba! (USA)" in the launcher's language */
    String versionName(Library.Version v) {
        if (v == null) return "?";
        return v.title + " (" + getResources().getStringArray(R.array.lib_regions)[v.region] + ")";
    }

    String version() {
        try { return "v" + getPackageManager().getPackageInfo(getPackageName(), 0).versionName; }
        catch (Exception e) { return "v?"; }
    }

    // ---- SAF disc picker ----
    void pickDisc() {
        adding = true; addName = null; importStatus = null; result = 0;
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
    static final int IDLE = 0, COPYING = 1, CHECKING = 2, EXTRACTING = 3, UNZIPPING = 4;
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
    /** "id:serial=SCUS-94236;boot=SCUS_942.36;sha1=<boot exe sha1>;main0=<MAIN0.EXE sha1>" or "err:<reason>"
     *  ("err:noboot": no game executable): which game the image is (Library.Version) */
    static native String nativeIdentify(String path);
    static native int nativeCheckProgress();
    /** extracts the image into outDir for the port's --data mode (manifest.txt last): "ok:<summary>" or "err:<reason>" */
    static native String nativeExtract(String image, String outDir);
    static native int nativeExtractCount();   // files done << 16 | files total
    static native void nativeCancel();
    /** the disc image of a .7z into outDir (port2/android/un7z.c): "ok\n" + "image FILE\tPATH" / "cnf" / "exe NAME SHA1" /
     *  "cue TRACK" / "nested" lines, or "err:<reason>" ("err:space:NEED:FREE", "err:cancelled") */
    static native String nativeUn7z(String archive, String outDir);
    static native long nativeUnpackBytes(int which);   // 0: bytes written, 1: the image's size
    /** files of the image by path ("SYSTEM/OPTSUB00.BIN"), read straight from it (.bin / .iso / .chd): one byte[] per
     *  path, null where missing; null when the image is not readable (Library.logoFromImage) */
    static native byte[][] nativeReadFiles(String image, String[] paths);
    volatile boolean native7z;                         // UNZIPPING a .7z: the progress comes from un7z.c

    void requestCancel() { cancelImport = true; if (nativeOk) try { nativeCancel(); } catch (Throwable ignored) { } }

    long sizeOf(Uri u) {
        try (Cursor c = getContentResolver().query(u, new String[]{android.provider.OpenableColumns.SIZE}, null, null, null)) {
            if (c != null && c.moveToFirst() && !c.isNull(0)) return c.getLong(0);
        } catch (Exception e) { /* unknown */ }
        return -1;
    }

    /** The game's data, extracted on the device (docs/PORT.md "4. Android"): internal files/data with the manifest the
     *  port's --data mode reads; files/data.tmp while extracting (renamed when complete). */
    File dataDir() { Library.Game g = game(); return g != null ? g.data() : new File(getFilesDir(), "data"); }
    boolean dataReady() { Library.Game g = game(); return g != null && g.extracted(); }

    /** The picked .bin/.iso/.chd (a .cue alongside is ignored: the data track is the image) is read in place through
     *  its file descriptor when the provider gives a real file, else copied to the cache first; then MAIN0.EXE's SHA-1,
     *  then every file extracted into files/data.tmp (each size-checked, any unreadable sector fails), renamed to
     *  files/data.  Cancel (B / "Cancel") stops and deletes the partial files. */
    void importDisc(List<Uri> uris) {
        cancelImport = false; result = 0; copied = 0; copyTotal = -1;
        adding = true; addName = null;
        phase = CHECKING;
        status(null);
        File tmpDir = new File(getCacheDir(), "import"), tmp = new File(getFilesDir(), "data.tmp");   // (tmp: the game's data.tmp once known)
        deleteTree(tmpDir);
        List<android.os.ParcelFileDescriptor> fds = new ArrayList<>();
        try {
            Uri img = null; String name = null;
            for (Uri u : uris) {
                String nm = displayName(u);
                if (!nm.toLowerCase(Locale.ROOT).endsWith(".cue") && img == null) { img = u; name = nm; }
            }
            if (img == null) { fail(getString(R.string.disc_needbin)); return; }
            addName = name;
            long size = sizeOf(img);
            CrashLog.append(this, "disc import: " + name + " (" + (size >= 0 ? size + " bytes" : "size unknown") + "), " + uris.size() + " file(s) picked");
            // what it is (DiscArchive.kind: its first bytes): an image, a .zip holding one, a .7z / .rar, something else
            byte[] head = new byte[DiscArchive.headBytes()]; int hn = 0;
            try (InputStream hin = getContentResolver().openInputStream(img)) {
                int r0;
                while (hin != null && hn < head.length && (r0 = hin.read(head, hn, head.length - hn)) > 0) hn += r0;
            }
            int kind = DiscArchive.kind(head, hn, name);
            CrashLog.append(this, "disc import: kind " + kind + " (" + hn + " head bytes)");
            if (kind == DiscArchive.RAR) { fail(getString(R.string.disc_archive, name)); return; }
            if (kind == DiscArchive.NOT_IMAGE) { fail(getString(R.string.disc_notimage, name)); return; }
            if (kind == DiscArchive.CUE) { fail(getString(R.string.disc_needbin)); return; }
            long free = getFilesDir().getUsableSpace(), need = (size > 0 ? size : 700L << 20) + (64L << 20);
            if (kind != DiscArchive.ZIP && free < need) { fail(getString(R.string.disc_space, need >> 20, free >> 20)); return; }
            String path = null, shown = name;
            if (kind == DiscArchive.ZIP) {   // the image inside, written to the cache (the ISO reader needs random access)
                phase = UNZIPPING; copyTotal = size; copied = 0;
                view.postInvalidate();
                DiscArchive.Result zr;
                try (InputStream zin = getContentResolver().openInputStream(img)) {
                    if (zin == null) throw new java.io.FileNotFoundException(name);
                    zr = DiscArchive.unzip(zin, tmpDir, 64L << 20, new DiscArchive.Progress() {
                        public void read(long n) { copied = n; }
                        public boolean cancelled() { return cancelImport; }
                    });
                } catch (DiscArchive.SpaceException x) {
                    fail(getString(R.string.disc_space, x.needMb, x.freeMb)); return;
                } catch (java.util.zip.ZipException x) {
                    CrashLog.append(this, "disc import: zip: " + x);
                    fail(getString(R.string.disc_zip_bad, name, String.valueOf(x.getMessage()))); return;
                }
                CrashLog.append(this, "disc import: zip: " + zr.entries + " entries, image " + zr.entry + " (" + zr.imageBytes + " bytes)"
                    + (zr.looseFiles ? ", loose files: boot " + zr.boot + " " + zr.bootSha1 : "") + (zr.cueMissing != null ? ", cue names " + zr.cueMissing : ""));
                if (zr.image == null) { noImage(zr, name); return; }
                path = zr.image.getAbsolutePath();
                shown = name + " › " + zr.entry.substring(zr.entry.lastIndexOf('/') + 1);
            }
            if (path == null) try {
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
                File t = new File(tmpDir, kind == DiscArchive.SEVENZ ? "archive.7z" : "image" + (low.endsWith(".chd") ? ".chd" : low.endsWith(".iso") ? ".iso" : ".bin"));
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
            if (!nativeOk) { fail(getString(R.string.disc_add_fail, "libtombidisc.so missing")); return; }
            if (kind == DiscArchive.SEVENZ) {   // the image inside the .7z, written to the cache (un7z.c: the LZMA SDK, streamed)
                phase = UNZIPPING; native7z = true; copied = 0; copyTotal = -1;
                view.postInvalidate();
                File o7 = new File(tmpDir, "7z");
                o7.mkdirs();
                String u;
                try { u = nativeUn7z(path, o7.getAbsolutePath()); } finally { native7z = false; }
                CrashLog.append(this, "disc import: 7z: " + u.replace('\n', ' ').replace('\t', ' '));
                if (u.equals("err:cancelled")) throw new InterruptedException();
                if (u.startsWith("err:space:")) {
                    String[] sp = u.split(":");
                    fail(getString(R.string.disc_space, Long.parseLong(sp[2]) >> 20, Long.parseLong(sp[3]) >> 20)); return;
                }
                if (!u.startsWith("ok")) { fail(getString(R.string.disc_zip_bad, name, u.startsWith("err:") ? u.substring(4) : u)); return; }
                DiscArchive.Result zr = DiscArchive.fromNative(u, o7);
                if (zr.image == null) { noImage(zr, name); return; }
                path = zr.image.getAbsolutePath();
                shown = name + " › " + zr.entry.substring(zr.entry.lastIndexOf('/') + 1);
            }
            phase = CHECKING;
            view.postInvalidate();
            // which game (disccheck.c nativeIdentify): the boot executable SYSTEM.CNF names and its SHA-1
            String r = nativeIdentify(path);
            CrashLog.append(this, "disc import: identify: " + r);
            if (!r.startsWith("id:")) {
                fail(r.equals("err:noboot") ? getString(R.string.disc_nomain0)
                     : r.startsWith("err:sector 16") ? getString(R.string.disc_notimage, name)
                     : getString(R.string.disc_add_fail, r.substring(4)));
                return;
            }
            java.util.Map<String, String> id = new java.util.HashMap<>();
            for (String kv : r.substring(3).split(";")) { int e = kv.indexOf('='); if (e > 0) id.put(kv.substring(0, e), kv.substring(e + 1)); }
            String sha = id.get("sha1"), main0 = id.get("main0"), serial = id.get("serial");
            Library.Version v = Library.bySha1(sha);
            if (serial == null || serial.isEmpty()) serial = v != null && v.pal() ? "SCES-01331" : "?";
            if (v == null) {
                if (REV0_SHA1.equals(main0)) { fail(getString(R.string.disc_rev0)); return; }
                fail(getString(R.string.disc_unknown, serial, sha == null ? "?" : sha)); return;
            }
            if (!v.supported()) {   // recognised, no port of it yet (Japan): listed in the library, nothing extracted
                Library.Game jg = lib.put(serial, v, shown, "");
                // its card's logo from the image itself, now (a .zip / .7z's temp image is deleted when this returns)
                try {
                    jg.logoFile().delete();
                    CrashLog.append(this, "disc import: logo from the image: " + (Library.logoFromImage(jg, path) != null));
                } catch (Throwable t) { CrashLog.append(this, "disc import: logo: " + t); }
                adding = false;
                phase = IDLE; result = -1;
                status(getString(R.string.lib_unsupported_long, versionName(v)));
                return;
            }
            if (!libPresent(v)) { fail(getString(R.string.lib_nolib_long, versionName(v))); return; }
            if (cancelImport) throw new InterruptedException();
            phase = EXTRACTING;
            view.postInvalidate();
            // its own folder: files/games/<serial>/data (data.tmp while extracting; renamed when complete)
            File gdir = new File(lib.root(), Library.idOf(serial));
            gdir.mkdirs();
            tmp = new File(gdir, "data.tmp");
            deleteTree(tmp);
            String x = nativeExtract(path, tmp.getAbsolutePath());
            CrashLog.append(this, "disc import: extraction: " + x);
            if (x.equals("err:cancelled")) throw new InterruptedException();
            if (!x.startsWith("ok:")) { deleteTree(tmp); fail(getString(R.string.disc_damaged, x.startsWith("err:") ? x.substring(4) : x)); return; }
            File data = new File(gdir, "data");
            deleteTree(data);
            new File(gdir, "logo.png").delete();
            if (!tmp.renameTo(data)) { deleteTree(tmp); fail(getString(R.string.disc_add_fail, "rename data.tmp")); return; }
            if (v.pal()) deleteTree(new File(filesDir(), "disc"));   // an image copied by an older version: no longer needed
            Library.Game g = lib.put(serial, v, shown, x.substring(3) + (x.endsWith(CONTENT_SHA1) ? " (= reference PAL disc)" : ""));
            try { Library.logo(g); } catch (Throwable t) { CrashLog.append(this, "disc import: logo: " + t); }
            adding = false;
            phase = IDLE;
            result = 1;
            status(getString(R.string.disc_added, versionName(v)));
        } catch (InterruptedException e) {
            deleteTree(tmp);
            phase = IDLE; result = -1;
            status(getString(R.string.disc_cancelled));
            CrashLog.append(this, "disc import: cancelled");
        } catch (Throwable e) {
            deleteTree(tmp);
            CrashLog.append(this, "disc import: " + CrashLog.stack(e));
            fail(getString(R.string.disc_add_fail, String.valueOf(e.getMessage())));
        } finally {
            for (android.os.ParcelFileDescriptor f : fds) try { f.close(); } catch (Throwable ignored) { }
            deleteTree(tmpDir);
        }
    }

    void fail(String msg) { phase = IDLE; result = -1; status(msg); CrashLog.append(this, "disc import failed: " + msg); }
    /** an archive without a disc image: why, naming the archive (and the game, for its loose files) */
    void noImage(DiscArchive.Result zr, String name) {
        if (zr.looseFiles) {
            Library.Version lv = zr.bootSha1 != null ? Library.bySha1(zr.bootSha1) : null;
            String what = lv != null ? versionName(lv) : zr.serial != null ? zr.serial : "?";
            fail(getString(R.string.disc_zip_files, name, what));
        } else if (zr.cueMissing != null) fail(getString(R.string.disc_zip_cue, name, zr.cueMissing));
        else fail(getString(zr.archiveInside ? R.string.disc_zip_nested : R.string.disc_zip_noimage, name));
    }

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
        lib.removeAll();                                   // every game of the library (files/games)
        deleteTree(new File(getFilesDir(), "data"));
        deleteTree(new File(getFilesDir(), "data.tmp"));
        deleteTree(new File(fd, "disc"));
        deleteTree(new File(fd, "patches"));
        File[] cache = getCacheDir().listFiles();
        if (cache != null) for (File c : cache) deleteTree(c);
        if (getExternalCacheDir() != null) { File[] ec = getExternalCacheDir().listFiles(); if (ec != null) for (File c : ec) deleteTree(c); }
        if (saves) for (Library.Version v : Library.VERSIONS) if (v.saveSub != null) deleteTree(new File(fd, v.saveSub));
        CrashLog.clearMarkers(this);
        prefs.edit().remove("disc").remove("disc_ok").remove("disc_info").remove("disc_name").apply();
        CrashLog.append(this, "launcher: data deleted (saves " + (saves ? "deleted" : "kept") + ")");
        result = 1;
        status(getString(saves ? R.string.disc_wiped_saves : R.string.disc_wiped));
    }

    /** Disc screen: "Remove this game": its extracted data and card (the saves stay, files/saves*) */
    void removeGame() {
        Library.Game g = game();
        if (g == null) return;
        String nm = versionName(g.ver) + " · " + g.serial;
        lib.remove(g.id);
        CrashLog.append(this, "launcher: game removed from the library: " + g.serial);
        result = 1;
        status(getString(R.string.lib_removed, nm));
    }
    /** the library's look (Options > Game > Library style): 0 carousel, 1 list, 2 cover */
    int libStyle() { return clamp(prefs.getInt("libstyle", 1), 3); }

    // ---- save import / export (SaveImport.java; the game's single card: files/saves/tombi_mc0.mcd, the USA game's in
    //      files/saves_usa: Library.Version.saveSub, the folder the loader gets as --save-sub) ----
    File cardFile() {
        Library.Version v = ver();
        File d = new File(filesDir(), v != null && v.saveSub != null ? v.saveSub : "saves");
        d.mkdirs();
        return new File(d, "tombi_mc0.mcd");
    }
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
    // ---- translation import (a .tpatch; the .tombipatch.json needs tools/tpatch_build.py on a PC first)
    void pickTranslation() {
        Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType("*/*");
        try { startActivityForResult(i, REQ_TRANSLATION); }
        catch (ActivityNotFoundException e) { Toast.makeText(this, String.valueOf(e.getMessage()), Toast.LENGTH_LONG).show(); }
    }
    void doTranslationImport(Uri u) {
        try {
            InputStream in = getContentResolver().openInputStream(u);
            if (in == null) throw new java.io.IOException("openInputStream");
            byte[] raw;
            try { raw = SaveImport.readAll(in, 32 << 20); } finally { in.close(); }
            if (raw.length < 58 || raw[0] != 'T' || raw[1] != 'P' || raw[2] != 'C' || raw[3] != 'H') {
                saveDialog(getString(R.string.tr_import), getString(raw.length > 0 && raw[0] == '{' ? R.string.tr_json : R.string.tr_bad));
                return;
            }
            File f = translationFile(), tmp = new File(f.getPath() + ".part");
            OutputStream o = new FileOutputStream(tmp);
            try { o.write(raw); } finally { o.close(); }
            if (!tmp.renameTo(f)) throw new java.io.IOException("rename " + tmp);
            int tl = (raw[56] & 255) | (raw[57] & 255) << 8;
            String title = new String(raw, 58, Math.max(0, Math.min(tl, raw.length - 58)), "UTF-8");
            CrashLog.append(this, "launcher: translation imported: " + title);
            saveDialog(getString(R.string.tr_import), getString(R.string.tr_done, title));
            status(getString(R.string.tr_done, title));
        } catch (Exception e) {
            saveDialog(getString(R.string.tr_import), getString(R.string.save_fail, String.valueOf(e.getMessage())));
        }
    }

    /** a dialog in the launcher's style (SelvaDialog): buttons with their actions (null: just close), cancel = the
     *  index B / Back chooses */
    void dialog(String title, String msg, String[] btns, Runnable[] acts, int cancel) {
        dlg.show(title, msg, btns, acts, cancel);
    }
    void saveDialog(String title, String msg) {
        dialog(title, msg, new String[]{"OK"}, new Runnable[]{null}, 0);
    }
    boolean uiSpanish() { return getResources().getConfiguration().locale.getLanguage().equals("es"); }

    /* Import save: a single save whose name is already on the card asks right away (replace it / save it to another
     * slot of the card when it has room and a free slot number / cancel); anything else asks the usual confirmation first */
    void doSaveImport(final Uri u) {
        final byte[] raw;
        try {
            InputStream in = getContentResolver().openInputStream(u);
            if (in == null) throw new java.io.IOException("openInputStream");
            try { raw = SaveImport.readAll(in, 4 << 20); } finally { in.close(); }
        } catch (Exception e) {
            saveDialog(getString(R.string.save_import), getString(R.string.save_fail, String.valueOf(e.getMessage())));
            return;
        }
        SaveImport.Conflict c = null;
        try { c = SaveImport.conflict(raw, cardFile(), uiSpanish()); } catch (Exception ignored) { }
        if (c != null) { askReplace(raw, c); return; }
        dialog(getString(R.string.save_import), getString(R.string.save_import_confirm),
                new String[]{getString(R.string.save_import_go), getString(R.string.cancel)},
                new Runnable[]{new Runnable() { public void run() { runSaveImport(raw, cardFile(), false, false); } }, null}, 1);
    }
    void askReplace(final byte[] raw, final SaveImport.Conflict c) {
        List<String> b = new ArrayList<>();
        List<Runnable> a = new ArrayList<>();
        b.add(getString(R.string.save_replace));
        a.add(new Runnable() { public void run() { runSaveImport(raw, c.dest, true, false); } });
        if (c.newSlot != null) {
            b.add(getString(R.string.save_new_slot));
            a.add(new Runnable() { public void run() { runSaveImport(raw, c.dest, false, true); } });
        }
        b.add(getString(R.string.cancel));
        a.add(null);
        dialog(getString(R.string.save_import), c.question(uiSpanish()), b.toArray(new String[0]), a.toArray(new Runnable[0]), b.size() - 1);
    }
    void runSaveImport(byte[] raw, File dest, boolean overwrite, boolean newSlot) {
        try {
            SaveImport.Result r = SaveImport.importBytes(raw, dest, uiSpanish(), overwrite, newSlot);
            StringBuilder sb = new StringBuilder();
            for (String l : r.lines) sb.append(l).append('\n');
            if (r.warn != null) sb.append('\n').append(r.warn);
            CrashLog.append(this, "launcher: save imported: " + r.lines.get(0));
            saveDialog(getString(R.string.save_import), sb.toString().trim());
        } catch (SaveImport.Conflict c) {
            askReplace(raw, c);
        } catch (Exception e) {
            saveDialog(getString(R.string.save_import), getString(R.string.save_fail, String.valueOf(e.getMessage())));
        }
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
        if (req == REQ_TRANSLATION && res == RESULT_OK && data != null && data.getData() != null) { doTranslationImport(data.getData()); return; }
        if ((req == REQ_WINGS_JP || req == REQ_WINGS_P9) && res == RESULT_OK && data != null && data.getData() != null) {
            final Uri wu = data.getData(); final int which = req == REQ_WINGS_JP ? 0 : 1;
            new Thread(new Runnable() { public void run() { wingsImport(wu, which); } }, "tombi-wings").start();
            return;
        }
        if (req == REQ_DISC && (res != RESULT_OK || data == null) && phase == IDLE && importStatus == null) {   // picker closed
            adding = false;
            if (addFromLib && view != null) view.setScreen(LauncherView.MAIN);
            if (view != null) view.invalidate();
            return;
        }
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
        if (dlg != null && dlg.showing()) return dlg.key(code, e.getRepeatCount());
        if (view.capture >= 0) return view.captureKey(code, e, true);
        return view.key(code) || super.onKeyDown(code, e);
    }
    @Override
    public boolean onKeyUp(int code, KeyEvent e) {
        if (dlg != null && dlg.showing()) return true;
        if (view.capture >= 0 || view.swallowUp == code) return view.captureKey(code, e, false);
        return super.onKeyUp(code, e);
    }
    @Override
    public boolean onGenericMotionEvent(MotionEvent e) {
        if (dlg != null && dlg.showing()) return true;
        if (view.capture >= 0 && view.captureMotion(e)) return true;
        if (e.getActionMasked() == MotionEvent.ACTION_SCROLL && view.wheel(e.getAxisValue(MotionEvent.AXIS_VSCROLL))) return true;
        return super.onGenericMotionEvent(e);
    }

    // ---------------------------------------------------------------------------------------------------------------
    static class Item {
        String label; String value; String sub; RectF r = new RectF(); boolean primary;   // sub: category summary
        Runnable act; Adjust adj;
        int check = -1;   // on/off option: 0/1, drawn as a tick box instead of a value
        int swatch = -1;  // a colour row (custom buttons): its RGB, drawn as a swatch next to the value
        boolean actFirst; // A / tap runs act (open the editor); left / right still adjust
        int game = -1;    // the library: a game card (index in LauncherView.games)
        boolean add, dim; // the library: the "Add game" card; a button that cannot act now (drawn faded)
        Item(String l, Runnable a) { label = l; act = a; }
    }
    interface Adjust { void by(int d); }

    class LauncherView extends View {
        static final int MAIN = 0, OPTIONS = 1, DISC = 2, CRASH = 3, LOG = 4, PADMAP = 5, OPTCAT = 6, CUSTOM = 7, COLOR = 8;
        /* CUSTOM: "Customise buttons" (custom style): preset, symbols, 8 colour rows, live preview; COLOR: one colour
         *  (pcEdit = 2 * position + 0 disc / 1 letter): R G B, H S V, a palette of swatches */
        int pcEdit;
        final RectF[] pcSw = new RectF[PC_PALETTE.length];
        /* Options = a list of categories (optCats); OPTCAT = the page of category `cat`.
         *  discFromOptions = the Disc screen was opened from an Options page (B/Back return there) */
        static final int CAT_GAME = 0, CAT_PAD = 3, CAT_CUT = 6;   // (1, 2, 4, 5: the old Graphics / Second screen / Data / Debug ids)
        int screen = MAIN, focus = 0, cat = CAT_GAME;
        int[] lastFocus = new int[9];
        /* Scrolling list (only the pad-mapping list overflows now; the 1280x720 canvas is scaled to fit any screen,
         *  so Options needs none): listScroll/listMax in canvas units. Keys keep the focused row visible
         *  (followFocus); a drag moves the list, with fling from the release velocity; a touch that moved less than
         *  the slop (8dp) is a tap. */
        float listScroll, listMax;
        boolean followFocus = true, dragging;
        int downFocus = -1;                        // the focused item when the touch went down
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
        List<Library.Game> games = new ArrayList<>();   // the library's cards (build, MAIN)
        boolean mainFocusSet;
        RectF[] drawn = new RectF[0];                   // the items' rectangles of the last frame (spatial navigation)
        final java.util.HashMap<String, android.graphics.Bitmap> logos = new java.util.HashMap<>();
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
            if (phase == IDLE && (sc != DISC || screen != DISC)) { importStatus = null; result = 0; }   // a new visit: no old message
            if (phase == IDLE && sc != DISC) { adding = false; addFromLib = false; }
            if (phase == IDLE && sc == DISC && screen != DISC) { adding = false; addFromLib = false; }
            screen = sc;
            focus = lastFocus[sc];
            listScroll = 0; followFocus = true; scroller.forceFinished(true);
            if (sc == CRASH || sc == LOG) {
                try { logLines = CrashLog.tail(LauncherActivity.this, 40); } catch (Throwable t) { logLines = new ArrayList<>(); }
                focus = 0;
            }
            invalidate();
        }

        String onoff(boolean b) { return getString(b ? R.string.on : R.string.off); }

        Item libStyleItem() {
            final String[] st = getResources().getStringArray(R.array.lib_styles);
            Item it = new Item(getString(R.string.lib_style), null);
            it.value = st[libStyle()];
            it.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("libstyle", (libStyle() + d + 3) % 3).apply(); } };
            return it;
        }

        /** the library: focus on a card selects that game */
        void focusMoved() {
            if (screen != MAIN || focus < 0 || focus >= items.size()) return;
            Item it = items.get(focus);
            if (it.game >= 0 && it.game < games.size()) lib.select(games.get(it.game).id);
        }
        /** the library: the item nearest in direction (dx, dy) from the focused one, by the drawn rectangles; -1 none */
        int spatial(int dx, int dy) {
            if (drawn.length != items.size() || focus >= drawn.length) return -1;
            RectF c = drawn[focus];
            int best = -1; float bd = Float.MAX_VALUE;
            for (int i = 0; i < drawn.length; i++) {
                if (i == focus || drawn[i].isEmpty()) continue;
                RectF r = drawn[i];
                float vx = r.centerX() - c.centerX(), vy = r.centerY() - c.centerY();
                float along = vx * dx + vy * dy, across = Math.abs(vx * dy) + Math.abs(vy * dx);
                // in front, and overlapping the row / column more than off to the side
                boolean overlap = dx != 0 ? r.bottom > c.top && r.top < c.bottom : r.right > c.left && r.left < c.right;
                if (along <= 1) continue;
                float d = along + across * (overlap ? 0.5f : 3f);
                if (d < bd) { bd = d; best = i; }
            }
            return best;
        }
        android.graphics.Bitmap logoOf(Library.Game g) {
            if (g == null) return null;
            // (a card without one is asked again once its logo.png exists: the disc added again while the launcher runs)
            if (!logos.containsKey(g.id) || logos.get(g.id) == null && g.logoFile().isFile()) {
                android.graphics.Bitmap b = null;
                try { b = Library.logo(g); } catch (Throwable t) { CrashLog.append(LauncherActivity.this, "launcher: logo " + g.id + ": " + t); }
                logos.put(g.id, b);
            }
            return logos.get(g.id);
        }

        Item toggle(int label, final String key) {
            final Item it = new Item(getString(label), null);
            it.check = flag(key, false) ? 1 : 0;
            it.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean(key, !flag(key, false)).apply(); } };
            return it;
        }

        void build() {
            items.clear();
            if (screen == MAIN) {   // the library (drawLibrary): the game cards, "Add game", then the buttons
                games = lib.games();
                for (int i = 0; i < games.size(); i++) {
                    final Library.Game g = games.get(i);
                    Item it = new Item(versionName(g.ver), new Runnable() { public void run() { lib.select(g.id); play(); } });
                    it.game = i;
                    items.add(it);
                }
                Item ad = new Item(getString(R.string.lib_add), new Runnable() { public void run() { setScreen(DISC); addFromLib = true; pickDisc(); } });
                ad.add = true;
                items.add(ad);
                Item a = new Item(getString(R.string.play), new Runnable() { public void run() { play(); } });
                a.primary = true; a.dim = !ready(); items.add(a);
                items.add(new Item(getString(R.string.options), new Runnable() { public void run() { setScreen(OPTIONS); } }));
                items.add(new Item(getString(R.string.quit), new Runnable() { public void run() { finish(); } }));
                if (!mainFocusSet) { mainFocusSet = true; focus = games.size() + 1; }   // first time: on Play
            } else if (screen == OPTIONS) {   // category list, one column (drawOptions): optCats
                for (int c : optCats()) category(c, catName(c), catSummary(c));
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(MAIN); } }));
            } else if (screen == OPTCAT) {   // one category page, same row style (drawOptions): its groups (optGroups)
                for (String g : optGroups(cat)) group(g);
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(OPTIONS); } }));
            } else if (screen == PADMAP) {
                PadMap pm = new PadMap(prefs.getString("padmap", ""));
                String[] names = getResources().getStringArray(R.array.pad_names);
                for (int t = 0; t < PadMap.NAMES.length; t++) {
                    final int target = t;
                    Item it = new Item(t < names.length ? names[t] : PadMap.NAMES[t], new Runnable() { public void run() { startCapture(target); } });
                    it.value = pm.describe(t, padWords());
                    items.add(it);
                }
                items.add(new Item(getString(R.string.pad_reset), new Runnable() { public void run() {
                    prefs.edit().remove("padmap").apply(); padStatus = getString(R.string.pad_reset_done); } }));
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(OPTCAT); } }));
            } else if (screen == CUSTOM) {
                buildCustom();
            } else if (screen == COLOR) {
                buildColor();
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
            } else if (adding) {   // the add flow: pick (again) or back
                items.add(new Item(getString(R.string.disc_pick), new Runnable() { public void run() { pickDisc(); } }));
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(discFromOptions ? OPTCAT : MAIN); } }));
            } else {
                items.add(new Item(getString(R.string.disc_pick), new Runnable() { public void run() { pickDisc(); } }));
                if (game() != null) items.add(new Item(getString(R.string.lib_remove), new Runnable() { public void run() { removeGame(); } }));
                items.add(new Item(getString(R.string.disc_wipe), new Runnable() { public void run() { confirmWipe = true; wipeSaves = false; focus = 0; } }));
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(discFromOptions ? OPTCAT : MAIN); } }));
            }
            if (focus >= items.size()) focus = 0;
        }

        void buildCustom() {
            final String[] presets = getResources().getStringArray(R.array.pc_presets);
            Item pr = new Item(getString(R.string.pc_preset), null);
            pr.value = presets[pcPreset()];
            pr.adj = new Adjust() { public void by(int d) { pcApplyPreset((pcPreset() + d + presets.length) % presets.length); } };
            items.add(pr);
            final String[] syms = getResources().getStringArray(R.array.pc_syms);
            Item sy = new Item(getString(R.string.pc_sym), null);
            sy.value = syms[pcSym()];
            sy.adj = new Adjust() { public void by(int d) { pcSetSym((pcSym() + d + syms.length) % syms.length); } };
            items.add(sy);
            String[] pos = getResources().getStringArray(R.array.pc_pos);
            for (int i = 0; i < 8; i++) {
                final int idx = i;
                Item c = new Item(pos[i / 2] + " · " + getString(i % 2 == 0 ? R.string.pc_disc : R.string.pc_letter),
                        new Runnable() { public void run() { pcEdit = idx; setScreen(COLOR); focus = 0; } });
                c.swatch = pcCol(i);
                c.value = String.format(java.util.Locale.ROOT, "#%06X", c.swatch);
                c.actFirst = true;
                c.adj = new Adjust() { public void by(int d) {   // steps through the palette from the nearest swatch
                    int cur = pcCol(idx), best = 0; long bd = Long.MAX_VALUE;
                    for (int k = 0; k < PC_PALETTE.length; k++) {
                        int q = PC_PALETTE[k]; long dd = 0;
                        for (int s = 0; s < 24; s += 8) { long e = (cur >> s & 255) - (q >> s & 255); dd += e * e; }
                        if (dd < bd) { bd = dd; best = k; }
                    }
                    int n = bd == 0 ? (best + d + PC_PALETTE.length) % PC_PALETTE.length : best;
                    pcSet(idx, PC_PALETTE[n]); } };
                items.add(c);
            }
            items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(OPTCAT); } }));
        }

        void buildColor() {
            final int idx = pcEdit;
            final int c = pcCol(idx);
            String[] names = getResources().getStringArray(R.array.pc_channels);   // R G B H S V
            for (int ch = 0; ch < 6; ch++) {
                final int k = ch;
                Item it = new Item(names[ch], null);
                if (ch < 3) it.value = String.valueOf(c >> (16 - 8 * ch) & 255);
                else { float[] h = new float[3]; Color.colorToHSV(0xFF000000 | c, h);
                    it.value = ch == 3 ? Math.round(h[0]) + "°" : Math.round(h[ch - 3] * 100) + " %"; }
                it.adj = new Adjust() { public void by(int d) {
                    int v = pcCol(idx);
                    if (k < 3) { int sh = 16 - 8 * k, x = Math.max(0, Math.min(255, (v >> sh & 255) + d * 8)); v = (v & ~(255 << sh)) | x << sh; }
                    else v = pcHsv(v, k == 3 ? d * 10 : 0, k == 4 ? d * 5 : 0, k == 5 ? d * 5 : 0);
                    pcSet(idx, v); } };
                items.add(it);
            }
            Item pal = new Item(getString(R.string.pc_palette), null);
            pal.swatch = c;
            pal.value = String.format(java.util.Locale.ROOT, "#%06X", c);
            pal.adj = new Adjust() { public void by(int d) {
                int cur = pcCol(idx), n = 0;
                for (int k = 0; k < PC_PALETTE.length; k++) if (PC_PALETTE[k] == cur) n = (k + d + PC_PALETTE.length) % PC_PALETTE.length;
                pcSet(idx, PC_PALETTE[n]); } };
            items.add(pal);
            items.add(new Item(getString(R.string.pc_done), new Runnable() { public void run() { setScreen(CUSTOM); } }));
        }

        /* leaving the crash card clears the markers (the log stays: Options > View last log) */
        void closeLog() {
            if (screen == CRASH) { CrashLog.clearMarkers(LauncherActivity.this); setScreen(MAIN); }
            else setScreen(OPTCAT);
        }

        /** a row of the Options list: opens page c (focus on its first row) */
        // ---- the options, as groups of rows (one method per group) in six categories by topic ----
        static final int CAT_LAUNCHDBG = 7, CAT_IMAGE = 8, CAT_DISC = 9;
        /* Game (language, translation, intro, cheats), Cut content, Picture and screen (graphics + second screen),
           Controls, Disc and saves, Launcher and debug (launcher language, library style, debug menu, log) */
        int[] optCats() {
            return palGame() ? new int[]{CAT_GAME, CAT_CUT, CAT_IMAGE, CAT_PAD, CAT_DISC, CAT_LAUNCHDBG}
                             : new int[]{CAT_GAME, CAT_IMAGE, CAT_PAD, CAT_DISC, CAT_LAUNCHDBG};
        }
        String[] optGroups(int c) {
            switch (c) {
                case CAT_GAME: return new String[]{"lang", "textpatch", "translation", "skipintro", "cheats"};
                case CAT_CUT: return new String[]{"cut"};
                case CAT_IMAGE: return new String[]{"gfx", "dual"};
                case CAT_PAD: return new String[]{"pad"};
                case CAT_DISC: return new String[]{"saves", "disc"};
                default: return new String[]{"uilang", "libstyle", "debugmenu", "viewlog"};   // CAT_LAUNCHDBG
            }
        }
        int catName(int c) {
            switch (c) {
                case CAT_GAME: return R.string.cat_game; case CAT_CUT: return R.string.cat_cut; case CAT_IMAGE: return R.string.cat_image;
                case CAT_PAD: return R.string.cat_pad; case CAT_DISC: return R.string.cat_disc; default: return R.string.cat_launchdbg;
            }
        }
        String catSummary(int c) {
            switch (c) {
                case CAT_GAME: return palGame() ? LANG_NAME[langIdx()] : "English";
                case CAT_CUT: return cutItems().size() + "/" + CUT_ITEMS.length + (flag("wings", false) ? " · " + getString(R.string.opt_wings_short) : "");
                case CAT_IMAGE: return aspectName(aspectIdx()) + " · " + getString(flag("gl", false) ? R.string.renderer_gl_short : R.string.renderer_soft)
                    + (palGame() && secondScreenOn() ? " · " + getString(R.string.cat_dual) : "");
                case CAT_PAD: return getResources().getStringArray(R.array.prompt_names)[promptIdx()];
                case CAT_LAUNCHDBG: return uiLangName(uiLangIdx());
                default: return null;
            }
        }
        /** the rows of one group (the PAL-only ones left out for another version: docs/USA.md "Native port") */
        void group(String g) {
            boolean pal = palGame();
            if (g.equals("lang") && pal) {
                Item l = new Item(getString(R.string.opt_lang), null);
                l.value = LANG_NAME[langIdx()];
                l.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("lang", (langIdx() + d + LANG_N.length) % LANG_N.length).apply(); } };
                items.add(l);
            } else if (g.equals("uilang")) {   // the launcher's own language (applyUiLang)
                Item ul = new Item(getString(R.string.opt_uilang), null);
                ul.value = uiLangName(uiLangIdx());
                ul.adj = new Adjust() { public void by(int d) {
                    prefs.edit().putInt("uilang", (uiLangIdx() + d + UI_LANGS.length) % UI_LANGS.length).apply();
                    applyUiLang(); } };
                items.add(ul);
            } else if (g.equals("textpatch") && pal) {
                Item tpi = new Item(getString(R.string.opt_textpatch), null);
                tpi.check = textPatchOn() ? 1 : 0;
                tpi.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("textpatch", !textPatchOn()).apply(); } };
                items.add(tpi);
            } else if (g.equals("translation") && pal) {   // (translations: PAL only for now)
                items.add(new Item(getString(R.string.tr_import), new Runnable() { public void run() { pickTranslation(); } }));
                if (translationFile().isFile())
                    items.add(new Item(getString(R.string.tr_remove), new Runnable() { public void run() {
                        translationFile().delete(); status(getString(R.string.tr_removed)); setScreen(screen); } }));
            } else if (g.equals("skipintro")) {
                items.add(toggle(R.string.opt_skipintro, "skipintro"));
            } else if (g.equals("libstyle")) {
                items.add(libStyleItem());
            } else if (g.equals("cut") && pal) {   // one switch per item (cutItems), then the lost wings glide and its two discs
                cutItems();
                for (int k = 0; k < CUT_ITEMS.length; k++) items.add(toggle(CUT_LABELS[k], "cut_" + CUT_ITEMS[k]));
                items.add(toggle(R.string.opt_wings, "wings"));
                for (int w = 0; w < 2; w++) {
                    final int which = w;
                    Item wi = new Item(getString(w == 0 ? R.string.wings_jp : R.string.wings_p9), new Runnable() { public void run() {
                        if (wingsStatus[which] == null) pickWings(which); } });
                    wi.sub = wingsValue(w);   // (an action row with its status on the right, like a category's summary)
                    items.add(wi);
                }
            } else if (g.equals("gfx")) {
                Item as = new Item(getString(R.string.opt_aspect), null);
                as.value = aspectName(aspectIdx());
                as.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("aspect", (aspectIdx() + d + ASPECTS.length) % ASPECTS.length).apply(); } };
                items.add(as);
                Item r = new Item(getString(R.string.opt_renderer), null);
                r.value = getString(flag("gl", false) ? R.string.renderer_gl : R.string.renderer_soft);
                r.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("gl", !flag("gl", false)).apply(); } };
                items.add(r);
                if (pal) items.add(toggle(R.string.opt_hz60, "hz60"));   // (USA: 60 Hz is its own speed)
                Item fp = new Item(getString(R.string.opt_fps), null);
                fp.value = fpsName(fpsIdx());
                fp.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("fps", (fpsIdx() + d + FPS.length) % FPS.length).apply(); } };
                items.add(fp);
                items.add(toggle(R.string.opt_showfps, "showfps"));
            } else if (g.equals("dual") && pal) {   // (the panel is PAL-only)
                Item ss = new Item(getString(R.string.opt_secondscreen), null);
                ss.check = secondScreenOn() ? 1 : 0;
                ss.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("secondscreen", !secondScreenOn()).apply(); } };
                items.add(ss);
                Item hb = new Item(getString(R.string.opt_hudbottom), null);
                hb.check = hudBottomOn() ? 1 : 0;
                hb.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("hudbottom", !hudBottomOn()).apply(); } };
                items.add(hb);
            } else if (g.equals("pad")) {
                items.add(new Item(getString(R.string.opt_padmap), new Runnable() { public void run() { setScreen(PADMAP); } }));
                items.add(toggle(R.string.opt_dualshock, "dualshock"));   // --dualshock (shims/pad.c)
                Item rb = new Item(getString(R.string.opt_rumble), null);   // --rumble (runtime/sdl.c)
                rb.value = rumbleName(rumbleIdx());
                rb.adj = new Adjust() { public void by(int d) {   // wraps like the other value rows (A / tap steps up)
                    int n = (rumbleIdx() + d + RUMBLE.length) % RUMBLE.length;
                    prefs.edit().putInt("rumble", n).apply(); rumbleTest(); } };
                items.add(rb);
                if (pal) {   // (the button icons are PAL-only in the port)
                    Item pr = new Item(getString(R.string.opt_prompts), null);   // --prompts (shims/prompts.c; TombiActivity resolves auto)
                    pr.value = getResources().getStringArray(R.array.prompt_names)[promptIdx()];
                    pr.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("prompts", (promptIdx() + d + PROMPTS.length) % PROMPTS.length).apply(); } };
                    items.add(pr);
                    items.add(new Item(getString(R.string.pc_open), new Runnable() { public void run() { setScreen(CUSTOM); } }));
                }
            } else if (g.equals("saves")) {
                items.add(new Item(getString(R.string.save_import), new Runnable() { public void run() { pickSaveImport(); } }));
                items.add(new Item(getString(R.string.save_export), new Runnable() { public void run() { pickSaveExport(); } }));
            } else if (g.equals("disc")) {
                items.add(new Item(getString(R.string.opt_disc), new Runnable() { public void run() { setScreen(DISC); } }));
                items.add(new Item(getString(R.string.disc_wipe), new Runnable() { public void run() {
                    setScreen(DISC); confirmWipe = true; wipeSaves = false; focus = 0; } }));
            } else if (g.equals("cheats")) {
                items.add(toggle(R.string.opt_inflives, "inflives"));   // (named fields: every version, cheatArgs)
                items.add(toggle(R.string.opt_infhealth, "infhealth"));
            } else if (g.equals("debugmenu")) {
                items.add(toggle(R.string.opt_debug, "debugmenu"));
            } else if (g.equals("viewlog")) {
                items.add(new Item(getString(R.string.opt_viewlog), new Runnable() { public void run() { setScreen(LOG); } }));
            }
        }

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
            if (screen == MAIN && (code == KeyEvent.KEYCODE_DPAD_UP || code == KeyEvent.KEYCODE_DPAD_DOWN
                    || code == KeyEvent.KEYCODE_DPAD_LEFT || code == KeyEvent.KEYCODE_DPAD_RIGHT)) {
                int dx = code == KeyEvent.KEYCODE_DPAD_LEFT ? -1 : code == KeyEvent.KEYCODE_DPAD_RIGHT ? 1 : 0;
                int dy = code == KeyEvent.KEYCODE_DPAD_UP ? -1 : code == KeyEvent.KEYCODE_DPAD_DOWN ? 1 : 0;
                int t = spatial(dx, dy);
                if (t < 0 && dy != 0) t = (focus + n + dy) % n;   // (no rectangles yet, or at an edge: in list order)
                if (t >= 0) { focus = t; focusMoved(); }
                invalidate();
                return true;
            }
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
                    else if (screen == CUSTOM) setScreen(OPTCAT); else if (screen == COLOR) setScreen(CUSTOM);
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
            if (it.actFirst && it.act != null) it.act.run();
            else if (it.adj != null) it.adj.by(1);
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
                downFocus = focus;
                if (hit >= 0 && !flinging) { focus = hit; focusMoved(); invalidate(); }
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
            if (act == MotionEvent.ACTION_UP && screen == COLOR) {   // a palette swatch
                for (int k = 0; k < pcSw.length; k++) if (pcSw[k] != null && pcSw[k].contains(x, y)) { pcSet(pcEdit, PC_PALETTE[k]); invalidate(); return true; }
            }
            // the library: a tap on another game's card only selects it; a tap on the selected card plays
            if (act == MotionEvent.ACTION_UP && screen == MAIN && hit >= 0 && hit < items.size() && items.get(hit).game >= 0 && downFocus != hit) return true;
            if (act == MotionEvent.ACTION_UP && hit >= 0 && hit == focus) {
                build();
                if ((screen == OPTCAT || screen == CUSTOM || screen == COLOR) && hit < items.size()) {   // value button: left half = previous, right half = next
                    Item it = items.get(hit);
                    if (it.adj != null && it.check < 0 && !it.actFirst) { it.adj.by(x < it.r.centerX() ? -1 : 1); invalidate(); return true; }
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

        /** t at x, centred in r's height, in the current paint, shrunk to at most maxW wide (the translations differ
         *  in length; nothing runs past its pill) */
        void drawFit(String t, float x, RectF r, float maxW) {
            float sz = p.getTextSize(), w = p.measureText(t);
            if (w > maxW && maxW > 0) p.setTextSize(sz * maxW / w);
            canvas.drawText(t, x, midBase(r), p);
            p.setTextSize(sz);
        }
        void drawFitAt(String t, float x, float base, float maxW) {
            float sz = p.getTextSize(), w = p.measureText(t);
            if (w > maxW && maxW > 0) p.setTextSize(sz * maxW / w);
            canvas.drawText(t, x, base, p);
            p.setTextSize(sz);
        }

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
            float w = p.measureText(t);
            if (w > 1200) { size = size * 1200 / w; p.setTextSize(size); }
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
            if (screen == MAIN) drawLibrary();
            else if (screen == OPTIONS || screen == OPTCAT) drawOptions();
            else if (screen == PADMAP) drawPadMap();
            else if (screen == CUSTOM || screen == COLOR) drawCustom();
            else if (screen == CRASH || screen == LOG) drawLog();
            else drawDisc();
            c.restore();
            if (drawn.length != items.size()) drawn = new RectF[items.size()];
            for (int i = 0; i < items.size(); i++) drawn[i] = new RectF(items.get(i).r);
        }

        // ---- the library (MAIN): three looks (Options > Game > Library style), the same items ----
        void drawLibrary() {
            int st = libStyle();
            if (st == 1) drawLibList(); else if (st == 2) drawLibCover(); else drawLibCarousel();
            drawChips();
        }

        /** the version / options chips along the bottom */
        void drawChips() {
            List<String> chips = new ArrayList<>();
            if (palGame()) {
                chips.add(LANG_NAME[langIdx()]);
                chips.add(aspectName(aspectIdx()));
                int nc = cutItems().size();
                chips.add(nc == 0 ? getString(R.string.chip_cut_off) : getString(R.string.chip_cut_on) + " " + nc + "/" + CUT_ITEMS.length);
            } else { chips.add("English"); chips.add(aspectName(aspectIdx())); }
            chips.add(version());
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

        Library.Game gameOf(Item it) { return it.game >= 0 && it.game < games.size() ? games.get(it.game) : null; }
        boolean isSel(Library.Game g) { Library.Game s = game(); return g != null && s != null && g.id.equals(s.id); }
        /** status line of a card: ready / no data / not supported / not in this APK */
        String statusOf(Library.Game g) {
            if (g.ver == null || !g.ver.supported()) return getString(R.string.lib_unsupported);
            if (!libPresent(g.ver)) return getString(R.string.lib_nolib);
            return g.ready() ? getString(R.string.lib_ready) : getString(R.string.disc_missing);
        }
        boolean okOf(Library.Game g) { return g.ready() && libPresent(g.ver); }

        /** the game's logo (from the player's disc) fitted in r, centred; the name in the title font without one */
        void drawLogo(Library.Game g, RectF r, boolean big) {
            android.graphics.Bitmap b = logoOf(g);
            if (b != null) {
                float sc = Math.min(r.width() / b.getWidth(), r.height() / b.getHeight());
                float w = b.getWidth() * sc, h = b.getHeight() * sc;
                RectF d = new RectF(r.centerX() - w / 2, r.centerY() - h / 2, r.centerX() + w / 2, r.centerY() + h / 2);
                Paint bp = new Paint();
                bp.setFilterBitmap(sc < 1.5f);   // pixel art: sharp when enlarged
                canvas.drawBitmap(b, null, d, bp);
                return;
            }
            String t = g != null && g.ver != null ? g.ver.logoText : "?";
            float size = big ? 96 : 48;
            text(lilita, size, ACCENT_SH);
            float w = p.measureText(t), max = r.width() - 8;
            if (w > max) { size = size * max / w; p.setTextSize(size); }
            float base = r.centerY() + size * 0.36f;
            canvas.drawText(t, r.centerX(), base + size * 6 / 108, p);
            p.setColor(ACCENT);
            canvas.drawText(t, r.centerX(), base, p);
        }

        /** a ready / not-ready dot + text, left aligned at x (or centred when centre) */
        void statusLine(Library.Game g, float x, float base, float maxW, boolean centre, int col) {
            boolean ok = okOf(g);
            String t = (ok ? "✓ " : "✗ ") + statusOf(g);
            text(nunito, 16, ok ? col : ACCENT_SH);
            p.setTextAlign(centre ? Paint.Align.CENTER : Paint.Align.LEFT);
            drawFitAt(t, x, base, maxW);
        }

        /** the "Add game" card: dashed outline, a big plus, the label and the formats */
        void addCard(Item it, boolean f, boolean wide) {
            RectF r = it.r;
            float rad = Math.min(26, r.height() / 2);
            p.setStyle(Paint.Style.FILL); p.setColor(f ? 0x40ffffff : 0x20ffffff);
            canvas.drawRoundRect(r, rad, rad, p);
            p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(4); p.setColor(f ? Color.WHITE : CREAM);
            p.setPathEffect(new android.graphics.DashPathEffect(new float[]{14, 10}, 0));
            canvas.drawRoundRect(r, rad, rad, p);
            p.setPathEffect(null); p.setStyle(Paint.Style.FILL);
            if (wide) {   // a row: the plus on the left, the texts beside it
                float cy = r.centerY(), cx = r.left + 52;
                p.setColor(ACCENT); canvas.drawCircle(cx, cy, 26, p);
                p.setColor(Color.WHITE); canvas.drawRect(cx - 13, cy - 3.5f, cx + 13, cy + 3.5f, p); canvas.drawRect(cx - 3.5f, cy - 13, cx + 3.5f, cy + 13, p);
                text(nunito, 22, Color.WHITE); p.setTextAlign(Paint.Align.LEFT);
                drawFitAt(it.label, r.left + 96, cy - 4, r.width() - 116);
                text(nunito, 15, SUB); p.setTextAlign(Paint.Align.LEFT);
                drawFitAt(getString(R.string.lib_add_sub), r.left + 96, cy + 20, r.width() - 116);
                return;
            }
            float cx = r.centerX(), cy = r.top + r.height() * 0.38f, rr = Math.min(44, r.width() / 6);
            p.setColor(ACCENT); canvas.drawCircle(cx, cy, rr, p);
            p.setColor(Color.WHITE); canvas.drawRect(cx - rr / 2, cy - rr / 7, cx + rr / 2, cy + rr / 7, p); canvas.drawRect(cx - rr / 7, cy - rr / 2, cx + rr / 7, cy + rr / 2, p);
            text(nunito, 22, Color.WHITE);
            drawFitAt(it.label, cx, r.top + r.height() * 0.70f, r.width() - 30);
            tp.setTypeface(nunito); tp.setTextSize(14); tp.setColor(SUB); tp.setTextAlign(Paint.Align.LEFT);
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout(getString(R.string.lib_add_sub), tp, (int) (r.width() - 36), Layout.Alignment.ALIGN_CENTER, 1.05f, 0, false);
            canvas.save(); canvas.translate(r.left + 18, r.top + r.height() * 0.76f); l.draw(canvas); canvas.restore();
        }

        /** a menu pill (Play / Options / Quit) in the launcher's style */
        void button(Item it, boolean f, float size) {
            if (it.primary) pill(it.r, it.dim ? 0xffc9a0b5 : ACCENT, ACCENT_SH, 6, f);
            else pill(it.r, f ? Color.WHITE : CREAM, CREAM_SH, 5, f);
            text(nunito, size, it.primary ? (it.dim ? 0xff6e4a5c : ACCENT_TEXT) : BG);
            drawFit(it.label, it.r.centerX(), it.r, it.r.width() - 30);
        }

        /** a card's frame: cream, its shadow, the white focus ring, the accent edge on the selected game */
        void cardFrame(RectF r, boolean f, boolean sel) {
            float rad = 26;
            if (f) {
                p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(5); p.setColor(Color.WHITE);
                tmp.set(r.left - 9, r.top - 9, r.right + 9, r.bottom + 6 + 9);
                canvas.drawRoundRect(tmp, rad + 8, rad + 8, p);
                p.setStyle(Paint.Style.FILL);
            }
            p.setColor(CREAM_SH); tmp.set(r.left, r.top + 6, r.right, r.bottom + 6); canvas.drawRoundRect(tmp, rad, rad, p);
            p.setColor(f ? Color.WHITE : CREAM); canvas.drawRoundRect(r, rad, rad, p);
            if (sel) {
                p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(5); p.setColor(ACCENT);
                tmp.set(r.left + 2.5f, r.top + 2.5f, r.right - 2.5f, r.bottom - 2.5f);
                canvas.drawRoundRect(tmp, rad - 2, rad - 2, p);
                p.setStyle(Paint.Style.FILL);
            }
        }

        /** a small rounded label (serial, region) at x (left), centred on cy; returns its right edge */
        /** a small drawn flag (no image files): 0 UK, 1 FR, 2 DE, 3 IT, 4 NL, 5 ES, 6 PT, 7 SV, 8 DA, 9 US, 10 JP, as the
         *  PC launcher's flag_pixmap; w = 1.5 h, top-left at x, y */
        void drawFlag(int n, float x, float y, float h) {
            float w = h * 1.5f;
            p.setStyle(Paint.Style.FILL);
            canvas.save();
            canvas.clipRect(x, y, x + w, y + h);
            switch (n) {
                case 0:   // United Kingdom (simplified)
                    box(x, y, w, h, 0, 0, 1, 1, 0xff012169);
                    p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(h * 0.19f); p.setColor(Color.WHITE);
                    canvas.drawLine(x, y, x + w, y + h, p); canvas.drawLine(x + w, y, x, y + h, p);
                    p.setStrokeWidth(h * 0.07f); p.setColor(0xffC8102E);
                    canvas.drawLine(x, y, x + w, y + h, p); canvas.drawLine(x + w, y, x, y + h, p);
                    p.setStyle(Paint.Style.FILL);
                    box(x, y, w, h, 0, 0.31f, 1, 0.69f, Color.WHITE); box(x, y, w, h, 0.375f, 0, 0.625f, 1, Color.WHITE);
                    box(x, y, w, h, 0, 0.375f, 1, 0.625f, 0xffC8102E); box(x, y, w, h, 0.417f, 0, 0.583f, 1, 0xffC8102E);
                    break;
                case 1: vert(x, y, w, h, 0xff0055A4, Color.WHITE, 0xffEF4135); break;
                case 2: horiz(x, y, w, h, 0xff000000, 0xffDD0000, 0xffFFCE00); break;
                case 3: vert(x, y, w, h, 0xff009246, Color.WHITE, 0xffCE2B37); break;
                case 4: horiz(x, y, w, h, 0xffAE1C28, Color.WHITE, 0xff21468B); break;
                case 5: box(x, y, w, h, 0, 0, 1, 1, 0xffAA151B); box(x, y, w, h, 0, 0.25f, 1, 0.75f, 0xffF1BF00); break;
                case 6:
                    box(x, y, w, h, 0, 0, 0.4f, 1, 0xff006600); box(x, y, w, h, 0.4f, 0, 1, 1, 0xffFF0000);
                    p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(h * 0.1f); p.setColor(0xffFFE000);
                    canvas.drawCircle(x + w * 0.4f, y + h / 2, h / 4, p); p.setStyle(Paint.Style.FILL);
                    break;
                case 7: nordic(x, y, w, h, 0xff006AA7, 0xffFECC02); break;
                case 8: nordic(x, y, w, h, 0xffC8102E, Color.WHITE); break;
                case 9:
                    for (int i = 0; i < 13; i++) box(x, y, w, h, 0, i / 13f, 1, (i + 1) / 13f, i % 2 == 0 ? 0xffB22234 : Color.WHITE);
                    box(x, y, w, h, 0, 0, 0.42f, 7 / 13f, 0xff3C3B6E);
                    break;
                case 10:
                    box(x, y, w, h, 0, 0, 1, 1, Color.WHITE);
                    p.setColor(0xffBC002D); canvas.drawCircle(x + w / 2, y + h / 2, h * 0.3f, p);
                    break;
                default: box(x, y, w, h, 0, 0, 1, 1, 0xff888888);
            }
            canvas.restore();
            p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(1); p.setColor(0x3c000000);
            canvas.drawRect(x + 0.5f, y + 0.5f, x + w - 0.5f, y + h - 0.5f, p);
            p.setStyle(Paint.Style.FILL);
        }
        void box(float x, float y, float w, float h, float x0, float y0, float x1, float y1, int c) {
            p.setColor(c); canvas.drawRect(x + w * x0, y + h * y0, x + w * x1, y + h * y1, p);
        }
        void vert(float x, float y, float w, float h, int a, int b, int c) {
            box(x, y, w, h, 0, 0, 1 / 3f, 1, a); box(x, y, w, h, 1 / 3f, 0, 2 / 3f, 1, b); box(x, y, w, h, 2 / 3f, 0, 1, 1, c);
        }
        void horiz(float x, float y, float w, float h, int a, int b, int c) {
            box(x, y, w, h, 0, 0, 1, 1 / 3f, a); box(x, y, w, h, 0, 1 / 3f, 1, 2 / 3f, b); box(x, y, w, h, 0, 2 / 3f, 1, 1, c);
        }
        void nordic(float x, float y, float w, float h, int bg, int cross) {
            box(x, y, w, h, 0, 0, 1, 1, bg); box(x, y, w, h, 0, 0.4f, 1, 0.6f, cross); box(x, y, w, h, 0.29f, 0, 0.46f, 1, cross);
        }
        /** the disc's languages as flags in a row from x, centred on cy; returns the right end (x when none) */
        float flags(Library.Game g, float x, float cy, float h) {
            int[] f = Library.flagsOf(g.serial);
            for (int n : f) { drawFlag(n, x, cy - h / 2, h); x += h * 1.5f + 5; }
            return f.length > 0 ? x - 5 : x;
        }
        float flagsWidth(Library.Game g, float h) { int n = Library.flagsOf(g.serial).length; return n == 0 ? 0 : n * (h * 1.5f + 5) - 5; }

        float tag(String t, float x, float cy, int bg, int fg) {
            text(nunito, 14, fg); p.setTextAlign(Paint.Align.LEFT);
            float w = p.measureText(t) + 20;
            tmp.set(x, cy - 13, x + w, cy + 13);
            p.setColor(bg); canvas.drawRoundRect(tmp, 13, 13, p);
            p.setColor(fg); canvas.drawText(t, x + 10, midBase(tmp), p);
            return x + w;
        }

        /** 0: Carousel. The cards in a row (the selected one lifted, the focused one ringed), Play / Options / Quit
         *  beneath, side by side */
        void drawLibCarousel() {
            bigTitle(getString(R.string.lib_title), 52, 74);
            List<Item> cards = new ArrayList<>(), btns = new ArrayList<>();
            for (Item it : items) (it.game >= 0 || it.add ? cards : btns).add(it);
            int n = cards.size();
            float gap = 26, cw = Math.min(n == 1 ? 420 : 340, (1200 - (n - 1) * gap) / n), ch = games.isEmpty() ? 250 : 300, top = 116;
            float x = 640 - (n * cw + (n - 1) * gap) / 2;
            for (Item it : cards) {
                int i = items.indexOf(it);
                boolean f = i == focus;
                Library.Game g = gameOf(it);
                boolean sel = isSel(g);
                float lift = sel ? -10 : 0;
                it.r.set(x, top + lift, x + cw, top + ch + lift);
                if (it.add) addCard(it, f, false);
                else {
                    cardFrame(it.r, f, sel);
                    RectF lr = new RectF(it.r.left + 18, it.r.top + 18, it.r.right - 18, it.r.top + 160);
                    drawLogo(g, lr, false);
                    text(nunito, 22, BG);
                    drawFitAt(versionName(g.ver), it.r.centerX(), it.r.top + 196, cw - 36);
                    text(nunito, 14, BG);
                    float tw = p.measureText(g.serial) + 20;
                    tag(g.serial, it.r.centerX() - tw / 2, it.r.top + 220, 0x262f6b3a, BG);
                    // the disc's languages: a row of flags under the serial (as small as the card needs)
                    float fh = 14;
                    while (fh > 8 && flagsWidth(g, fh) > cw - 36) fh -= 1;
                    flags(g, it.r.centerX() - flagsWidth(g, fh) / 2, it.r.top + 248, fh);
                    statusLine(g, it.r.centerX(), it.r.top + 282, cw - 36, true, 0xff2a7a3a);
                }
                x += cw + gap;
            }
            if (games.isEmpty()) {
                tp.setTypeface(nunito); tp.setTextSize(18); tp.setColor(SUB); tp.setTextAlign(Paint.Align.LEFT);
                wrapped(getString(R.string.lib_empty), 392);
            }
            // Play wide, Options and Quit beside it
            float bw0 = 380, bw1 = 250, bw2 = 200, g2 = 22, y = 470;
            float bx = 640 - (bw0 + bw1 + bw2 + 2 * g2) / 2;
            float[] ws = {bw0, bw1, bw2};
            for (int k = 0; k < btns.size() && k < 3; k++) {
                Item it = btns.get(k);
                it.r.set(bx, it.primary ? y : y + 8, bx + ws[k], it.primary ? y + 78 : y + 70);
                button(it, items.indexOf(it) == focus, it.primary ? 30 : 24);
                bx += ws[k] + g2;
            }
            text(nunito, 16, SUB);
            Library.Game sg = game();
            if (sg != null && sg.name != null && !sg.name.isEmpty()) drawFitAt(getString(R.string.lib_from, sg.name), 640, 600, 1100);
        }

        /** 1: List. One row per game on the left (logo, name, serial, status); the selected game large on the right with
         *  Play / Options / Quit */
        void drawLibList() {
            text(lilita, 50, ACCENT_SH); p.setTextAlign(Paint.Align.LEFT);
            canvas.drawText(getString(R.string.lib_title), 44, 81, p);
            p.setColor(ACCENT); canvas.drawText(getString(R.string.lib_title), 44, 78, p);
            List<Item> rows = new ArrayList<>(), btns = new ArrayList<>();
            for (Item it : items) (it.game >= 0 || it.add ? rows : btns).add(it);
            float top = 108, bottom = 630, gap = 16;
            float rh = Math.min(116, (bottom - top - (rows.size() - 1) * gap) / Math.max(1, rows.size()));
            float y = top;
            for (Item it : rows) {
                int i = items.indexOf(it);
                boolean f = i == focus;
                it.r.set(40, y, 640, y + rh);
                if (it.add) addCard(it, f, true);
                else {
                    Library.Game g = gameOf(it);
                    cardFrame(it.r, f, isSel(g));
                    RectF lr = new RectF(it.r.left + 16, it.r.top + 12, it.r.left + 206, it.r.bottom - 12);
                    drawLogo(g, lr, false);
                    float tx = it.r.left + 226, tw = it.r.right - 20 - tx;
                    text(nunito, 22, BG); p.setTextAlign(Paint.Align.LEFT);
                    drawFitAt(versionName(g.ver), tx, it.r.top + rh * 0.36f, tw);
                    float r2 = tag(g.serial, tx, it.r.top + rh * 0.58f, 0x262f6b3a, BG);
                    if (flagsWidth(g, 15) > 0) r2 = flags(g, r2 + 10, it.r.top + rh * 0.58f, 15);
                    statusLine(g, r2 + 12, it.r.top + rh * 0.58f + 6, it.r.right - 20 - r2 - 12, false, 0xff2a7a3a);
                }
                y += rh + gap;
            }
            // the selected game, large
            RectF pane = new RectF(670, 108, 1240, 630);
            p.setColor(CHIP); canvas.drawRoundRect(pane, 28, 28, p);
            Library.Game g = game();
            if (g != null) {
                drawLogo(g, new RectF(pane.left + 30, pane.top + 22, pane.right - 30, pane.top + 184), true);
                text(nunito, 26, Color.WHITE);
                drawFitAt(versionName(g.ver), pane.centerX(), pane.top + 218, pane.width() - 40);
                text(nunito, 16, SUB);
                // the serial and the disc's languages (flags) on one line (the file it came from: the Disc screen)
                float sw = p.measureText(g.serial), fw = flagsWidth(g, 16), all = sw + (fw > 0 ? 14 + fw : 0);
                p.setTextAlign(Paint.Align.LEFT);
                canvas.drawText(g.serial, pane.centerX() - all / 2, pane.top + 248, p);
                if (fw > 0) flags(g, pane.centerX() - all / 2 + sw + 14, pane.top + 242, 16);
            } else {
                tp.setTypeface(nunito); tp.setTextSize(18); tp.setColor(SUB); tp.setTextAlign(Paint.Align.LEFT);
                @SuppressWarnings("deprecation")
                StaticLayout l = new StaticLayout(getString(R.string.lib_empty), tp, (int) pane.width() - 60, Layout.Alignment.ALIGN_CENTER, 1.1f, 0, false);
                canvas.save(); canvas.translate(pane.left + 30, pane.top + 100); l.draw(canvas); canvas.restore();
            }
            // Play / Options / Quit at the pane's bottom, inset from it as much as from its sides (the device report: they
            // touched the edge); the button's shadow (5) inside the inset
            float inset = 48, bgap = 14, bsum = -bgap;
            for (Item it : btns) bsum += (it.primary ? 68 : 54) + bgap;
            float by = pane.bottom - inset - 5 - bsum;
            for (Item it : btns) {
                float h = it.primary ? 68 : 54;
                it.r.set(pane.left + inset, by, pane.right - inset, by + h);
                button(it, items.indexOf(it) == focus, it.primary ? 30 : 23);
                by += h + bgap;
            }
        }

        /** 2: Cover. The selected game's logo large (as the title used to be), a tab per game under it, the menu as
         *  before */
        void drawLibCover() {
            Library.Game g = game();
            RectF lr = new RectF(290, 22, 990, 214);
            if (g != null) drawLogo(g, lr, true);
            else bigTitle("TOMBI!", 108, 160);
            text(nunito, 18, SUB);
            p.setLetterSpacing(2f / 18f);
            canvas.drawText(g != null ? (versionName(g.ver) + "  ·  " + g.serial).toUpperCase(Locale.ROOT) : getString(R.string.subtitle), 640, 244, p);
            p.setLetterSpacing(0);
            List<Item> tabs = new ArrayList<>(), btns = new ArrayList<>();
            for (Item it : items) (it.game >= 0 || it.add ? tabs : btns).add(it);
            // the tabs: one pill per game (region + status mark), "+ Add game" last
            text(nunito, 18, BG);
            float gap = 14, total = -gap;
            String[] lab = new String[tabs.size()];
            for (int k = 0; k < tabs.size(); k++) {
                Item it = tabs.get(k);
                Library.Game tg = gameOf(it);
                lab[k] = it.add ? "+  " + it.label : (okOf(tg) ? "" : "✗ ") + tg.ver.title + " · " + getResources().getStringArray(R.array.lib_regions)[tg.ver.region];
                total += Math.min(300, p.measureText(lab[k]) + 48) + gap;
            }
            float x = 640 - total / 2, ty = 268;
            for (int k = 0; k < tabs.size(); k++) {
                Item it = tabs.get(k);
                boolean f = items.indexOf(it) == focus, sel = !it.add && isSel(gameOf(it));
                text(nunito, 18, BG);
                float w = Math.min(300, p.measureText(lab[k]) + 48);
                it.r.set(x, ty, x + w, ty + 46);
                if (it.add) {
                    p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(3); p.setColor(f ? Color.WHITE : CREAM);
                    p.setPathEffect(new android.graphics.DashPathEffect(new float[]{10, 8}, 0));
                    canvas.drawRoundRect(it.r, 23, 23, p);
                    p.setPathEffect(null); p.setStyle(Paint.Style.FILL);
                    text(nunito, 18, Color.WHITE);
                } else {
                    pill(it.r, sel ? ACCENT : f ? Color.WHITE : CREAM, sel ? ACCENT_SH : CREAM_SH, 4, f);
                    text(nunito, 18, sel ? ACCENT_TEXT : BG);
                }
                drawFit(lab[k], it.r.centerX(), it.r, w - 24);
                x += w + gap;
            }
            float y = 352;
            if (games.isEmpty()) {
                tp.setTypeface(nunito); tp.setTextSize(17); tp.setColor(SUB); tp.setTextAlign(Paint.Align.LEFT);
                y = Math.max(y, 330 + wrapped(getString(R.string.lib_empty), 330) + 18);
            }
            // the menu: its bottom (with the shadow) at most 30 above the chips (650)
            float bstep = 16, bsum = -bstep;
            for (Item it : btns) bsum += (it.primary ? 78 + 6 : 62 + 5) + bstep;
            if (y + bsum > 620) { bstep = Math.max(8, bstep - (y + bsum - 620) / Math.max(1, btns.size() - 1)); bsum = -bstep; for (Item it : btns) bsum += (it.primary ? 78 + 6 : 62 + 5) + bstep; }
            if (y + bsum > 620) y = 620 - bsum;
            for (Item it : btns) {
                float hh = it.primary ? 78 : 62;
                it.r.set(640 - 230, y, 640 + 230, y + hh);
                button(it, items.indexOf(it) == focus, it.primary ? 30 : 24);
                y += hh + (it.primary ? 6 : 5) + bstep;
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

        /** the custom buttons pages: the rows on the left, the preview (drawn as the game draws it) on the right */
        void drawCustom() {
            bigTitle(getString(R.string.pc_title), 50, 66);
            float top = 96, bottom = 640, rowH = Math.min(56, (bottom - top + 8) / Math.max(1, items.size())), bh = rowH - 8;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                float y = top + i * rowH;
                it.r.set(40, y, 700, y + bh);
                pill(it.r, f ? ACCENT : CREAM, f ? ACCENT_SH : CREAM_SH, 3, f);
                int col = f ? ACCENT_TEXT : BG;
                text(nunito, 19, col);
                if (it.value == null) { drawFit(it.label, it.r.centerX(), it.r, it.r.width() - 40); continue; }
                float vr = it.r.right - 44, cy = it.r.centerY(), vw = p.measureText(it.value) + (it.swatch >= 0 ? 40 : 0);
                p.setTextAlign(Paint.Align.LEFT);
                drawFit(it.label, it.r.left + 22, it.r, vr - vw - 40 - (it.r.left + 22));
                p.setTextAlign(Paint.Align.RIGHT);
                canvas.drawText(it.value, vr, midBase(it.r), p);
                float vl = vr - p.measureText(it.value);
                if (it.swatch >= 0) {
                    tmp.set(vl - 40, cy - 13, vl - 12, cy + 13);
                    p.setColor(0xFF000000 | it.swatch); canvas.drawRoundRect(tmp, 6, 6, p);
                    p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(2); p.setColor(col); canvas.drawRoundRect(tmp, 6, 6, p);
                    p.setStyle(Paint.Style.FILL); vl -= 40;
                }
                p.setColor(col);
                arrow(vr + 12, cy, 1);
                arrow(vl - 12, cy, -1);
            }
            // preview panel
            tmp.set(740, 100, 1240, 640);
            p.setColor(CHIP); canvas.drawRoundRect(tmp, 24, 24, p);
            Paint px = new Paint();   // nearest: one rect per glyph pixel
            if (screen == CUSTOM) {
                float sc = 8, gw = 12 * sc, gh = 16 * sc, cx = 990, cy = 330;
                float[][] at = {{cx - gw / 2, cy + 40}, {cx + 70, cy - gh / 2}, {cx - 70 - gw, cy - gh / 2}, {cx - gw / 2, cy - 40 - gh}};
                for (int pos = 0; pos < 4; pos++) glyphAt(px, pcGlyph(pos, pcSym(), pcCol(2 * pos), pcCol(2 * pos + 1)), at[pos][0], at[pos][1], sc);
                text(nunito, 16, Color.WHITE);
                canvas.drawText(getString(R.string.pc_preview), 990, 612, p);
            } else {
                int pos = pcEdit / 2;
                glyphAt(px, pcGlyph(pos, pcSym(), pcCol(2 * pos), pcCol(2 * pos + 1)), 990 - 60, 120, 10);
                float sw = 64, gap = 12, x0 = 990 - (6 * sw + 5 * gap) / 2, y0 = 300;
                for (int k = 0; k < PC_PALETTE.length; k++) {
                    if (pcSw[k] == null) pcSw[k] = new RectF();
                    float x = x0 + (k % 6) * (sw + gap), y = y0 + (k / 6) * (sw + gap);
                    pcSw[k].set(x, y, x + sw, y + sw);
                    p.setColor(0xFF000000 | PC_PALETTE[k]); canvas.drawRoundRect(pcSw[k], 10, 10, p);
                    if (PC_PALETTE[k] == pcCol(pcEdit)) {
                        p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(4); p.setColor(Color.WHITE);
                        canvas.drawRoundRect(pcSw[k], 10, 10, p); p.setStyle(Paint.Style.FILL);
                    }
                }
            }
            tp.setTypeface(nunito); tp.setTextSize(14); tp.setColor(SUB);
            tp.setTextAlign(Paint.Align.LEFT);
            wrapped(getString(R.string.pc_hint), 656);
        }

        void glyphAt(Paint px, int[] g, float x0, float y0, float sc) {
            for (int y = 0; y < 16; y++) for (int x = 0; x < 12; x++) {
                int c = g[y * 12 + x];
                if (c == 0) continue;
                px.setColor(c);
                canvas.drawRect(x0 + x * sc, y0 + y * sc, x0 + (x + 1) * sc, y0 + (y + 1) * sc, px);
            }
        }

        void drawOptions() {
            bigTitle(screen == OPTIONS ? getString(R.string.options) : getString(catName(cat)), 50, 66);
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
                    p.setTextSize(17); p.setTextAlign(Paint.Align.RIGHT);
                    float sw = Math.min(p.measureText(it.sub), 360);
                    drawFit(it.sub, it.r.right - 30, it.r, 360);
                    p.setTextSize(22); p.setTextAlign(Paint.Align.LEFT);
                    drawFit(it.label, it.r.left + 30, it.r, it.r.width() - 60 - sw - 24);
                } else if (it.check >= 0) {   // on/off: label + tick box on the right
                    p.setTextAlign(Paint.Align.LEFT);
                    drawFit(it.label, it.r.left + 26, it.r, it.r.width() - 26 - 52 - 20);
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
                    drawFit(it.label, it.r.centerX(), it.r, it.r.width() - 60);
                } else {   // value: label + "◀ value ▶"
                    float vr = it.r.right - 50, cy = it.r.centerY();
                    p.setTextAlign(Paint.Align.LEFT);
                    drawFit(it.label, it.r.left + 26, it.r, vr - p.measureText(it.value) - 40 - (it.r.left + 26));
                    p.setTextAlign(Paint.Align.RIGHT);
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

        String[] padWords() { return new String[]{getString(R.string.pad_dpad) + " ", getString(R.string.pad_axis) + " ", getString(R.string.pad_none)}; }
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
            padStatus = getString(R.string.pad_bound, t < names.length ? names[t] : PadMap.NAMES[t], PadMap.srcName(s, padWords()));
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
                if (it.value == null) drawFit(it.label, 640, it.r, it.r.width() - 52);
                else {
                    p.setTextAlign(Paint.Align.RIGHT); canvas.drawText(it.value, it.r.right - 26, midBase(it.r), p);
                    float vw = p.measureText(it.value);
                    p.setTextAlign(Paint.Align.LEFT); drawFit(it.label, it.r.left + 26, it.r, it.r.width() - 52 - vw - 20);
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
                drawFitAt(getString(R.string.pad_press, capture < names.length ? names[capture] : PadMap.NAMES[capture]), 640, 340, 680);
                text(nunito, 18, BG);
                drawFitAt(getString(R.string.pad_cancel), 640, 400, 680);
            }
        }

        /** t wrapped (never cut) in a centred column 900 wide from top; returns its height */
        float wrapped(String t, float top) { return wrapped(t, top, 900); }
        float wrapped(String t, float top, int width) {
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout(t, tp, width, Layout.Alignment.ALIGN_CENTER, 1.1f, 0, false);
            canvas.save();
            canvas.translate(640 - width / 2f, top);
            l.draw(canvas);
            canvas.restore();
            return l.getHeight();
        }

        /** Selva pill bar: dark track, accent fill with its shadow edge (as the buttons), phase / % / MB labels. */
        void progressBar(int ph, float top) {
            long done = copied, tot = copyTotal;
            if (ph == UNZIPPING && native7z && nativeOk) { done = nativeUnpackBytes(0); tot = nativeUnpackBytes(1); }
            float frac;
            if (ph == CHECKING || ph == EXTRACTING) frac = nativeOk ? Math.max(0, Math.min(1, nativeCheckProgress() / 1000f)) : 0;   // (UNZIPPING: bytes of the zip)
            else frac = tot > 0 ? Math.min(1f, done / (float) tot) : 0;
            String label = getString(ph == EXTRACTING ? R.string.disc_phase_extract : ph == CHECKING ? R.string.disc_phase_check
                                     : ph == UNZIPPING ? R.string.disc_phase_unzip : R.string.disc_phase_copy);
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
            String mb = ph == EXTRACTING ? extractDetail() : ph == CHECKING ? "SYSTEM.CNF · BOOT (SHA-1)"
                    : tot > 0 ? (done >> 20) + " / " + (tot >> 20) + " MB" : (done >> 20) + " MB";
            canvas.drawText(mb, 640, top + 78, p);
        }

        String extractDetail() {
            int c = nativeOk ? nativeExtractCount() : 0;
            return getString(R.string.disc_extract_files, (c >>> 16) & 0xFFFF, c & 0xFFFF);
        }

        /** the result of an import / a removal: a tinted box as tall as its wrapped text; returns its bottom */
        float resultBox(boolean ok, String msg, float top) {
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT); tp.setTextSize(18); tp.setColor(Color.WHITE);
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout((ok ? "✓  " : "✗  ") + msg, tp, 680, Layout.Alignment.ALIGN_CENTER, 1.05f, 0, false);
            tmp.set(640 - 370, top, 640 + 370, top + Math.max(64, l.getHeight() + 28));
            p.setStyle(Paint.Style.FILL);
            p.setColor(ok ? 0x5527c25a : 0x55ff3a5c);
            canvas.drawRoundRect(tmp, 22, 22, p);
            float bottom = tmp.bottom;
            canvas.save();
            canvas.translate(640 - 340, tmp.centerY() - l.getHeight() / 2f);
            l.draw(canvas);
            canvas.restore();
            return bottom;
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
                drawFit(it.label, it.r.centerX(), it.r, bw - 30);
                x += bw + gap;
            }
        }

        /** the Disc screen: the selected game (its status, where its data came from) or, while adding a disc, the file
         *  being added; the help; the progress / result; the buttons.  One column: each block is wrapped (never cut) and
         *  starts below the previous one, the buttons below them all (closer together when the column is long). */
        void drawDisc() {
            Library.Game g = adding ? null : game();
            bigTitle(adding ? getString(R.string.lib_add) : g != null && g.ver != null ? versionName(g.ver) : getString(R.string.disc), 60, 92);
            float y = 124;
            if (adding) {   // the file being added (its name: the error below belongs to it, not to the selected game)
                String nm = addName;
                text(nunito, 19, Color.WHITE);
                String t = nm != null ? getString(R.string.disc_adding, nm) : getString(R.string.lib_add_sub);
                tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT); tp.setTextSize(19); tp.setColor(nm != null ? Color.WHITE : SUB);
                @SuppressWarnings("deprecation")
                StaticLayout l = new StaticLayout(t, tp, 860, Layout.Alignment.ALIGN_CENTER, 1.05f, 0, false);
                tmp.set(640 - 450, y, 640 + 450, y + Math.max(40, l.getHeight() + 14));
                p.setColor(CHIP); p.setStyle(Paint.Style.FILL);
                canvas.drawRoundRect(tmp, 20, 20, p);
                canvas.save(); canvas.translate(640 - 430, tmp.centerY() - l.getHeight() / 2f); l.draw(canvas); canvas.restore();
                y = tmp.bottom + 12;
            } else {
                File disc = discFile();
                boolean ok = g != null && okOf(g);
                String head = g == null ? getString(R.string.disc_missing) : statusOf(g) + "  ·  " + g.serial;
                tmp.set(640 - 300, y, 640 + 300, y + 40);
                p.setColor(CHIP); p.setStyle(Paint.Style.FILL);
                canvas.drawRoundRect(tmp, 20, 20, p);
                text(nunito, 19, ok ? SUB : ACCENT);
                drawFit((ok ? "✓  " : "✗  ") + head, 640, tmp, tmp.width() - 30);
                y = tmp.bottom + 10;
                tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT);
                tp.setTextSize(14); tp.setColor(0xd0ffffff);
                if (g != null && dataReady()) y += wrapped(getString(R.string.disc_data, g.name == null || g.name.isEmpty() ? "?" : g.name, g.info == null ? "" : g.info), y, 1060) + 12;
                else if (disc != null) y += wrapped(getString(R.string.disc_path, disc.getAbsolutePath()), y, 1060) + 12;
            }
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT);
            tp.setTextSize(16); tp.setColor(SUB);
            y += wrapped(getString(R.string.disc_howto), y, 1000) + 16;
            int ph = phase;
            if (ph != IDLE) { progressBar(ph, y + 6); y += 92; postInvalidateDelayed(100); }   // ~10 Hz while busy
            else if (result != 0 && importStatus != null) y = resultBox(result > 0, importStatus, y) + 14;
            else if (importStatus != null) { tp.setColor(Color.WHITE); tp.setTextSize(18); y += wrapped(importStatus, y) + 14; }
            if (confirmWipe) { tp.setColor(ACCENT); tp.setTextSize(18); y += wrapped(getString(R.string.disc_wipe_confirm), y) + 14; }
            // the buttons: from y (at least 384 as before), the spacing shrinks to fit above the bottom edge
            int n = items.size();
            float top = Math.max(y, n > 2 ? 384 : 400), bh = n > 2 ? 52 : 58, step = bh + 19;
            if (n > 0 && top + (n - 1) * step + bh > 708) { step = Math.max(bh + 6, (708 - top - bh) / Math.max(1, n - 1)); if (top + (n - 1) * step + bh > 708) top = 708 - (n - 1) * step - bh; }
            for (int i = 0; i < n; i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 260, top + i * step, 640 + 260, top + i * step + bh);
                pill(it.r, f ? Color.WHITE : CREAM, CREAM_SH, 5, f);
                text(nunito, 22, BG);
                if (it.value == null) drawFit(it.label, 640, it.r, it.r.width() - 40);
                else {
                    p.setTextAlign(Paint.Align.LEFT); canvas.drawText(it.label, it.r.left + 26, midBase(it.r), p);
                    p.setTextAlign(Paint.Align.RIGHT); canvas.drawText("‹  " + it.value + "  ›", it.r.right - 26, midBase(it.r), p);
                }
            }
        }
    }
}

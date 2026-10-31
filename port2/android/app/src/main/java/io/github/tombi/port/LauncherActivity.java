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
import android.view.View;
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
 * Fonts (OFL, assets/fonts) are fetched by the CI build; without them the system bold face is used.
 */
public class LauncherActivity extends Activity {
    static final int BG = 0xff24592f, CIRCLE_BL = 0xff1e4c28, CIRCLE_TR = 0xff266030, ACCENT = 0xffff6aa8,
            ACCENT_SH = 0xff7a2e00, ACCENT_TEXT = 0xff4a1530, CREAM = 0xfffff8e6, CREAM_SH = 0xff173d20,
            SUB = 0xfffff8e6, CHIP = 0x59000000, CHIP_STATUS = 0x80000000, BTN_TEXT = 0xff1f4d29,
            FOCUS = 0xffffe066, CARD = 0x47000000, SHADOW = 0x59000000, PINK = 0xffff8fbf;
    static final int[] LANG_N = {0, 5, 6, 7, 8};
    static final String[] LANG_NAME = {"English", "Español", "Português", "Svenska", "Dansk"};
    static final String[] ASPECTS = {"auto", "4:3", "16:9", "21:9"};
    static final int REQ_DISC = 2;
    static final String MAIN0_SHA1 = "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc";   // EXE/MAIN0.EXE of SCES-01331
    static final String TEXT_PATCH = "es_mejorada.tombipatch.json";

    SharedPreferences prefs;
    LauncherView view;
    volatile String importStatus;

    @Override
    protected void onCreate(Bundle b) {
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
        view.invalidate();
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
    boolean flag(String k, boolean def) { return prefs.getBoolean(k, def); }
    /* second screen panel (runtime/panel.c, TombiActivity): default on when the device has a secondary display */
    boolean secondScreenOn() {
        android.hardware.display.DisplayManager dm = (android.hardware.display.DisplayManager) getSystemService(DISPLAY_SERVICE);
        boolean has = dm != null && dm.getDisplays(android.hardware.display.DisplayManager.DISPLAY_CATEGORY_PRESENTATION).length > 0;
        return flag("secondscreen", has);
    }
    static int clamp(int v, int n) { return v < 0 || v >= n ? 0 : v; }

    File filesDir() {
        File f = getExternalFilesDir(null);
        return f != null ? f : getFilesDir();
    }

    File discFile() {
        String d = prefs.getString("disc", null);
        return d != null && new File(d).isFile() ? new File(d) : null;
    }
    boolean ready() { return discFile() != null; }
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
        if (disc != null) { a.add("--disc"); a.add(disc.getAbsolutePath()); }
        if (textPatchOn() && LANG_N[langIdx()] == 5) {
            File tp = textPatch();
            if (tp != null) { a.add("--text-patch"); a.add(tp.getAbsolutePath()); }
        }
        a.add("--lang"); a.add(String.valueOf(LANG_N[langIdx()]));
        a.add("--aspect"); a.add(ASPECTS[aspectIdx()]);
        a.add("--renderer"); a.add(flag("gl", false) ? "gl" : "soft");
        if (flag("cut", false)) {
            a.add("--cut-content");
            // English cut lines: tombi2 picks en_cut_content.tpatch from --patch-dir by itself (as the PC launcher does)
            if (LANG_N[langIdx()] == 0 && patchAsset("en_cut_content.tpatch") != null) { a.add("--patch-dir"); a.add(new File(filesDir(), "patches").getAbsolutePath()); }
        }
        if (flag("debugmenu", false)) a.add("--debug-menu");
        if (secondScreenOn()) a.add("--second-screen");
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
        Intent i = new Intent(this, TombiActivity.class);
        i.putExtra(TombiActivity.EXTRA_ARGS, buildArgs());
        startActivity(i);
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
    static final int IDLE = 0, COPYING = 1, CHECKING = 2;
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

    long sizeOf(Uri u) {
        try (Cursor c = getContentResolver().query(u, new String[]{android.provider.OpenableColumns.SIZE}, null, null, null)) {
            if (c != null && c.moveToFirst() && !c.isNull(0)) return c.getLong(0);
        } catch (Exception e) { /* unknown */ }
        return -1;
    }

    /** Copies the picked .bin/.iso/.chd (and .cue) into files/disc, checks MAIN0.EXE's SHA-1, remembers the image.
     *  Cancel (B / "Cancel") stops the copy and deletes the partial files. */
    void importDisc(List<Uri> uris) {
        List<File> written = new ArrayList<>();
        cancelImport = false; result = 0; copied = 0;
        long tot = 0;
        for (Uri u : uris) { long n = sizeOf(u); if (n < 0) { tot = -1; break; } tot += n; }
        copyTotal = tot;
        phase = COPYING;
        status(null);
        try {
            File dir = new File(filesDir(), "disc");
            dir.mkdirs();
            File image = null, cue = null;
            byte[] buf = new byte[1 << 18];
            for (Uri u : uris) {
                String name = displayName(u).replace('/', '_');
                String low = name.toLowerCase(Locale.ROOT);
                File dst = new File(dir, name);
                InputStream in = getContentResolver().openInputStream(u);
                if (in == null) throw new Exception(name);
                written.add(dst);
                OutputStream out = new FileOutputStream(dst);
                int n;
                try {
                    while ((n = in.read(buf)) > 0) {
                        if (cancelImport) throw new InterruptedException();
                        out.write(buf, 0, n);
                        copied += n;
                    }
                } finally { in.close(); out.close(); }
                if (low.endsWith(".cue")) cue = dst; else image = dst;
            }
            if (image == null) { fail(getString(R.string.disc_needbin)); return; }
            phase = CHECKING;
            view.postInvalidate();
            String r = nativeOk ? nativeMain0Sha1(image.getAbsolutePath()) : "err:libtombidisc.so missing";
            String sha = r.startsWith("sha1:") ? r.substring(5) : null;
            boolean ok = MAIN0_SHA1.equals(sha);
            prefs.edit().putString("disc", (cue != null ? cue : image).getAbsolutePath()).putBoolean("disc_ok", ok).apply();
            phase = IDLE;
            result = ok ? 1 : -1;
            status(ok ? getString(R.string.disc_ok) : sha != null ? getString(R.string.disc_badsha, sha)
                    : r.equals("err:nomain0") ? getString(R.string.disc_nomain0) : getString(R.string.disc_fail, r.substring(4)));
        } catch (InterruptedException e) {
            for (File f : written) f.delete();
            phase = IDLE; result = -1;
            status(getString(R.string.disc_cancelled));
        } catch (Exception e) {
            for (File f : written) f.delete();
            fail(getString(R.string.disc_fail, String.valueOf(e.getMessage())));
        }
    }

    void fail(String msg) { phase = IDLE; result = -1; status(msg); }

    @Override
    protected void onActivityResult(int req, int res, Intent data) {
        super.onActivityResult(req, res, data);
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
        return view.key(code) || super.onKeyDown(code, e);
    }

    // ---------------------------------------------------------------------------------------------------------------
    static class Item {
        String label; String value; RectF r = new RectF(); boolean primary;
        Runnable act; Adjust adj;
        Item(String l, Runnable a) { label = l; act = a; }
    }
    interface Adjust { void by(int d); }

    class LauncherView extends View {
        static final int MAIN = 0, OPTIONS = 1, DISC = 2;
        int screen = MAIN, focus = 0;
        int[] lastFocus = new int[3];
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

        void setScreen(int sc) {
            lastFocus[screen] = focus;
            screen = sc;
            focus = lastFocus[sc];
            if (sc != DISC && phase == IDLE) { importStatus = null; result = 0; }
            invalidate();
        }

        String onoff(boolean b) { return getString(b ? R.string.on : R.string.off); }

        Item toggle(int label, final String key) {
            final Item it = new Item(getString(label), null);
            it.value = onoff(flag(key, false));
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
            } else if (screen == OPTIONS) {
                Item l = new Item(getString(R.string.opt_lang), null);
                l.value = LANG_NAME[langIdx()];
                l.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("lang", (langIdx() + d + LANG_N.length) % LANG_N.length).apply(); } };
                items.add(l);
                Item as = new Item(getString(R.string.opt_aspect), null);
                as.value = aspectName(aspectIdx());
                as.adj = new Adjust() { public void by(int d) { prefs.edit().putInt("aspect", (aspectIdx() + d + ASPECTS.length) % ASPECTS.length).apply(); } };
                items.add(as);
                Item tpi = new Item(getString(R.string.opt_textpatch), null);
                tpi.value = onoff(textPatchOn());
                tpi.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("textpatch", !textPatchOn()).apply(); } };
                items.add(tpi);
                items.add(toggle(R.string.opt_cut, "cut"));
                items.add(toggle(R.string.opt_debug, "debugmenu"));
                Item ss = new Item(getString(R.string.opt_secondscreen), null);
                ss.value = onoff(secondScreenOn());
                ss.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("secondscreen", !secondScreenOn()).apply(); } };
                items.add(ss);
                Item r = new Item(getString(R.string.opt_renderer), null);
                r.value = getString(flag("gl", false) ? R.string.renderer_gl : R.string.renderer_soft);
                r.adj = new Adjust() { public void by(int d) { prefs.edit().putBoolean("gl", !flag("gl", false)).apply(); } };
                items.add(r);
                items.add(toggle(R.string.opt_skipintro, "skipintro"));
                items.add(toggle(R.string.opt_hz60, "hz60"));
                items.add(toggle(R.string.opt_fps60, "fps60"));
                items.add(toggle(R.string.opt_showfps, "showfps"));
                items.add(toggle(R.string.opt_inflives, "inflives"));
                items.add(toggle(R.string.opt_infhealth, "infhealth"));
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(MAIN); } }));
            } else if (phase != IDLE) {   // importing: the only action is Cancel (also B)
                items.add(new Item(getString(R.string.cancel), new Runnable() { public void run() { cancelImport = true; } }));
            } else {
                items.add(new Item(getString(R.string.disc_pick), new Runnable() { public void run() { pickDisc(); } }));
                items.add(new Item(getString(R.string.back), new Runnable() { public void run() { setScreen(MAIN); } }));
            }
            if (focus >= items.size()) focus = 0;
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
                    Item it = items.get(focus);
                    if (it.adj != null) it.adj.by(code == KeyEvent.KEYCODE_DPAD_LEFT ? -1 : 1);
                    break;
                }
                case KeyEvent.KEYCODE_BUTTON_A: case KeyEvent.KEYCODE_DPAD_CENTER: case KeyEvent.KEYCODE_ENTER:
                case KeyEvent.KEYCODE_NUMPAD_ENTER: case KeyEvent.KEYCODE_SPACE:
                    activate(items.get(focus)); break;
                case KeyEvent.KEYCODE_BUTTON_B: case KeyEvent.KEYCODE_BACK: case KeyEvent.KEYCODE_ESCAPE:
                    if (phase != IDLE) cancelImport = true;
                    else if (screen == MAIN) finish(); else setScreen(MAIN);
                    break;
                case KeyEvent.KEYCODE_BUTTON_START:
                    if (phase != IDLE) break;
                    if (screen == MAIN) play(); else setScreen(MAIN);
                    break;
                default: return false;
            }
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
            float x = (e.getX() - ox) / s, y = (e.getY() - oy) / s;
            int hit = -1;
            for (int i = 0; i < items.size(); i++) if (items.get(i).r.contains(x, y)) hit = i;
            if (e.getActionMasked() == MotionEvent.ACTION_DOWN) {
                if (hit >= 0) { focus = hit; invalidate(); }
                return true;
            }
            if (e.getActionMasked() == MotionEvent.ACTION_UP && hit >= 0 && hit == focus) {
                build();
                if (hit < items.size()) activate(items.get(hit));
            }
            return true;
        }

        // ---- drawing ----
        void text(Typeface tf, float size, int color) {
            p.setTypeface(tf); p.setTextSize(size); p.setColor(color); p.setLetterSpacing(0);
            p.setTextAlign(Paint.Align.CENTER); p.setStyle(Paint.Style.FILL); p.clearShadowLayer();
        }

        void shadowOn() { p.setShadowLayer(1.5f, 0, 2, SHADOW); }

        float midBase(RectF r) { return r.centerY() - (p.descent() + p.ascent()) / 2; }

        void pill(RectF r, int bg, int shadow, float sh, boolean focused) {
            float rad = r.height() / 2;
            if (focused) {   // thick yellow focus ring around the pill and its shadow
                p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(7); p.setColor(FOCUS);
                tmp.set(r.left - 9, r.top - 9, r.right + 9, r.bottom + sh + 9);
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
            else if (screen == OPTIONS) drawOptions();
            else drawDisc();
            c.restore();
        }

        void drawMain() {
            bigTitle("TOMBI!", 108, 170);
            text(nunito, 22, SUB); shadowOn();
            p.setLetterSpacing(3f / 22f);
            canvas.drawText(getString(R.string.subtitle), 640, 214, p);
            float y = 256;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                float hh = it.primary ? 78 : 62;
                it.r.set(640 - 230, y, 640 + 230, y + hh);
                boolean f = i == focus;
                if (it.primary) pill(it.r, ACCENT, ACCENT_SH, 6, f);
                else pill(it.r, CREAM, CREAM_SH, 5, f);
                text(nunito, it.primary ? 30 : 26, it.primary ? ACCENT_TEXT : BTN_TEXT);
                canvas.drawText(it.label, 640, midBase(it.r), p);
                y += hh + (it.primary ? 6 : 5) + 16;
            }
            String[] chips = {LANG_NAME[langIdx()], aspectName(aspectIdx()),
                    getString(flag("cut", false) ? R.string.chip_cut_on : R.string.chip_cut_off), version()};
            text(nunito, 22, SUB);
            float pad = 18, gap = 12, total = -gap;
            for (String t : chips) total += p.measureText(t) + 2 * pad + gap;
            float x = 640 - total / 2;
            for (String t : chips) {
                float cw = p.measureText(t) + 2 * pad;
                tmp.set(x, 640, x + cw, 686);
                p.setColor(CHIP);
                canvas.drawRoundRect(tmp, 23, 23, p);
                p.setColor(SUB);
                canvas.drawText(t, tmp.centerX(), midBase(tmp), p);
                x += cw + gap;
            }
        }

        void drawOptions() {
            bigTitle(getString(R.string.options), 50, 66);
            float y = 88;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 380, y, 640 + 380, y + 48);
                pill(it.r, f ? ACCENT : CREAM, f ? ACCENT_SH : CREAM_SH, 4, f);
                int col = f ? ACCENT_TEXT : BTN_TEXT;
                text(nunito, 24, col);
                if (it.value == null) {
                    canvas.drawText(it.label, 640, midBase(it.r), p);
                } else {
                    p.setTextAlign(Paint.Align.LEFT);
                    canvas.drawText(it.label, it.r.left + 26, midBase(it.r), p);
                    p.setTextAlign(Paint.Align.RIGHT);
                    canvas.drawText("‹  " + it.value + "  ›", it.r.right - 26, midBase(it.r), p);
                }
                y += 62;
            }
            tp.setTypeface(nunito); tp.setTextSize(22); tp.setColor(SUB);
            tp.setTextAlign(Paint.Align.LEFT);
            wrapped(getString(R.string.opt_hint), Math.max(y + 10, 560), true);
        }

        void wrapped(String t, float top) { wrapped(t, top, false); }

        /** Body text (Nunito, soft dark shadow); card=true puts it on a translucent dark rounded card. */
        float wrapped(String t, float top, boolean card) {
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout(t, tp, 900, Layout.Alignment.ALIGN_CENTER, 1.1f, 0, false);
            float lh = l.getHeight();
            if (card) {
                p.setStyle(Paint.Style.FILL); p.setColor(CARD); p.clearShadowLayer();
                tmp.set(640 - 450 - 28, top, 640 + 450 + 28, top + lh + 40);
                canvas.drawRoundRect(tmp, 18, 18, p);
                top += 20;
            }
            tp.setShadowLayer(1.5f, 0, 2, SHADOW);
            canvas.save();
            canvas.translate(640 - 450, top);
            l.draw(canvas);
            canvas.restore();
            return lh + (card ? 40 : 0);
        }

        /** Selva pill bar: dark track, accent fill with its shadow edge (as the buttons), phase / % / MB labels. */
        void progressBar(int ph, float top) {
            long done = copied, tot = copyTotal;
            float frac;
            if (ph == CHECKING) frac = nativeOk ? Math.max(0, Math.min(1, nativeCheckProgress() / 1000f)) : 0;
            else frac = tot > 0 ? Math.min(1f, done / (float) tot) : 0;
            String label = getString(ph == CHECKING ? R.string.disc_phase_check : R.string.disc_phase_copy);
            text(nunito, 22, SUB); shadowOn();
            p.setTextAlign(Paint.Align.LEFT);
            canvas.drawText(label, 640 - 300, top + 6, p);
            p.setTextAlign(Paint.Align.RIGHT);
            String pct = ph == CHECKING || tot > 0 ? Math.round(frac * 100) + " %" : "";
            canvas.drawText(pct, 640 + 300, top + 6, p);
            RectF track = new RectF(640 - 300, top + 20, 640 + 300, top + 50);
            float rad = track.height() / 2;
            p.clearShadowLayer(); p.setColor(0x59000000);
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
            text(nunito, 22, SUB); shadowOn();
            String mb = ph == CHECKING ? "MAIN0.EXE (SHA-1)"
                    : tot > 0 ? (done >> 20) + " / " + (tot >> 20) + " MB" : (done >> 20) + " MB";
            canvas.drawText(mb, 640, top + 78, p);
        }

        void resultBox(boolean ok, String msg, float top) {
            tmp.set(640 - 320, top, 640 + 320, top + 80);
            p.setStyle(Paint.Style.FILL); p.clearShadowLayer();
            p.setColor(ok ? 0xff14522a : 0xff7a1d3d);
            canvas.drawRoundRect(tmp, 22, 22, p);
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT); tp.setTextSize(22); tp.setColor(SUB); tp.clearShadowLayer();
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout((ok ? "✓  " : "✗  ") + msg, tp, 600, Layout.Alignment.ALIGN_CENTER, 1.0f, 0, false);
            canvas.save();
            canvas.translate(640 - 300, tmp.centerY() - l.getHeight() / 2f);
            l.draw(canvas);
            canvas.restore();
        }

        void drawDisc() {
            bigTitle(getString(R.string.disc), 60, 92);
            File disc = discFile();
            boolean ok = disc != null && prefs.getBoolean("disc_ok", false);
            String head = getString(disc == null ? R.string.disc_missing : ok ? R.string.disc_ok : R.string.disc_unverified);
            tmp.set(640 - 300, 112, 640 + 300, 158);
            p.setColor(CHIP_STATUS); p.setStyle(Paint.Style.FILL);
            canvas.drawRoundRect(tmp, 23, 23, p);
            text(nunito, 24, ok ? SUB : PINK);
            canvas.drawText((ok ? "✓  " : "✗  ") + head, 640, midBase(tmp), p);
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT);
            tp.setTextSize(22); tp.setColor(SUB);
            if (disc != null) {
                CharSequence pth = android.text.TextUtils.ellipsize(getString(R.string.disc_path, disc.getAbsolutePath()), tp, 900, android.text.TextUtils.TruncateAt.MIDDLE);
                wrapped(pth.toString(), 166);
            }
            wrapped(getString(R.string.disc_howto), 206, true);
            int ph = phase;
            if (ph != IDLE) { progressBar(ph, 352); postInvalidateDelayed(100); }   // ~10 Hz while busy
            else if (result != 0 && importStatus != null) resultBox(result > 0, importStatus, 352);
            else if (importStatus != null) { tp.setColor(SUB); wrapped(importStatus, 360); }
            float y = 460;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 230, y, 640 + 230, y + 58);
                pill(it.r, CREAM, CREAM_SH, 5, f);
                text(nunito, 26, BTN_TEXT);
                canvas.drawText(it.label, 640, midBase(it.r), p);
                y += 58 + 5 + 22;
            }
        }
    }
}

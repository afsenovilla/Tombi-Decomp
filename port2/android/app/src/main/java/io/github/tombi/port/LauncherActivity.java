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
import java.io.FileInputStream;
import java.io.RandomAccessFile;
import java.security.MessageDigest;
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
    static final int BG = 0xff2f6b3a, CIRCLE_BL = 0xff275a31, CIRCLE_TR = 0xff3a7c45, ACCENT = 0xffff6aa8,
            ACCENT_SH = 0xff7a2e00, ACCENT_TEXT = 0xff4a1530, CREAM = 0xfffff8e6, CREAM_SH = 0xff1d4424,
            SUB = 0xffffe8a8, CHIP = 0x40000000;
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

    /** Copies the picked .bin/.iso (and .cue) into files/disc, checks MAIN0.EXE's SHA-1, remembers the image. */
    void importDisc(List<Uri> uris) {
        try {
            File dir = new File(filesDir(), "disc");
            dir.mkdirs();
            File image = null, cue = null;
            byte[] buf = new byte[1 << 16];
            for (Uri u : uris) {
                String name = displayName(u).replace('/', '_');
                String low = name.toLowerCase(Locale.ROOT);
                File dst = new File(dir, name);
                InputStream in = getContentResolver().openInputStream(u);
                if (in == null) throw new Exception(name);
                OutputStream out = new FileOutputStream(dst);
                long total = 0; int n;
                try {
                    while ((n = in.read(buf)) > 0) {
                        out.write(buf, 0, n);
                        if (((total += n) & ((32 << 20) - 1)) < n) status(getString(R.string.disc_copying, total >> 20));
                    }
                } finally { in.close(); out.close(); }
                if (low.endsWith(".cue")) cue = dst; else image = dst;
            }
            if (image == null) { status(getString(R.string.disc_needbin)); return; }
            status(getString(R.string.disc_checking));
            String sha = main0Sha1(image);
            prefs.edit().putString("disc", (cue != null ? cue : image).getAbsolutePath())
                    .putBoolean("disc_ok", MAIN0_SHA1.equals(sha)).apply();
            status(sha == null ? getString(R.string.disc_nomain0) : MAIN0_SHA1.equals(sha)
                    ? getString(R.string.disc_ok) : getString(R.string.disc_badsha, sha));
        } catch (Exception e) {
            status(getString(R.string.disc_fail, String.valueOf(e.getMessage())));
        }
    }

    // ---- ISO 9660 reader (2048-byte .iso or raw 2352-byte .bin, Mode 1 or Mode 2 Form 1) ----
    RandomAccessFile isoF; int secSize, secOff;

    byte[] sector(int lba) throws Exception {
        byte[] b = new byte[2048];
        isoF.seek((long) lba * secSize + secOff);
        isoF.readFully(b);
        return b;
    }
    static int le32(byte[] b, int o) { return (b[o] & 255) | (b[o + 1] & 255) << 8 | (b[o + 2] & 255) << 16 | (b[o + 3] & 255) << 24; }

    /** {lba, size} of the entry `name` in the directory at (lba, size), or null. */
    int[] findEntry(int lba, int size, String name) throws Exception {
        for (int s = 0; s < (size + 2047) / 2048; s++) {
            byte[] b = sector(lba + s);
            for (int o = 0; o < 2048 && (b[o] & 255) > 0; o += b[o] & 255) {
                int nl = b[o + 32] & 255;
                String n = new String(b, o + 33, nl, "ISO-8859-1");
                int semi = n.indexOf(';'); if (semi >= 0) n = n.substring(0, semi);
                if (n.equalsIgnoreCase(name)) return new int[]{le32(b, o + 2), le32(b, o + 10)};
            }
        }
        return null;
    }

    String main0Sha1(File image) {
        try {
            isoF = new RandomAccessFile(image, "r");
            if (isoF.length() % 2352 == 0) {
                secSize = 2352;
                isoF.seek(16L * 2352 + 15);
                secOff = isoF.read() == 2 ? 24 : 16;
            } else { secSize = 2048; secOff = 0; }
            byte[] pvd = sector(16);
            int[] dir = findEntry(le32(pvd, 156 + 2), le32(pvd, 156 + 10), "EXE");
            int[] f = dir != null ? findEntry(dir[0], dir[1], "MAIN0.EXE") : null;
            if (f == null) return null;
            MessageDigest md = MessageDigest.getInstance("SHA-1");
            for (int left = f[1], l = f[0]; left > 0; left -= 2048, l++) md.update(sector(l), 0, Math.min(2048, left));
            StringBuilder sb = new StringBuilder();
            for (byte x : md.digest()) sb.append(String.format("%02x", x & 255));
            return sb.toString();
        } catch (Exception e) {
            return null;
        } finally {
            try { if (isoF != null) isoF.close(); } catch (Exception e) { /* ignore */ }
            isoF = null;
        }
    }

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

    void status(String s) { importStatus = s; view.postInvalidate(); }

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
            importStatus = sc == DISC ? importStatus : null;
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
                    if (screen == MAIN) finish(); else setScreen(MAIN);
                    break;
                case KeyEvent.KEYCODE_BUTTON_START:
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
            else if (screen == OPTIONS) drawOptions();
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

        void drawOptions() {
            bigTitle(getString(R.string.options), 50, 66);
            float y = 88;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 380, y, 640 + 380, y + 38);
                pill(it.r, f ? ACCENT : CREAM, f ? ACCENT_SH : CREAM_SH, 4, f);
                int col = f ? ACCENT_TEXT : BG;
                text(nunito, 19, col);
                if (it.value == null) {
                    canvas.drawText(it.label, 640, midBase(it.r), p);
                } else {
                    p.setTextAlign(Paint.Align.LEFT);
                    canvas.drawText(it.label, it.r.left + 26, midBase(it.r), p);
                    p.setTextAlign(Paint.Align.RIGHT);
                    canvas.drawText("‹  " + it.value + "  ›", it.r.right - 26, midBase(it.r), p);
                }
                y += 44;
            }
            tp.setTypeface(nunito); tp.setTextSize(14); tp.setColor(SUB);
            tp.setTextAlign(Paint.Align.LEFT);
            wrapped(getString(R.string.opt_hint), 684);
        }

        void wrapped(String t, float top) {
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout(t, tp, 900, Layout.Alignment.ALIGN_CENTER, 1.1f, 0, false);
            canvas.save();
            canvas.translate(640 - 450, top);
            l.draw(canvas);
            canvas.restore();
        }

        void drawDisc() {
            bigTitle(getString(R.string.disc), 60, 92);
            File disc = discFile();
            boolean ok = disc != null && prefs.getBoolean("disc_ok", false);
            String head = getString(disc == null ? R.string.disc_missing : ok ? R.string.disc_ok : R.string.disc_unverified);
            tmp.set(640 - 300, 124, 640 + 300, 164);
            p.setColor(CHIP); p.setStyle(Paint.Style.FILL);
            canvas.drawRoundRect(tmp, 20, 20, p);
            text(nunito, 19, ok ? SUB : ACCENT);
            canvas.drawText((ok ? "✓  " : "✗  ") + head, 640, midBase(tmp), p);
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT);
            tp.setTextSize(16); tp.setColor(Color.WHITE);
            if (disc != null) wrapped(getString(R.string.disc_path, disc.getAbsolutePath()), 178);
            tp.setColor(SUB);
            wrapped(getString(R.string.disc_howto), 222);
            String st = importStatus;
            if (st != null) { tp.setColor(Color.WHITE); tp.setTextSize(18); wrapped(st, 330); }
            float y = 400;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 230, y, 640 + 230, y + 58);
                pill(it.r, f ? Color.WHITE : CREAM, CREAM_SH, 5, f);
                text(nunito, 22, BG);
                canvas.drawText(it.label, 640, midBase(it.r), p);
                y += 58 + 5 + 18;
            }
        }
    }
}

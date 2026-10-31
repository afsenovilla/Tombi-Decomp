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
import android.provider.DocumentsContract;
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
 * SharedPreferences, turned into port flags for TombiActivity), disc/data status with a SAF folder import, quit.
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
    static final int REQ_TREE = 1;

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
    File dataDir() { return new File(filesDir(), "data"); }
    boolean dataValid() {
        File m = new File(dataDir(), "manifest.txt");
        return m.isFile() && m.length() > 0;
    }

    String[] buildArgs() {
        List<String> a = new ArrayList<>();
        a.add("--lang"); a.add(String.valueOf(LANG_N[langIdx()]));
        a.add("--aspect"); a.add(ASPECTS[aspectIdx()]);
        a.add("--renderer"); a.add(flag("gl", false) ? "gl" : "soft");
        if (flag("cut", false)) a.add("--cut-content");
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
        if (!dataValid()) {
            Toast.makeText(this, R.string.need_data, Toast.LENGTH_LONG).show();
            view.setScreen(LauncherView.DATA);
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

    // ---- SAF import ----
    void pickTree() {
        try {
            startActivityForResult(new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE), REQ_TREE);
        } catch (ActivityNotFoundException e) {
            importStatus = getString(R.string.data_nosaf);
            view.invalidate();
        }
    }

    @Override
    protected void onActivityResult(int req, int res, Intent data) {
        super.onActivityResult(req, res, data);
        if (req != REQ_TREE || res != RESULT_OK || data == null || data.getData() == null) return;
        final Uri tree = data.getData();
        new Thread(new Runnable() { public void run() { importTree(tree); } }, "tombi-import").start();
    }

    static class Doc { String id, name; boolean dir; }

    List<Doc> children(Uri tree, String docId) {
        List<Doc> out = new ArrayList<>();
        Uri u = DocumentsContract.buildChildDocumentsUriUsingTree(tree, docId);
        Cursor c = getContentResolver().query(u, new String[]{DocumentsContract.Document.COLUMN_DOCUMENT_ID,
                DocumentsContract.Document.COLUMN_DISPLAY_NAME, DocumentsContract.Document.COLUMN_MIME_TYPE}, null, null, null);
        if (c == null) return out;
        try {
            while (c.moveToNext()) {
                Doc d = new Doc();
                d.id = c.getString(0); d.name = c.getString(1);
                d.dir = DocumentsContract.Document.MIME_TYPE_DIR.equals(c.getString(2));
                out.add(d);
            }
        } finally { c.close(); }
        return out;
    }

    static Doc find(List<Doc> l, String name, boolean dir) {
        for (Doc d : l) if (d.dir == dir && d.name != null && d.name.equalsIgnoreCase(name)) return d;
        return null;
    }

    int copied;

    void importTree(Uri tree) {
        try {
            String root = DocumentsContract.getTreeDocumentId(tree);
            List<Doc> top = children(tree, root);
            if (find(top, "manifest.txt", false) == null) {
                Doc sub = find(top, "data", true);
                if (sub == null || find(children(tree, sub.id), "manifest.txt", false) == null) {
                    status(getString(R.string.data_bad));
                    return;
                }
                root = sub.id;
            }
            copied = 0;
            File dst = dataDir();
            dst.mkdirs();
            copyDir(tree, root, dst);
            status(getString(R.string.data_done, copied));
        } catch (Exception e) {
            status(getString(R.string.data_fail, String.valueOf(e.getMessage())));
        }
    }

    void copyDir(Uri tree, String docId, File dst) throws Exception {
        byte[] buf = new byte[1 << 16];
        for (Doc d : children(tree, docId)) {
            File f = new File(dst, d.name);
            if (d.dir) { f.mkdirs(); copyDir(tree, d.id, f); continue; }
            InputStream in = getContentResolver().openInputStream(DocumentsContract.buildDocumentUriUsingTree(tree, d.id));
            if (in == null) throw new Exception(d.name);
            OutputStream out = new FileOutputStream(f);
            try { int n; while ((n = in.read(buf)) > 0) out.write(buf, 0, n); }
            finally { in.close(); out.close(); }
            if (++copied % 25 == 0) status(getString(R.string.data_copying, copied));
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
        static final int MAIN = 0, OPTIONS = 1, DATA = 2;
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
            importStatus = sc == DATA ? importStatus : null;
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
                items.add(new Item(getString(R.string.data), new Runnable() { public void run() { setScreen(DATA); } }));
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
                items.add(new Item(getString(R.string.data_import), new Runnable() { public void run() { pickTree(); } }));
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
            else drawData();
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
            bigTitle(getString(R.string.options), 52, 68);
            float y = 92;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 380, y, 640 + 380, y + 40);
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
                y += 47;
            }
            tp.setTypeface(nunito); tp.setTextSize(14); tp.setColor(SUB);
            tp.setTextAlign(Paint.Align.LEFT);
            wrapped(getString(R.string.opt_hint), 680);
        }

        void wrapped(String t, float top) {
            @SuppressWarnings("deprecation")
            StaticLayout l = new StaticLayout(t, tp, 900, Layout.Alignment.ALIGN_CENTER, 1.1f, 0, false);
            canvas.save();
            canvas.translate(640 - 450, top);
            l.draw(canvas);
            canvas.restore();
        }

        void drawData() {
            bigTitle(getString(R.string.data), 64, 110);
            boolean ok = dataValid();
            tmp.set(640 - 250, 150, 640 + 250, 192);
            p.setColor(CHIP); p.setStyle(Paint.Style.FILL);
            canvas.drawRoundRect(tmp, 21, 21, p);
            text(nunito, 20, ok ? SUB : ACCENT);
            canvas.drawText((ok ? "✓  " : "✗  ") + getString(ok ? R.string.data_ok : R.string.data_missing), 640, midBase(tmp), p);
            tp.setTypeface(nunito); tp.setTextAlign(Paint.Align.LEFT);
            tp.setTextSize(17); tp.setColor(Color.WHITE);
            wrapped(getString(R.string.data_path, dataDir().getAbsolutePath()), 214);
            tp.setTextSize(17); tp.setColor(SUB);
            wrapped(getString(R.string.data_howto), 270);
            String st = importStatus;
            if (st != null) { tp.setColor(Color.WHITE); tp.setTextSize(19); wrapped(st, 380); }
            float y = 450;
            for (int i = 0; i < items.size(); i++) {
                Item it = items.get(i);
                boolean f = i == focus;
                it.r.set(640 - 230, y, 640 + 230, y + 62);
                pill(it.r, f ? Color.WHITE : CREAM, CREAM_SH, 5, f);
                text(nunito, 24, BG);
                canvas.drawText(it.label, 640, midBase(it.r), p);
                y += 62 + 5 + 18;
            }
        }
    }
}

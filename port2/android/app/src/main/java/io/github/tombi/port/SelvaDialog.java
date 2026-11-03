package io.github.tombi.port;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.os.Build;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

/**
 * A dialog drawn in the launcher's own "Selva" style instead of the system AlertDialog: a dark-green rounded panel
 * over the dimmed screen, the title in Lilita One (pink with its shadow, as the launcher's titles), the message in
 * Nunito, and the choices as the launcher's pill buttons (the focused one pink with the white ring, the others cream).
 * An overlay View laid over the activity's content (the launcher's LauncherView, the game's SDL surface), GONE until
 * show(); it eats every touch while shown.  Keys come from the activity (key()): D-pad moves between the buttons,
 * A / Enter chooses, B / Back / Esc is the cancel choice.  Same 1280x720 virtual space scaled to the view as the
 * launcher.
 */
final class SelvaDialog extends View {
    String title, msg;
    String[] btns = new String[0];
    Runnable[] acts = new Runnable[0];
    RectF[] r = new RectF[0];
    int cancel, focus, pressed = -1;
    final RectF panel = new RectF(), tmp = new RectF();
    final Paint p = new Paint(Paint.ANTI_ALIAS_FLAG);
    final TextPaint tp = new TextPaint(Paint.ANTI_ALIAS_FLAG);
    final Typeface lilita, nunito;
    float s = 1, ox, oy;
    StaticLayout body;
    boolean stacked;

    SelvaDialog(Context c) {
        super(c);
        Typeface l = font(c, "fonts/LilitaOne-Regular.ttf", 0), n = font(c, "fonts/Nunito.ttf", 800);
        lilita = l != null ? l : Typeface.create(Typeface.SANS_SERIF, Typeface.BOLD);
        nunito = n != null ? n : Typeface.create(Typeface.SANS_SERIF, Typeface.BOLD);
        setVisibility(GONE);
    }

    static Typeface font(Context c, String path, int weight) {
        try {
            if (weight > 0 && Build.VERSION.SDK_INT >= 26)
                return new Typeface.Builder(c.getAssets(), path).setFontVariationSettings("'wght' " + weight).build();
            return Typeface.createFromAsset(c.getAssets(), path);
        } catch (Exception e) { return null; }
    }

    /** shows the dialog (replacing one already shown): title (null: none), message, the buttons and what each does
     *  (null: just close), the index B / Back chooses.  Focus starts on the first button. */
    void show(String title, String msg, String[] btns, Runnable[] acts, int cancel) {
        show(title, msg, btns, acts, cancel, 0);
    }
    /** ... with the focus starting on button `focus` */
    void show(String title, String msg, String[] btns, Runnable[] acts, int cancel, int focus) {
        this.title = title; this.msg = msg == null ? "" : msg; this.btns = btns; this.acts = acts;
        this.cancel = Math.max(0, Math.min(cancel, btns.length - 1));
        r = new RectF[btns.length];
        for (int i = 0; i < r.length; i++) r[i] = new RectF();
        this.focus = Math.max(0, Math.min(focus, btns.length - 1)); pressed = -1; body = null;
        setVisibility(VISIBLE);
        bringToFront();
        invalidate();
    }

    boolean showing() { return getVisibility() == VISIBLE; }

    /** closes the dialog and runs button i's action (which may show another dialog) */
    void choose(int i) {
        setVisibility(GONE);
        Runnable a = i >= 0 && i < acts.length ? acts[i] : null;
        if (a != null) a.run();
    }
    void cancel() { choose(cancel); }

    /** a key pressed while shown (always consumed).  A held key's repeats move the focus but never choose, so
     *  holding A on one dialog does not also answer the next one. */
    boolean key(int code, int repeat) {
        int n = btns.length;
        if (n == 0) { setVisibility(GONE); return true; }
        switch (code) {
            case KeyEvent.KEYCODE_DPAD_LEFT: case KeyEvent.KEYCODE_DPAD_UP:
                focus = (focus + n - 1) % n; break;
            case KeyEvent.KEYCODE_DPAD_RIGHT: case KeyEvent.KEYCODE_DPAD_DOWN:
                focus = (focus + 1) % n; break;
            case KeyEvent.KEYCODE_BUTTON_A: case KeyEvent.KEYCODE_DPAD_CENTER: case KeyEvent.KEYCODE_ENTER:
            case KeyEvent.KEYCODE_NUMPAD_ENTER: case KeyEvent.KEYCODE_SPACE: case KeyEvent.KEYCODE_BUTTON_START:
                if (repeat == 0) choose(focus);
                break;
            case KeyEvent.KEYCODE_BUTTON_B: case KeyEvent.KEYCODE_BACK: case KeyEvent.KEYCODE_ESCAPE:
                if (repeat == 0) cancel();
                break;
            default:
                return true;
        }
        invalidate();
        return true;
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        float x = (e.getX() - ox) / s, y = (e.getY() - oy) / s;
        int hit = -1;
        for (int i = 0; i < r.length; i++) {
            tmp.set(r[i]); tmp.inset(-6, -8);   // a little slack around the pills
            if (tmp.contains(x, y)) hit = i;
        }
        switch (e.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
                pressed = hit;
                if (hit >= 0) focus = hit;
                invalidate();
                break;
            case MotionEvent.ACTION_UP:
                if (hit >= 0 && hit == pressed) { pressed = -1; choose(hit); }
                else { pressed = -1; invalidate(); }
                break;
            case MotionEvent.ACTION_CANCEL:
                pressed = -1; invalidate();
                break;
        }
        return true;
    }

    // ---- drawing (LauncherActivity's colours) ----
    static final float W = 840, PAD = 44, BTN_H = 62, BTN_GAP = 22;

    void text(Typeface tf, float size, int color) {
        p.setTypeface(tf); p.setTextSize(size); p.setColor(color); p.setLetterSpacing(0);
        p.setTextAlign(Paint.Align.CENTER); p.setStyle(Paint.Style.FILL);
    }

    @SuppressWarnings("deprecation")
    StaticLayout layoutBody(float size) {
        tp.setTypeface(nunito); tp.setTextSize(size); tp.setColor(LauncherActivity.CREAM);
        tp.setTextAlign(Paint.Align.LEFT);
        return new StaticLayout(msg, tp, (int) (W - 2 * PAD), Layout.Alignment.ALIGN_CENTER, 1.15f, 0, false);
    }

    /** pill with its shadow, as LauncherView.pill */
    void pill(Canvas c, RectF b, int bg, int shadow, float sh, boolean focused) {
        float rad = b.height() / 2;
        if (focused) {
            p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(5); p.setColor(Color.WHITE);
            tmp.set(b.left - 7, b.top - 7, b.right + 7, b.bottom + sh + 7);
            c.drawRoundRect(tmp, tmp.height() / 2, tmp.height() / 2, p);
            p.setStyle(Paint.Style.FILL);
        }
        p.setColor(shadow);
        tmp.set(b.left, b.top + sh, b.right, b.bottom + sh);
        c.drawRoundRect(tmp, rad, rad, p);
        p.setColor(bg);
        c.drawRoundRect(b, rad, rad, p);
    }

    @Override
    protected void onDraw(Canvas c) {
        int w = getWidth(), h = getHeight();
        s = Math.min(w / 1280f, h / 720f);
        ox = (w - 1280 * s) / 2; oy = (h - 720 * s) / 2;
        c.drawColor(0xb0000000);
        c.save();
        c.translate(ox, oy);
        c.scale(s, s);
        int n = btns.length;
        // buttons: one row when they fit, else one under the other
        text(nunito, 24, LauncherActivity.BG);
        float rowW = -BTN_GAP;
        float[] bw = new float[n];
        for (int i = 0; i < n; i++) { bw[i] = Math.max(190, p.measureText(btns[i]) + 70); rowW += bw[i] + BTN_GAP; }
        stacked = rowW > W - 2 * PAD;
        float btnsH = n == 0 ? 0 : stacked ? n * BTN_H + (n - 1) * BTN_GAP : BTN_H;
        float titleH = title == null ? 0 : 72;
        // the message, shrunk until the panel fits the screen
        float size = 23, maxH = 690 - (PAD + titleH + 26 + btnsH + PAD + 8);
        if (body == null) {
            body = layoutBody(size);
            while (body.getHeight() > maxH && size > 13) { size -= 1.5f; body = layoutBody(size); }
        }
        float bodyH = msg.isEmpty() ? 0 : body.getHeight();
        float ph = PAD + titleH + bodyH + (n > 0 ? 34 : 0) + btnsH + PAD;
        float top = Math.max(12, 360 - ph / 2);
        panel.set(640 - W / 2, top, 640 + W / 2, top + ph);
        // panel: shadow, fill, light rim
        p.setStyle(Paint.Style.FILL);
        p.setColor(LauncherActivity.CREAM_SH);
        tmp.set(panel.left, panel.top + 10, panel.right, panel.bottom + 10);
        c.drawRoundRect(tmp, 36, 36, p);
        p.setColor(LauncherActivity.BG);
        c.drawRoundRect(panel, 36, 36, p);
        p.setStyle(Paint.Style.STROKE); p.setStrokeWidth(4); p.setColor(LauncherActivity.CIRCLE_TR);
        tmp.set(panel); tmp.inset(2, 2);
        c.drawRoundRect(tmp, 34, 34, p);
        p.setStyle(Paint.Style.FILL);
        float y = panel.top + PAD;
        if (title != null) {   // as LauncherView.bigTitle: pink over its brown shadow, shrunk to the panel
            float ts = 46;
            text(lilita, ts, LauncherActivity.ACCENT_SH);
            float tw = p.measureText(title);
            if (tw > W - 2 * PAD) { ts = ts * (W - 2 * PAD) / tw; p.setTextSize(ts); }
            float base = y + ts * 0.9f;
            c.drawText(title, 640, base + ts * 6 / 108, p);
            p.setColor(LauncherActivity.ACCENT);
            c.drawText(title, 640, base, p);
            y += titleH;
        }
        if (!msg.isEmpty()) {
            c.save();
            c.translate(panel.left + PAD, y);
            body.draw(c);
            c.restore();
            y += bodyH;
        }
        y += 34;
        float x = 640 - rowW / 2;
        for (int i = 0; i < n; i++) {
            if (stacked) r[i].set(640 - 300, y, 640 + 300, y + BTN_H);
            else { r[i].set(x, y, x + bw[i], y + BTN_H); x += bw[i] + BTN_GAP; }
            boolean f = i == focus;
            pill(c, r[i], f ? LauncherActivity.ACCENT : i == pressed ? Color.WHITE : LauncherActivity.CREAM,
                    f ? LauncherActivity.ACCENT_SH : LauncherActivity.CREAM_SH, 5, f);
            text(nunito, 24, f ? LauncherActivity.ACCENT_TEXT : LauncherActivity.BG);
            float tw = p.measureText(btns[i]), mw = r[i].width() - 40;
            if (tw > mw) p.setTextSize(24 * mw / tw);
            c.drawText(btns[i], r[i].centerX(), r[i].centerY() - (p.descent() + p.ascent()) / 2, p);
            if (stacked) y += BTN_H + BTN_GAP;
        }
        c.restore();
    }
}

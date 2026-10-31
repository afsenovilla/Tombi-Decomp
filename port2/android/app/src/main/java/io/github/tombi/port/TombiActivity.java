package io.github.tombi.port;

import android.app.Presentation;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.hardware.display.DisplayManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;

import java.nio.ByteBuffer;

import org.libsdl.app.SDLActivity;

/** The game: SDL2's activity, with the launcher's arguments handed to SDL_main (port2/android/loader.c).
 *  With --second-screen (launcher: "Segunda pantalla: mapa y estado") the port's map + status panel
 *  (port2/runtime/panel.c) is shown on a secondary display (dual-screen handhelds) through a Presentation;
 *  no secondary display: nothing changes; displays added / removed while playing are followed. */
public class TombiActivity extends SDLActivity {
    public static final String EXTRA_ARGS = "io.github.tombi.port.ARGS";

    /* registered at run time by libtombi.so (runtime/panel.c, RegisterNatives on this class) */
    static native int nativePanelFrame(ByteBuffer dst, int[] dims, int lastSeq);
    static native void nativePanelLayout(int w, int h);

    private DisplayManager dm;
    private PanelPresentation panel;
    private final Handler ui = new Handler(Looper.getMainLooper());
    private final DisplayManager.DisplayListener listener = new DisplayManager.DisplayListener() {
        @Override public void onDisplayAdded(int id) { updatePanel(); }
        @Override public void onDisplayRemoved(int id) { updatePanel(); }
        @Override public void onDisplayChanged(int id) { }
    };

    @Override
    protected String[] getArguments() {
        String[] a = getIntent() != null ? getIntent().getStringArrayExtra(EXTRA_ARGS) : null;
        return a != null ? a : new String[0];
    }

    private boolean panelWanted() {
        for (String s : getArguments()) if ("--second-screen".equals(s)) return true;
        return false;
    }

    @Override
    protected void onCreate(Bundle b) {
        super.onCreate(b);
        if (panelWanted()) {
            dm = (DisplayManager) getSystemService(Context.DISPLAY_SERVICE);
            if (dm != null) { dm.registerDisplayListener(listener, ui); updatePanel(); }
        }
    }

    private void updatePanel() {
        if (dm == null || isFinishing()) return;
        Display[] ds = dm.getDisplays(DisplayManager.DISPLAY_CATEGORY_PRESENTATION);
        Display d = ds.length > 0 ? ds[0] : null;
        if (panel != null && (d == null || panel.getDisplay().getDisplayId() != d.getDisplayId())) {
            panel.dismiss(); panel = null;
        }
        if (d != null && panel == null) {
            try {
                panel = new PanelPresentation(this, d);
                panel.show();
            } catch (WindowManager.InvalidDisplayException e) {
                panel = null;
            }
        }
    }

    @Override
    protected void onDestroy() {
        boolean finishing = isFinishing();
        if (dm != null) dm.unregisterDisplayListener(listener);
        if (panel != null) { panel.dismiss(); panel = null; }
        super.onDestroy();
        // the ":game" process cannot start the game twice (the low range is reserved once, in JNI_OnLoad)
        if (finishing) System.exit(0);
    }

    /** The panel on the secondary display: polls the native buffer ~30 times a second and draws it scaled to fit.
     *  Not focusable, so the game keeps the controller and the keys. */
    static class PanelPresentation extends Presentation {
        PanelPresentation(Context c, Display d) { super(c, d); }

        @Override
        protected void onCreate(Bundle b) {
            super.onCreate(b);
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL
                    | WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            setContentView(new PanelView(getContext()));
        }
    }

    static class PanelView extends View {
        private final ByteBuffer buf = ByteBuffer.allocateDirect(960 * 960 * 4);
        private final int[] dims = new int[2];
        private Bitmap bmp;
        private int seq;
        private boolean nativeReady = true, layoutSent;
        private final Paint paint = new Paint(Paint.FILTER_BITMAP_FLAG);
        private final Rect src = new Rect(), dst = new Rect();
        private final Runnable tick = new Runnable() {
            @Override public void run() { poll(); postDelayed(this, 33); }
        };

        PanelView(Context c) { super(c); setBackgroundColor(Color.rgb(0xF4, 0xEC, 0xD4)); }

        @Override protected void onAttachedToWindow() { super.onAttachedToWindow(); post(tick); }
        @Override protected void onDetachedFromWindow() { removeCallbacks(tick); super.onDetachedFromWindow(); }

        private void poll() {
            if (!nativeReady) return;
            try {
                if (!layoutSent && getWidth() > 0) { nativePanelLayout(getWidth(), getHeight()); layoutSent = true; }
                int s = nativePanelFrame(buf, dims, seq);
                if (s == 0) return;
                seq = s;
                int w = dims[0], h = dims[1];
                if (bmp == null || bmp.getWidth() != w || bmp.getHeight() != h) bmp = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
                buf.rewind();
                buf.limit(w * h * 4);
                bmp.copyPixelsFromBuffer(buf);
                buf.clear();
                invalidate();
            } catch (UnsatisfiedLinkError e) {
                // the game has not registered the natives yet (it does on its first panel frame): try again later
                layoutSent = false;
            }
        }

        @Override
        protected void onDraw(Canvas c) {
            if (bmp == null) return;
            int W = getWidth(), H = getHeight(), w = bmp.getWidth(), h = bmp.getHeight();
            float k = Math.min((float) W / w, (float) H / h);
            int dw = Math.round(w * k), dh = Math.round(h * k);
            src.set(0, 0, w, h);
            dst.set((W - dw) / 2, (H - dh) / 2, (W - dw) / 2 + dw, (H - dh) / 2 + dh);
            c.drawBitmap(bmp, src, dst, paint);
        }
    }
}

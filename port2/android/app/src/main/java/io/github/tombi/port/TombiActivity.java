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
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import java.nio.ByteBuffer;

import org.libsdl.app.SDLActivity;

/** The game: SDL2's activity, with the launcher's arguments handed to SDL_main (port2/android/loader.c).
 *  With --second-screen (launcher: "Segunda pantalla: mapa y estado") the port's map + status panel
 *  (port2/runtime/panel.c) is shown on a secondary display (dual-screen handhelds) through a Presentation;
 *  no secondary display: nothing changes; displays added / removed while playing are followed.
 *  The controller is read here with the launcher's button mapping (PadMap, EXTRA_PADMAP) and the PS1 buttons handed to
 *  the game through nativePadState (runtime/sdl.c); until the game has registered it, SDL reads the controller. */
public class TombiActivity extends SDLActivity {
    public static final String EXTRA_ARGS = "io.github.tombi.port.ARGS";
    public static final String EXTRA_PADMAP = "io.github.tombi.port.PADMAP";

    /* registered by libtombi.so at start (runtime/sdl.c): the PS1 buttons held (shims/pad.c bits) */
    static native void nativePadState(int buttons);
    /* registered with the panel (runtime/panel.c): a panel action (IN_*) */
    static native void nativePanelKey(int action);

    /* registered at run time by libtombi.so (runtime/panel.c, RegisterNatives on this class) */
    static native int nativePanelFrame(ByteBuffer dst, int[] dims, int lastSeq);
    static native void nativePanelLayout(int w, int h);
    static native void nativePanelTouch(int x, int y, int action);   // canvas pixels; 0 down, 1 up, 2 move

    private DisplayManager dm;
    private PanelPresentation panel;
    private PadMap pad;
    private int padSent = -1;
    private final java.util.HashSet<String> padSeen = new java.util.HashSet<>();
    private final Handler ui = new Handler(Looper.getMainLooper());
    private final DisplayManager.DisplayListener listener = new DisplayManager.DisplayListener() {
        @Override public void onDisplayAdded(int id) { updatePanel(); }
        @Override public void onDisplayRemoved(int id) { updatePanel(); }
        @Override public void onDisplayChanged(int id) { }
    };

    /* registered by libtombi.so (runtime/sdl.c, --dualshock): the left stick as the pad's analog bytes, 0..255 */
    static native void nativePadStick(int lx, int ly);

    @Override
    protected String[] getArguments() {
        String[] a = getIntent() != null ? getIntent().getStringArrayExtra(EXTRA_ARGS) : null;
        if (a == null) return new String[0];
        // --prompts auto: the style from the controller's name; and --prompt-map: the physical button of each PS1
        // face button with the player's mapping (shims/prompts.c)
        java.util.ArrayList<String> out = new java.util.ArrayList<>();
        boolean prompts = false;
        for (int i = 0; i < a.length; i++) {
            if ("--prompts".equals(a[i]) && i + 1 < a.length) {
                prompts = true;
                out.add(a[i]); out.add("auto".equals(a[i + 1]) ? autoPromptStyle() : a[i + 1]); i++;
            } else out.add(a[i]);
        }
        if (prompts) {
            PadMap pm = pad != null ? pad : new PadMap(getIntent().getStringExtra(EXTRA_PADMAP));
            out.add("--prompt-map"); out.add(promptMap(pm));
        }
        return out.toArray(new String[0]);
    }
    private boolean dualshock() {
        String[] a = getIntent() != null ? getIntent().getStringArrayExtra(EXTRA_ARGS) : null;
        if (a != null) for (String s : a) if ("--dualshock".equals(s)) return true;
        return false;
    }
    /* the style for the controller in use: its name (Xbox / PlayStation / Nintendo pads; the AYN Thor's "Odin
       Controller" and the Retroid handhelds label their buttons A B X Y as Android's key codes: Nintendo look) */
    static String autoPromptStyle() {
        try {
            for (int id : InputDevice.getDeviceIds()) {
                InputDevice d = InputDevice.getDevice(id);
                if (d == null || d.isVirtual() || (d.getSources() & InputDevice.SOURCE_GAMEPAD) != InputDevice.SOURCE_GAMEPAD) continue;
                String n = d.getName() == null ? "" : d.getName().toLowerCase(java.util.Locale.ROOT);
                if (n.contains("xbox") || n.contains("x-box")) return "xbox";
                if (n.contains("dualsense") || n.contains("dualshock") || n.contains("playstation") || n.contains("wireless controller")
                        || n.contains("ps4") || n.contains("ps5") || n.contains("ps3")) return "playstation";
                if (n.contains("nintendo") || n.contains("pro controller") || n.contains("joy-con") || n.contains("odin")
                        || n.contains("ayn") || n.contains("thor") || n.contains("retroid")) return "nintendo";
                return "generic";
            }
        } catch (Throwable ignored) { }
        return "playstation";   // no controller (touch): the game's own glyphs
    }
    static String promptMap(PadMap pm) {
        String[] ps = {"cross", "circle", "square", "triangle"};
        StringBuilder b = new StringBuilder();
        for (String p : ps) {
            String src = "none";
            for (String s : pm.src[PadMap.index(p)]) { String v = promptSrc(s); if (v != null) { src = v; break; } }
            if (b.length() > 0) b.append(',');
            b.append(p).append('=').append(src);
        }
        return b.toString();
    }
    static String promptSrc(String s) {
        if (s.equals("k" + KeyEvent.KEYCODE_BUTTON_A)) return "a";
        if (s.equals("k" + KeyEvent.KEYCODE_BUTTON_B)) return "b";
        if (s.equals("k" + KeyEvent.KEYCODE_BUTTON_X)) return "x";
        if (s.equals("k" + KeyEvent.KEYCODE_BUTTON_Y)) return "y";
        if (s.equals("k" + KeyEvent.KEYCODE_BUTTON_L1)) return "l1";
        if (s.equals("k" + KeyEvent.KEYCODE_BUTTON_R1)) return "r1";
        if (s.equals("k" + KeyEvent.KEYCODE_BUTTON_L2) || s.startsWith("a" + MotionEvent.AXIS_LTRIGGER)) return "l2";
        if (s.equals("k" + KeyEvent.KEYCODE_BUTTON_R2) || s.startsWith("a" + MotionEvent.AXIS_RTRIGGER)) return "r2";
        return null;
    }

    // ---- vibration (--dualshock: the game's PadSetAct, runtime/sdl.c calls rumble) ----
    private static TombiActivity self;
    private static InputDevice lastPad;
    /* small motor on / off, big motor 0..255; 0, 0 stops.  The controller's vibrator, else the device's own */
    static void rumble(int small, int big) {
        try {
            TombiActivity a = self;
            if (a == null) return;
            android.os.Vibrator v = null;
            InputDevice d = lastPad;
            if (d != null) { android.os.Vibrator dv = d.getVibrator(); if (dv != null && dv.hasVibrator()) v = dv; }
            if (v == null) v = (android.os.Vibrator) a.getSystemService(Context.VIBRATOR_SERVICE);
            if (v == null || !v.hasVibrator()) return;
            int amp = Math.max(big, small != 0 ? 160 : 0);
            if (amp == 0) { v.cancel(); return; }
            if (android.os.Build.VERSION.SDK_INT >= 26)
                v.vibrate(android.os.VibrationEffect.createOneShot(2000, Math.max(1, Math.min(255, amp))));
            else v.vibrate(2000);
        } catch (Throwable ignored) { }
    }

    private boolean panelWanted() {
        for (String s : getArguments()) if ("--second-screen".equals(s)) return true;
        return false;
    }

    @Override
    protected void onCreate(Bundle b) {
        final Context app = getApplicationContext();
        try { onCreateLogged(app, b); }
        catch (Throwable t) {   // anything before / in SDL's start-up: into tombi.log and the marker, then the usual crash
            CrashLog.append(app, "TombiActivity.onCreate FAILED: " + CrashLog.stack(t));
            CrashLog.writeMarker(app, CrashLog.CRASHED, "TombiActivity.onCreate: " + t + "\n");
            if (t instanceof RuntimeException) throw (RuntimeException) t;
            if (t instanceof Error) throw (Error) t;
            throw new RuntimeException(t);
        }
    }

    private void onCreateLogged(final Context app, Bundle b) {
        // the log first (CrashLog): a fresh tombi.log, its dir handed to libmain.so (loaded by super.onCreate, whose
        // JNI_OnLoad opens it), Java exceptions and a blocked UI thread written into it
        CrashLog.rotate(app);
        CrashLog.append(app, "TombiActivity.onCreate, pid " + android.os.Process.myPid() + (b != null ? " (restored state)" : ""));
        CrashLog.append(app, CrashLog.deviceInfo(app).replace("\n", "; "));
        StringBuilder a = new StringBuilder("arguments:");
        for (String s : getArguments()) a.append(' ').append(s);
        CrashLog.append(app, a.toString());
        try { android.system.Os.setenv("TOMBI_LOG_DIR", CrashLog.dir(app).getAbsolutePath(), true); }
        catch (Throwable t) { CrashLog.append(app, "setenv TOMBI_LOG_DIR: " + t); }
        final Thread.UncaughtExceptionHandler prev = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override public void uncaughtException(Thread t, Throwable e) {
                CrashLog.append(app, "UNCAUGHT EXCEPTION in thread \"" + t.getName() + "\": " + CrashLog.stack(e));
                CrashLog.writeMarker(app, CrashLog.CRASHED, "java exception: " + e + "\n");
                if (prev != null) prev.uncaughtException(t, e); else System.exit(2);
            }
        });
        startUiWatchdog(app);
        self = this;
        try {
            pad = new PadMap(getIntent() != null ? getIntent().getStringExtra(EXTRA_PADMAP) : null);
            if (dualshock()) pad.dropStickDpad();   // the stick goes to the game as analog (nativePadStick), not as d-pad
            CrashLog.append(app, "button mapping: " + (pad.save().isEmpty() ? "defaults" : pad.save()));
        } catch (Throwable t) { pad = null; CrashLog.append(app, "button mapping failed, SDL reads the controller: " + CrashLog.stack(t)); }
        logControllers(app);
        super.onCreate(b);
        CrashLog.append(app, "SDLActivity.onCreate done");
        if (panelWanted()) {
            try {
                dm = (DisplayManager) getSystemService(Context.DISPLAY_SERVICE);
                if (dm != null) { dm.registerDisplayListener(listener, ui); updatePanel(); }
            } catch (Throwable t) {
                CrashLog.append(app, "second screen: setup failed, playing without it: " + CrashLog.stack(t));
                dm = null;
            }
        }
    }

    // ---- the controller (PadMap) ----
    /* every controller with the keys / axes the mapping cares about, into the log (for "button X does nothing") */
    private void logControllers(Context app) {
        try {
            for (int id : InputDevice.getDeviceIds()) {
                InputDevice d = InputDevice.getDevice(id);
                if (d == null || (d.getSources() & (InputDevice.SOURCE_GAMEPAD | InputDevice.SOURCE_JOYSTICK)) == 0) continue;
                int[] k = {KeyEvent.KEYCODE_BUTTON_L1, KeyEvent.KEYCODE_BUTTON_R1, KeyEvent.KEYCODE_BUTTON_L2, KeyEvent.KEYCODE_BUTTON_R2,
                        KeyEvent.KEYCODE_BUTTON_THUMBL, KeyEvent.KEYCODE_BUTTON_THUMBR};
                boolean[] has = d.hasKeys(k);
                StringBuilder sb = new StringBuilder("controller \"" + d.getName() + "\" (vendor " + d.getVendorId() + ", product " + d.getProductId()
                        + ", sources 0x" + Integer.toHexString(d.getSources()) + "): hasKeys L1 " + has[0] + " R1 " + has[1] + " L2 " + has[2] + " R2 " + has[3]
                        + " L3 " + has[4] + " R3 " + has[5] + "; axes");
                for (InputDevice.MotionRange r : d.getMotionRanges()) sb.append(' ').append(MotionEvent.axisToString(r.getAxis()).replace("AXIS_", ""));
                CrashLog.append(app, sb.toString());
            }
        } catch (Throwable t) { CrashLog.append(app, "controllers: " + t); }
    }
    private void padSeen(String s) {   // each key / axis once in the log
        if (padSeen.size() < 64 && padSeen.add(s)) CrashLog.append(getApplicationContext(), "controller input: " + s + " (" + PadMap.srcName(s, false) + ")");
    }
    /* the mapping's state to the game; false: the natives are not registered yet (the game registers them as it
       starts), SDL keeps the controller meanwhile */
    private boolean padSend() {
        int t = pad.targets(), bits = PadMap.ps1Bits(t);
        try {
            if (bits != padSent) { nativePadState(bits); padSent = bits; }
        } catch (UnsatisfiedLinkError e) {
            return false;
        }
        int p = pad.pressed(t);
        for (int i = PadMap.PS1; i < PadMap.NAMES.length; i++)
            if ((p >> i & 1) != 0) try { nativePanelKey(PadMap.PANEL_ACTION[i - PadMap.PS1]); } catch (UnsatisfiedLinkError ignored) { }
        return true;
    }
    @Override
    public boolean dispatchKeyEvent(KeyEvent e) {
        int code = e.getKeyCode();
        if (pad != null && !PadMap.systemKey(code) && PadMap.fromController(e.getDevice(), e.getSource())
                && (e.getAction() == KeyEvent.ACTION_DOWN || e.getAction() == KeyEvent.ACTION_UP)) {
            if (e.getAction() == KeyEvent.ACTION_DOWN && e.getRepeatCount() == 0) padSeen("k" + code);
            pad.key(code, e.getAction() == KeyEvent.ACTION_DOWN);
            if (padSend()) return true;
        }
        return super.dispatchKeyEvent(e);
    }
    @Override
    public boolean dispatchGenericMotionEvent(MotionEvent e) {
        if (pad != null && e.getActionMasked() == MotionEvent.ACTION_MOVE && (e.getSource() & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK) {
            pad.motion(e);
            lastPad = e.getDevice();
            if (dualshock()) {
                try { nativePadStick(stickByte(e.getAxisValue(MotionEvent.AXIS_X)), stickByte(e.getAxisValue(MotionEvent.AXIS_Y))); }
                catch (UnsatisfiedLinkError ignored) { }
            }
            for (int a : new int[]{MotionEvent.AXIS_LTRIGGER, MotionEvent.AXIS_RTRIGGER, MotionEvent.AXIS_BRAKE, MotionEvent.AXIS_GAS,
                    MotionEvent.AXIS_HAT_X, MotionEvent.AXIS_HAT_Y, MotionEvent.AXIS_Z, MotionEvent.AXIS_RZ}) {
                float v = e.getAxisValue(a);
                if (v > 0.5f) padSeen("a" + a + "+"); else if (v < -0.5f) padSeen("a" + a + "-");
            }
            if (padSend()) return true;
        }
        return super.dispatchGenericMotionEvent(e);
    }

    /* a 15 % dead zone, full deflection from 90 % (as runtime/sdl.c on the PC) */
    static int stickByte(float f) {
        float a = Math.abs(f);
        if (a < 0.15f) return 0x80;
        a = Math.min(1f, (a - 0.15f) / 0.75f);
        int r = (int) (a * 127.5f + 0.5f);
        return f < 0 ? Math.max(0, 0x80 - r) : Math.min(0xFF, 0x7F + r);
    }

    /* "se cuelga y se cierra": the UI thread blocked (an ANR) leaves its stack in the log every 5 s (at most 4 times) */
    private volatile long uiTick;
    private void startUiWatchdog(final Context app) {
        uiTick = android.os.SystemClock.uptimeMillis();
        final Runnable tick = new Runnable() { @Override public void run() { uiTick = android.os.SystemClock.uptimeMillis(); } };
        Thread w = new Thread(new Runnable() {
            @Override public void run() {
                int reports = 0;
                while (reports < 4) {
                    try { Thread.sleep(1000); } catch (InterruptedException e) { return; }
                    ui.post(tick);
                    long late = android.os.SystemClock.uptimeMillis() - uiTick;
                    if (late < 5000 * (reports + 1)) continue;
                    reports++;
                    StringBuilder b = new StringBuilder("UI thread blocked for " + late / 1000 + " s (ANR?); its stack:");
                    for (StackTraceElement e : Looper.getMainLooper().getThread().getStackTrace()) b.append("\n    at ").append(e);
                    CrashLog.append(app, b.toString());
                }
            }
        }, "tombi-ui-watchdog");
        w.setDaemon(true);
        w.start();
    }

    private void updatePanel() {
        if (dm == null || isFinishing()) return;
        try {
            Display[] ds = dm.getDisplays(DisplayManager.DISPLAY_CATEGORY_PRESENTATION);
            Display d = ds != null && ds.length > 0 ? ds[0] : null;
            if (panel != null && (d == null || panel.getDisplay().getDisplayId() != d.getDisplayId())) {
                try { panel.dismiss(); } catch (Throwable ignored) { }
                panel = null;
            }
            if (d != null && panel == null) {
                CrashLog.append(getApplicationContext(), "second screen: Presentation on display #" + d.getDisplayId() + " \"" + d.getName() + "\"");
                panel = new PanelPresentation(this, d);
                panel.show();
            }
        } catch (Throwable t) {   // InvalidDisplayException, BadTokenException, ...: play without the panel
            CrashLog.append(getApplicationContext(), "second screen: Presentation failed, playing without it: " + CrashLog.stack(t));
            panel = null;
        }
    }

    @Override
    protected void onDestroy() {
        boolean finishing = isFinishing();
        CrashLog.append(getApplicationContext(), "TombiActivity.onDestroy (finishing " + finishing + ")");
        try { if (dm != null) dm.unregisterDisplayListener(listener); } catch (Throwable ignored) { }
        try { if (panel != null) panel.dismiss(); } catch (Throwable ignored) { }
        panel = null;
        if (finishing) CrashLog.file(getApplicationContext(), CrashLog.RUNNING).delete();   // a clean end ("crashed" stays if loader.c wrote it)
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
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL   // touchable, never takes the controller
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
        // nearest neighbour: the panel is pixel art.  new Paint() has FILTER_BITMAP_FLAG set on Android 9+ (bilinear: the
        // integer-scaled canvas came out blurred / "pixelated" on the AYN Thor), so it is cleared explicitly
        private final Paint paint = new Paint();
        { paint.setFilterBitmap(false); paint.setAntiAlias(false); paint.setDither(false); }
        private final Rect src = new Rect(), dst = new Rect();
        private final Runnable tick = new Runnable() {
            @Override public void run() { poll(); postDelayed(this, 33); }
        };

        PanelView(Context c) { super(c); setBackgroundColor(Color.rgb(0x60, 0x50, 0x48)); }

        // the canvas follows the view's size (the display's, 1240 x 1080 on the Thor: canvas 413 x 360, k = 3)
        @Override protected void onSizeChanged(int w, int h, int ow, int oh) { super.onSizeChanged(w, h, ow, oh); layoutSent = false; }

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

        private int scale = 1;

        @Override
        public boolean onTouchEvent(android.view.MotionEvent e) {
            int a = e.getActionMasked();
            int act = a == android.view.MotionEvent.ACTION_DOWN ? 0 : a == android.view.MotionEvent.ACTION_UP ? 1
                    : a == android.view.MotionEvent.ACTION_MOVE ? 2 : -1;
            if (act < 0 || bmp == null) return true;
            try {
                nativePanelTouch((int) ((e.getX() - dst.left) / scale), (int) ((e.getY() - dst.top) / scale), act);
            } catch (UnsatisfiedLinkError ignored) {
            }
            return true;
        }

        @Override
        protected void onDraw(Canvas c) {
            if (bmp == null) return;
            int W = getWidth(), H = getHeight(), w = bmp.getWidth(), h = bmp.getHeight();
            int k = Math.max(1, Math.min(W / w, H / h));   // integer scale (the canvas is the display / k)
            int dw = w * k, dh = h * k;
            scale = k;
            src.set(0, 0, w, h);
            dst.set((W - dw) / 2, (H - dh) / 2, (W - dw) / 2 + dw, (H - dh) / 2 + dh);
            c.drawBitmap(bmp, src, dst, paint);
        }
    }
}

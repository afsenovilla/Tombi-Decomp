package io.github.tombi.port;

import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.util.ArrayList;
import java.util.List;

/**
 * The controller mapping (launcher: Options > Controls / button mapping; TombiActivity applies it): each PS1 button
 * and each panel action has a list of sources, an Android key code ("k103") or an axis past half way ("a18+",
 * "a16-").  The defaults are what SDL's controller mapping gives on the PC plus the triggers, hats and the left stick;
 * the saved string ("r1=k103;a18+,l2=k104") holds only the targets the player changed, and replaces their defaults.
 * TombiActivity reads the controller with it and hands the PS1 buttons to the game (nativePadState, runtime/sdl.c),
 * so a button works whatever SDL's own Android mapping makes of the device (it left R1, L2 and R2 out on some).
 */
final class PadMap {
    /** PS1 buttons in runtime/sdl.c's order (button_names), then the panel's actions (runtime/panel_events.inc IN_*) */
    static final String[] NAMES = {"select", "start", "up", "right", "down", "left", "l2", "r2", "l1", "r1",
            "triangle", "circle", "cross", "square", "p_tabl", "p_tabr", "p_zoomout", "p_zoomin", "p_hide"};
    static final int PS1 = 14;
    /** the pad's bits (shims/pad.c, P_* of runtime/sdl.c) of the 14 PS1 buttons */
    static final int[] BITS = {0x1, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000};
    /** panel actions (IN_TAB_L, IN_TAB_R, IN_ZOOM_OUT, IN_ZOOM_IN, IN_HIDE) */
    static final int[] PANEL_ACTION = {7, 8, 9, 10, 11};

    static String[] defaults(int t) {
        switch (NAMES[t]) {
            case "select": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_SELECT, "k" + KeyEvent.KEYCODE_BACK};
            case "start": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_START, "k" + KeyEvent.KEYCODE_MENU};
            case "up": return new String[]{"k" + KeyEvent.KEYCODE_DPAD_UP, "a" + MotionEvent.AXIS_HAT_Y + "-", "a" + MotionEvent.AXIS_Y + "-"};
            case "down": return new String[]{"k" + KeyEvent.KEYCODE_DPAD_DOWN, "a" + MotionEvent.AXIS_HAT_Y + "+", "a" + MotionEvent.AXIS_Y + "+"};
            case "left": return new String[]{"k" + KeyEvent.KEYCODE_DPAD_LEFT, "a" + MotionEvent.AXIS_HAT_X + "-", "a" + MotionEvent.AXIS_X + "-"};
            case "right": return new String[]{"k" + KeyEvent.KEYCODE_DPAD_RIGHT, "a" + MotionEvent.AXIS_HAT_X + "+", "a" + MotionEvent.AXIS_X + "+"};
            case "l2": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_L2, "a" + MotionEvent.AXIS_LTRIGGER + "+", "a" + MotionEvent.AXIS_BRAKE + "+"};
            case "r2": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_R2, "a" + MotionEvent.AXIS_RTRIGGER + "+", "a" + MotionEvent.AXIS_GAS + "+"};
            case "l1": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_L1};
            case "r1": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_R1};
            case "triangle": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_Y};
            case "circle": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_B};
            case "cross": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_A, "k" + KeyEvent.KEYCODE_DPAD_CENTER};
            case "square": return new String[]{"k" + KeyEvent.KEYCODE_BUTTON_X};
            default: return new String[0];   // panel actions: unbound until the player picks a button
        }
    }

    final String[][] src = new String[NAMES.length][];

    PadMap(String saved) {
        for (int t = 0; t < NAMES.length; t++) src[t] = defaults(t);
        if (saved == null) return;
        for (String e : saved.split(",")) {
            int eq = e.indexOf('=');
            if (eq <= 0) continue;
            int t = index(e.substring(0, eq));
            if (t < 0) continue;
            String v = e.substring(eq + 1).trim();
            src[t] = v.isEmpty() ? new String[0] : v.split(";");
        }
    }

    static int index(String name) { for (int t = 0; t < NAMES.length; t++) if (NAMES[t].equals(name)) return t; return -1; }

    /** the saved form: only the targets that differ from their defaults */
    String save() {
        StringBuilder b = new StringBuilder();
        for (int t = 0; t < NAMES.length; t++) {
            if (java.util.Arrays.equals(src[t], defaults(t))) continue;
            if (b.length() > 0) b.append(',');
            b.append(NAMES[t]).append('=').append(join(src[t]));
        }
        return b.toString();
    }
    static String join(String[] a) { StringBuilder b = new StringBuilder(); for (String s : a) { if (b.length() > 0) b.append(';'); b.append(s); } return b.toString(); }

    /** a controller's event (the launcher's capture and TombiActivity both test this) */
    static boolean fromController(InputDevice d, int source) {
        int pad = InputDevice.SOURCE_GAMEPAD | InputDevice.SOURCE_JOYSTICK;
        return (source & pad) == pad || (source & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD
                || (source & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK
                || (d != null && (d.getSources() & pad) != 0 && !d.isVirtual());
    }
    /** keys that stay the system's (volume, power, home) */
    static boolean systemKey(int code) {
        return code == KeyEvent.KEYCODE_VOLUME_UP || code == KeyEvent.KEYCODE_VOLUME_DOWN || code == KeyEvent.KEYCODE_VOLUME_MUTE
                || code == KeyEvent.KEYCODE_POWER || code == KeyEvent.KEYCODE_HOME || code == KeyEvent.KEYCODE_APP_SWITCH;
    }

    // ---- the live state (TombiActivity) ----
    private final boolean[] keyDown = new boolean[512];
    private final float[] axis = new float[64];
    private int lastTargets;   // bit t: target t held (PS1 bits are built from it)

    boolean key(int code, boolean down) {
        if (code < 0 || code >= keyDown.length) return false;
        keyDown[code] = down;
        return true;
    }
    void motion(MotionEvent e) {
        for (int a = 0; a < axis.length; a++) axis[a] = e.getAxisValue(a);
    }
    private boolean held(String s) {
        try {
            if (s.startsWith("k")) { int c = Integer.parseInt(s.substring(1)); return c >= 0 && c < keyDown.length && keyDown[c]; }
            if (s.startsWith("a")) {
                boolean neg = s.endsWith("-");
                int a = Integer.parseInt(s.substring(1, s.length() - (s.endsWith("+") || neg ? 1 : 0)));
                return a >= 0 && a < axis.length && (neg ? axis[a] < -0.5f : axis[a] > 0.5f);
            }
        } catch (NumberFormatException ignored) { }
        return false;
    }
    /** bit t set: target t held now */
    int targets() {
        int m = 0;
        for (int t = 0; t < NAMES.length; t++) for (String s : src[t]) if (held(s)) { m |= 1 << t; break; }
        return m;
    }
    static int ps1Bits(int targets) { int b = 0; for (int t = 0; t < PS1; t++) if ((targets >> t & 1) != 0) b |= BITS[t]; return b; }
    /** targets newly held since the last call (for the panel's actions: one step per press) */
    int pressed(int now) { int p = now & ~lastTargets; lastTargets = now; return p; }

    // ---- names for the launcher ----
    static String srcName(String s, boolean es) {
        try {
            if (s.startsWith("k")) {
                String n = KeyEvent.keyCodeToString(Integer.parseInt(s.substring(1)));
                n = n.replace("KEYCODE_", "").replace("BUTTON_", "").replace("DPAD_", es ? "CRUCETA " : "DPAD ").replace('_', ' ');
                return n;
            }
            if (s.startsWith("a")) {
                boolean neg = s.endsWith("-");
                int a = Integer.parseInt(s.substring(1, s.length() - (s.endsWith("+") || neg ? 1 : 0)));
                return (es ? "Eje " : "Axis ") + MotionEvent.axisToString(a).replace("AXIS_", "") + (neg ? " -" : " +");
            }
        } catch (Exception ignored) { }
        return s;
    }
    String describe(int t, boolean es) {
        if (src[t].length == 0) return es ? "(sin asignar)" : "(none)";
        List<String> out = new ArrayList<>();
        for (String s : src[t]) { String n = srcName(s, es); if (!out.contains(n)) out.add(n); }
        StringBuilder b = new StringBuilder();
        for (int i = 0; i < out.size() && i < 2; i++) { if (i > 0) b.append(" / "); b.append(out.get(i)); }
        if (out.size() > 2) b.append(" …");
        return b.toString();
    }
}

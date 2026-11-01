package io.github.tombi.port;

import android.app.ActivityManager;
import android.content.Context;
import android.hardware.display.DisplayManager;
import android.os.Build;
import android.view.Display;

import java.io.File;
import java.io.FileOutputStream;
import java.io.RandomAccessFile;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.charset.Charset;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

/** The game's log and crash markers, shared by the launcher and the ":game" process (TombiActivity).
 *  Everything lives in the app's INTERNAL files dir (getFilesDir(): the same path in both processes, always writable,
 *  no permission): tombi.log (this run: TombiActivity's [java] lines, port2/android/loader.c's [loader] lines, the
 *  port's own stderr), tombi.prev.log (the run before), "running" (written by the launcher before the start, removed
 *  by TombiActivity on a clean finish) and "crashed" (loader.c: fatal signal, abnormal exit, start-up failure).
 *  The user reaches the log through the launcher (crash card, Options > View last log: copy / share / export). */
final class CrashLog {
    static final String LOG = "tombi.log", PREV = "tombi.prev.log", RUNNING = "running", CRASHED = "crashed";
    private CrashLog() { }

    static File dir(Context c) { return c.getFilesDir(); }
    static File file(Context c, String name) { return new File(dir(c), name); }

    static synchronized void append(Context c, String line) {
        try {
            FileOutputStream o = new FileOutputStream(file(c, LOG), true);
            try {
                o.write(("[java " + new SimpleDateFormat("HH:mm:ss.SSS", Locale.ROOT).format(new Date()) + "] " + line + "\n").getBytes("UTF-8"));
                o.getFD().sync();
            } finally { o.close(); }
        } catch (Throwable ignored) { }
    }

    static void writeMarker(Context c, String name, String text) {
        try { FileOutputStream o = new FileOutputStream(file(c, name)); try { o.write(text.getBytes("UTF-8")); } finally { o.close(); } }
        catch (Throwable ignored) { }
    }

    /** start of a game run (TombiActivity, before SDL loads libmain.so): tombi.log -> tombi.prev.log, a fresh log */
    static void rotate(Context c) {
        File log = file(c, LOG), prev = file(c, PREV);
        if (log.exists()) { prev.delete(); log.renameTo(prev); }
        try { new FileOutputStream(log).close(); } catch (Throwable ignored) { }
    }

    static String stack(Throwable t) { StringWriter w = new StringWriter(); t.printStackTrace(new PrintWriter(w)); return w.toString(); }

    static String deviceInfo(Context c) {
        StringBuilder b = new StringBuilder();
        b.append("device: ").append(Build.MANUFACTURER).append(' ').append(Build.MODEL).append(" (").append(Build.DEVICE)
         .append("), Android ").append(Build.VERSION.RELEASE).append(" (SDK ").append(Build.VERSION.SDK_INT).append(")");
        if (Build.VERSION.SDK_INT >= 31) b.append(", soc ").append(Build.SOC_MANUFACTURER).append(' ').append(Build.SOC_MODEL);
        b.append("\nABIs: ");
        for (String a : Build.SUPPORTED_ABIS) b.append(a).append(' ');
        b.append("(32-bit: ");
        for (String a : Build.SUPPORTED_32_BIT_ABIS) b.append(a).append(' ');
        b.append(")");
        try { b.append("\napp: ").append(c.getPackageManager().getPackageInfo(c.getPackageName(), 0).versionName); }
        catch (Throwable ignored) { }
        b.append("\n").append(displays(c));
        return b.toString();
    }

    static String displays(Context c) {
        StringBuilder b = new StringBuilder("displays:");
        try {
            DisplayManager dm = (DisplayManager) c.getSystemService(Context.DISPLAY_SERVICE);
            Display[] all = dm != null ? dm.getDisplays() : new Display[0];
            Display[] pres = dm != null ? dm.getDisplays(DisplayManager.DISPLAY_CATEGORY_PRESENTATION) : new Display[0];
            for (Display d : all) {
                android.graphics.Point p = new android.graphics.Point(); d.getRealSize(p);
                boolean isPres = false; for (Display q : pres) if (q.getDisplayId() == d.getDisplayId()) isPres = true;
                b.append(" [#").append(d.getDisplayId()).append(" \"").append(d.getName()).append("\" ").append(p.x).append('x').append(p.y)
                 .append(' ').append(Math.round(d.getRefreshRate())).append("Hz flags=").append(d.getFlags()).append(isPres ? " presentation" : "").append(']');
            }
        } catch (Throwable t) { b.append(" (error: ").append(t).append(')'); }
        return b.toString();
    }

    /** the ":game" process is alive (the user may come back to the launcher while it runs) */
    static boolean gameAlive(Context c) {
        try {
            ActivityManager am = (ActivityManager) c.getSystemService(Context.ACTIVITY_SERVICE);
            List<ActivityManager.RunningAppProcessInfo> ps = am != null ? am.getRunningAppProcesses() : null;
            if (ps != null) for (ActivityManager.RunningAppProcessInfo p : ps) if (p.processName != null && p.processName.endsWith(":game")) return true;
        } catch (Throwable ignored) { }
        return false;
    }

    /** the last run ended abnormally: "crashed" left by loader.c, or "running" left over (killed: ANR, OOM, native crash
     *  before the handler) while the game process is gone */
    static boolean crashedLastRun(Context c) {
        return file(c, CRASHED).exists() || (file(c, RUNNING).exists() && !gameAlive(c));
    }
    /** markers written before the app was last installed / updated: an update kills a running game (the "running"
     *  marker stays), so they say nothing about this build; the launcher clears them instead of showing the crash card */
    static boolean markersStale(Context c) {
        try {
            long upd = c.getPackageManager().getPackageInfo(c.getPackageName(), 0).lastUpdateTime;
            File cr = file(c, CRASHED), ru = file(c, RUNNING);
            if (!cr.exists() && !ru.exists()) return false;
            return (!cr.exists() || cr.lastModified() < upd) && (!ru.exists() || ru.lastModified() < upd);
        } catch (Throwable t) { return false; }
    }
    static void clearMarkers(Context c) { file(c, CRASHED).delete(); file(c, RUNNING).delete(); }

    static String readMarker(Context c, String name) {
        File f = file(c, name);
        if (!f.exists()) return null;
        byte[] b = readTailBytes(f, 2048);
        return new String(b, Charset.forName("UTF-8")).trim();
    }

    static byte[] readTailBytes(File f, int max) {
        try {
            RandomAccessFile r = new RandomAccessFile(f, "r");
            try {
                long len = r.length(), from = Math.max(0, len - max);
                byte[] b = new byte[(int) (len - from)];
                r.seek(from); r.readFully(b);
                return b;
            } finally { r.close(); }
        } catch (Throwable t) { return new byte[0]; }
    }

    /** the last n lines of tombi.log (empty list: no log) */
    static List<String> tail(Context c, int n) {
        String s = new String(readTailBytes(file(c, LOG), 64 * 1024), Charset.forName("UTF-8"));
        String[] all = s.split("\n");
        List<String> out = new ArrayList<>();
        for (int i = Math.max(0, all.length - n); i < all.length; i++) if (!(all[i].isEmpty() && i == all.length - 1)) out.add(all[i]);
        return out;
    }

    /** the text to copy / share / export: device, markers, the log (its last maxBytes) and the previous run's tail */
    static String report(Context c, int maxBytes) {
        StringBuilder b = new StringBuilder("Tombi! Android - informe / report\n");
        b.append(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.ROOT).format(new Date())).append('\n');
        b.append(deviceInfo(c)).append('\n');
        String cr = readMarker(c, CRASHED), ru = readMarker(c, RUNNING);
        if (cr != null) b.append("crashed marker: ").append(cr).append('\n');
        if (ru != null) b.append("running marker left over: ").append(ru).append('\n');
        try {
            long upd = c.getPackageManager().getPackageInfo(c.getPackageName(), 0).lastUpdateTime;
            SimpleDateFormat f = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.ROOT);
            b.append("app installed/updated: ").append(f.format(new Date(upd)));
            if (file(c, RUNNING).exists()) b.append(", running marker: ").append(f.format(new Date(file(c, RUNNING).lastModified())));
            if (file(c, CRASHED).exists()) b.append(", crashed marker: ").append(f.format(new Date(file(c, CRASHED).lastModified())));
            b.append(", tombi.log: ").append(f.format(new Date(file(c, LOG).lastModified()))).append('\n');
        } catch (Throwable ignored) { }
        File log = file(c, LOG);
        b.append("\n===== tombi.log (").append(log.length()).append(" bytes) =====\n");
        if (log.length() > maxBytes) b.append("[... first ").append(log.length() - maxBytes).append(" bytes cut ...]\n");
        b.append(new String(readTailBytes(log, maxBytes), Charset.forName("UTF-8")));
        File prev = file(c, PREV);
        if (prev.exists()) {
            b.append("\n===== tombi.prev.log (last 4 KB of ").append(prev.length()).append(" bytes) =====\n");
            b.append(new String(readTailBytes(prev, 4096), Charset.forName("UTF-8")));
        }
        return b.toString();
    }
}

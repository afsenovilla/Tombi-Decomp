package io.github.tombi.port;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/**
 * The launcher's game library (docs/PORT_VERSIONS_PLAN.md stage U2): the discs the player added, one per serial, each
 * extracted into its own folder files/games/&lt;id&gt;/data (the port's --data mode) and run by the game library of its
 * version (loader.c --game-lib: libtombi.so for PAL, libtombi_usa.so for the USA release).  The version is the boot
 * executable's SHA-1 (disccheck.c nativeIdentify: SYSTEM.CNF BOOT, or EXE/MAIN0.EXE on the PAL discs), the same table
 * as port2/shims/psyq.c and tools/extract_data.py.  A recognised release without a port yet (Japan) is listed but
 * cannot be played.  The card's logo is cut at run time from the player's own extracted disc (the title screen's logo
 * page and its palette: Version.logoPage / logoClut, coordinates only); without it the card shows the name as text.
 *
 * Kept in SharedPreferences "launcher": games = "id,id,...", game = the selected id, g.&lt;id&gt;.(ver|serial|name|info|disc).
 * Installs from before the library (files/data, or an image copied by an even older version: prefs "disc") become the
 * PAL entry on first start (migrate()).
 */
final class Library {
    /** a release the launcher knows: its boot executable's SHA-1, names, game library (null: no port yet) */
    static final class Version {
        final String key, sha1, title, logoText, lib, saveSub, logoPage, logoClut;
        final int region;          // 0 Europe, 1 USA, 2 Japan (R.array.lib_regions)
        final int logoOff;         // cutLogo: page row of the "!" piece = logo row + logoOff
        Version(String key, String sha1, String title, String logoText, int region, String lib, String saveSub, String logoPage, String logoClut, int logoOff) {
            this.key = key; this.sha1 = sha1; this.title = title; this.logoText = logoText; this.region = region; this.lib = lib;
            this.saveSub = saveSub; this.logoPage = logoPage; this.logoClut = logoClut; this.logoOff = logoOff;
        }
        boolean pal() { return "pal".equals(key); }
        boolean supported() { return lib != null; }
    }
    static final Version[] VERSIONS = {
        // SCES-01331 / SCES-01330 Rev 1: EXE/MAIN0.EXE (the boot exe only loads it)
        new Version("pal", "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc", "Tombi!", "TOMBI!", 0, "libtombi.so", "saves",
                    "SYSTEM/A00202.000", "SYSTEM/A00203.000", 112),
        new Version("usa", "81cbc79f0230aeb4252e058039f47ac95a777f5a", "Tomba!", "TOMBA!", 1, "libtombi_usa.so", "saves_usa",
                    "SYSTEM/A00012.000", "SYSTEM/A00013.000", 88),
        new Version("jp", "07d577698ad31ec1a69b9d7b80a7c3f90b650888", "Ore no Tomba", "ORE NO TOMBA", 2, null, null, null, null, 0),
    };
    static final String PAL_REV0_SHA1 = "118459cef9b3747f57b13a498c475ffb8ab512ec";   // July 1998 Rev 0: other overlays

    /** the disc's languages as flags (LauncherView.drawFlag: 0 UK, 1 FR, 2 DE, 3 IT, 4 NL, 5 ES, 6 PT, 7 SV, 8 DA, 9 US, 10 JP;
     *  tools/game_library.py DISC_FLAGS) */
    static int[] flagsOf(String serial) {
        if ("SCES-01331".equals(serial)) return new int[]{0, 5, 6, 7, 8};
        if ("SCES-01330".equals(serial)) return new int[]{0, 1, 2, 3, 4};
        if ("SCUS-94236".equals(serial)) return new int[]{9};
        if ("SLPS-01144".equals(serial)) return new int[]{10};
        return new int[0];
    }
    static Version bySha1(String s) { for (Version v : VERSIONS) if (v.sha1.equalsIgnoreCase(s)) return v; return null; }
    static Version byKey(String k) { for (Version v : VERSIONS) if (v.key.equals(k)) return v; return null; }

    /** one disc of the library */
    static final class Game {
        String id, serial, name, info, disc; Version ver; File dir;
        File data() { return new File(dir, "data"); }
        boolean extracted() { return new File(data(), "manifest.txt").isFile(); }
        File legacyDisc() { return disc != null && new File(disc).isFile() ? new File(disc) : null; }
        /** data (or an old image) present and a port of its version */
        boolean ready() { return ver != null && ver.supported() && (extracted() || legacyDisc() != null); }
        File logoFile() { return new File(dir, "logo.png"); }
    }

    final Context ctx;
    final SharedPreferences prefs;
    Library(Context ctx, SharedPreferences prefs) { this.ctx = ctx; this.prefs = prefs; }

    File root() { return new File(ctx.getFilesDir(), "games"); }
    /** SCES-01331 -> sces01331 */
    static String idOf(String serial) { return serial.toLowerCase(Locale.ROOT).replaceAll("[^a-z0-9]", ""); }

    List<Game> games() {
        List<Game> out = new ArrayList<>();
        for (String id : prefs.getString("games", "").split(",")) {
            if (id.isEmpty()) continue;
            Game g = new Game();
            g.id = id;
            g.ver = byKey(prefs.getString("g." + id + ".ver", ""));
            g.serial = prefs.getString("g." + id + ".serial", id.toUpperCase(Locale.ROOT));
            g.name = prefs.getString("g." + id + ".name", "");
            g.info = prefs.getString("g." + id + ".info", "");
            g.disc = prefs.getString("g." + id + ".disc", null);
            g.dir = new File(root(), id);
            out.add(g);
        }
        return out;
    }
    Game get(String id) { for (Game g : games()) if (g.id.equals(id)) return g; return null; }
    /** the selected game: the stored one, else the first ready one, else the first */
    Game selected() {
        List<Game> l = games();
        String id = prefs.getString("game", "");
        for (Game g : l) if (g.id.equals(id)) return g;
        for (Game g : l) if (g.ready()) return g;
        return l.isEmpty() ? null : l.get(0);
    }
    void select(String id) { prefs.edit().putString("game", id).apply(); }

    /** adds or updates an entry (the newest first in the list) and selects it */
    Game put(String serial, Version v, String name, String info) {
        String id = idOf(serial);
        StringBuilder b = new StringBuilder(id);
        for (String o : prefs.getString("games", "").split(",")) if (!o.isEmpty() && !o.equals(id)) b.append(',').append(o);
        prefs.edit().putString("games", b.toString()).putString("game", id)
                .putString("g." + id + ".ver", v.key).putString("g." + id + ".serial", serial)
                .putString("g." + id + ".name", name == null ? "" : name).putString("g." + id + ".info", info == null ? "" : info)
                .remove("g." + id + ".disc").apply();
        new File(root(), id).mkdirs();
        return get(id);
    }
    /** the entry and its extracted data (the saves stay: they are per version, files/saves*) */
    void remove(String id) {
        LauncherActivity.deleteTree(new File(root(), id));
        StringBuilder b = new StringBuilder();
        for (String o : prefs.getString("games", "").split(","))
            if (!o.isEmpty() && !o.equals(id)) { if (b.length() > 0) b.append(','); b.append(o); }
        SharedPreferences.Editor e = prefs.edit().putString("games", b.toString());
        for (String k : new String[]{"ver", "serial", "name", "info", "disc"}) e.remove("g." + id + "." + k);
        if (id.equals(prefs.getString("game", ""))) e.remove("game");
        e.apply();
    }
    void removeAll() {
        for (Game g : games()) remove(g.id);
        LauncherActivity.deleteTree(root());
    }

    /** an install from before the library: files/data (extracted by the launcher) becomes the PAL entry (its own
     *  folder, a rename on the same storage), or an image an even older version copied (prefs "disc") */
    void migrate() {
        File old = new File(ctx.getFilesDir(), "data");
        if (new File(old, "manifest.txt").isFile()) {
            String serial = serialOf(old);
            String id = idOf(serial);
            File dst = new File(new File(root(), id), "data");
            dst.getParentFile().mkdirs();
            LauncherActivity.deleteTree(dst);
            if (old.renameTo(dst) || dst.isDirectory()) {
                put(serial, VERSIONS[0], prefs.getString("disc_name", ""), prefs.getString("disc_info", ""));
                prefs.edit().remove("disc_name").remove("disc_info").remove("disc_ok").apply();
                CrashLog.append(ctx, "launcher: library: the extracted PAL data moved to games/" + id);
            } else CrashLog.append(ctx, "launcher: library: could not move files/data to " + dst);
        }
        LauncherActivity.deleteTree(new File(ctx.getFilesDir(), "data.tmp"));
        String disc = prefs.getString("disc", null);
        if (disc != null && new File(disc).isFile() && games().isEmpty()) {
            Game g = put("SCES-01331", VERSIONS[0], new File(disc).getName(), "");
            prefs.edit().putString("g." + g.id + ".disc", disc).remove("disc").apply();
        }
    }

    /** SCES-01331 from data/SYSTEM.CNF (BOOT = cdrom:\SCES_013.31;1), else the manifest's serial, else SCES-01331 */
    static String serialOf(File data) {
        String s = readSmall(new File(data, "manifest.txt"));
        if (s != null) for (String l : s.split("\n")) if (l.startsWith("serial ") && l.length() > 7 && !l.substring(7).trim().equals("?")) return l.substring(7).trim();
        s = readSmall(new File(data, "SYSTEM.CNF"));
        if (s != null) for (String l : s.split("[\r\n]+")) {
            if (!l.trim().toUpperCase(Locale.ROOT).startsWith("BOOT")) continue;
            String v = l.substring(l.indexOf('=') + 1).trim();
            v = v.substring(v.indexOf(':') + 1).replace('\\', '/');
            v = v.substring(v.lastIndexOf('/') + 1);
            int sc = v.indexOf(';'); if (sc >= 0) v = v.substring(0, sc);
            String u = v.toUpperCase(Locale.ROOT).replace(".", "");
            if (u.length() == 10 && u.charAt(4) == '_') return u.substring(0, 4) + "-" + u.substring(5);
        }
        return "SCES-01331";
    }
    static String readSmall(File f) {
        try (InputStream in = new FileInputStream(f)) {
            byte[] b = new byte[(int) Math.min(f.length(), 1 << 16)];
            int n = 0, r;
            while (n < b.length && (r = in.read(b, n, b.length - n)) > 0) n += r;
            return new String(b, 0, n, "ISO-8859-1");
        } catch (Exception e) { return null; }
    }

    // ---- the card's logo, cut from the player's own disc data ----
    /** logo.png of the game, made from its data on first use; null when the version has no recipe or the files are
     *  not there (the card then shows the name as text) */
    static Bitmap logo(Game g) {
        if (g == null || g.ver == null) return null;
        File f = g.logoFile();
        if (!f.isFile() && g.ver.logoPage != null && g.extracted()) {
            Bitmap b = cutLogo(new File(g.data(), g.ver.logoPage), new File(g.data(), g.ver.logoClut), g.ver.logoOff);
            if (b != null) {
                try (FileOutputStream o = new FileOutputStream(f)) { b.compress(Bitmap.CompressFormat.PNG, 100, o); }
                catch (Exception e) { f.delete(); }
                return b;
            }
        }
        return f.isFile() ? BitmapFactory.decodeFile(f.getPath()) : null;
    }
    /** the title screen's logo as the game assembles it: an 8 bpp page of 256 x 256 texels (VRAM x 256 words) and its
     *  256-colour CLUT (RGB555, 0 = transparent); the word part is texels (8..247, 0..111), the rest of the last letter,
     *  the "!" and the TM are (8..55 / 8..67, 112..223), drawn to the right of it with page row = logo row + off (where
     *  the edge columns continue each other: 112 PAL, 88 USA).  Returns the visible part, about 290 x 112, or null. */
    static Bitmap cutLogo(File page, File clut, int off) {
        byte[] p = readAll(page, 65536), c = readAll(clut, 512);
        if (p == null || c == null || p.length < 65536 || c.length < 512 || off < 0 || off > 112) return null;
        int[] pal = new int[256];
        for (int i = 0; i < 256; i++) {
            int v = (c[2 * i] & 255) | (c[2 * i + 1] & 255) << 8;
            int r = v & 31, gg = v >> 5 & 31, bb = v >> 10 & 31;
            pal[i] = v == 0 ? 0 : 0xFF000000 | (r << 3 | r >> 2) << 16 | (gg << 3 | gg >> 2) << 8 | (bb << 3 | bb >> 2);
        }
        int w = 240 + 64, h = 224 - off;
        int[] px = new int[w * h];
        for (int y = 0; y < h; y++) {
            if (y < 112) for (int x = 0; x < 240; x++) px[y * w + x] = pal[p[y * 256 + 8 + x] & 255];
            int py = y + off;
            if (py >= 112 && py < 224) for (int x = 0; x < 64; x++) px[y * w + 240 + x] = pal[p[py * 256 + 8 + x] & 255];
        }
        // the visible extent (the page has transparent margins)
        int x0 = w, x1 = -1, y0 = h, y1 = -1;
        for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) if (px[y * w + x] != 0) {
            if (x < x0) x0 = x; if (x > x1) x1 = x; if (y < y0) y0 = y; if (y > y1) y1 = y;
        }
        if (x1 < x0 + 32 || y1 < y0 + 16) return null;     // not the expected picture: no logo
        Bitmap b = Bitmap.createBitmap(x1 - x0 + 1, y1 - y0 + 1, Bitmap.Config.ARGB_8888);
        b.setPixels(px, y0 * w + x0, w, 0, 0, x1 - x0 + 1, y1 - y0 + 1);
        return b;
    }
    static byte[] readAll(File f, int max) {
        try (InputStream in = new FileInputStream(f)) {
            byte[] b = new byte[(int) Math.min(f.length(), max)];
            int n = 0, r;
            while (n < b.length && (r = in.read(b, n, b.length - n)) > 0) n += r;
            return n == b.length ? b : null;
        } catch (Exception e) { return null; }
    }
}

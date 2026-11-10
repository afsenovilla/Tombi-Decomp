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
 * cannot be played.  The card's logo is cut at run time from the player's own disc (the title screen's logo page, its
 * palette and the title overlay's sprite list: Version.logoPage / logoClut / logoOvl, file names only; from the
 * extracted data, or straight from the image when the game is added and not extracted, Japan); without it the card
 * shows the name as text.
 *
 * Kept in SharedPreferences "launcher": games = "id,id,...", game = the selected id, g.&lt;id&gt;.(ver|serial|name|info|disc).
 * Installs from before the library (files/data, or an image copied by an even older version: prefs "disc") become the
 * PAL entry on first start (migrate()).
 */
final class Library {
    /** a release the launcher knows: its boot executable's SHA-1, names, game library (null: no port yet) */
    static final class Version {
        final String key, sha1, title, logoText, lib, saveSub, logoPage, logoClut, logoOvl;
        final int region;          // 0 Europe, 1 USA, 2 Japan (R.array.lib_regions)
        Version(String key, String sha1, String title, String logoText, int region, String lib, String saveSub, String logoPage, String logoClut, String logoOvl) {
            this.key = key; this.sha1 = sha1; this.title = title; this.logoText = logoText; this.region = region; this.lib = lib;
            this.saveSub = saveSub; this.logoPage = logoPage; this.logoClut = logoClut; this.logoOvl = logoOvl;
        }
        boolean pal() { return "pal".equals(key); }
        boolean supported() { return lib != null; }
    }
    static final Version[] VERSIONS = {
        // SCES-01331 / SCES-01330 Rev 1: EXE/MAIN0.EXE (the boot exe only loads it)
        new Version("pal", "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc", "Tombi!", "TOMBI!", 0, "libtombi.so", "saves",
                    "SYSTEM/A00202.000", "SYSTEM/A00203.000", "SYSTEM/O0PTSUB0.BIN"),
        new Version("usa", "81cbc79f0230aeb4252e058039f47ac95a777f5a", "Tomba!", "TOMBA!", 1, "libtombi_usa.so", "saves_usa",
                    "SYSTEM/A00012.000", "SYSTEM/A00013.000", "SYSTEM/OPTSUB00.BIN"),
        // SLPS-01144: no port yet, not extracted; its logo is read from the image when it is added (logoFromImage)
        new Version("jp", "07d577698ad31ec1a69b9d7b80a7c3f90b650888", "オレっ！トンバ", "オレっ！トンバ", 2, null, null,
                    "SYSTEM/A00012.000", "SYSTEM/A00013.000", "SYSTEM/OPTSUB00.BIN"),
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

    // ---- the card's logo, cut from the player's own disc ----
    /** logo.png of the game (in its folder): made from its extracted data on first use; a game the launcher does not
     *  extract (Japan) has the one logoFromImage made when it was added.  null when there is none (the card then
     *  shows the name as text) */
    static Bitmap logo(Game g) {
        if (g == null || g.ver == null) return null;
        File f = g.logoFile();
        if (!f.isFile() && g.ver.logoPage != null && g.extracted()) {
            File d = g.data();
            Bitmap b = cutLogo(readAll(new File(d, g.ver.logoPage), 1 << 20), readAll(new File(d, g.ver.logoClut), 1 << 20),
                               readAll(new File(d, g.ver.logoOvl), 4 << 20));
            if (b != null) { save(b, f); return b; }
        }
        return f.isFile() ? BitmapFactory.decodeFile(f.getPath()) : null;
    }
    /** the logo of a game not extracted, read straight from its disc image (disccheck.c nativeReadFiles: the image the
     *  player picked, or the temp image of a .zip / .7z while it is still there) into logo.png; null when it fails */
    static Bitmap logoFromImage(Game g, String image) {
        if (g == null || g.ver == null || g.ver.logoPage == null || image == null || !LauncherActivity.nativeOk) return null;
        byte[][] r = LauncherActivity.nativeReadFiles(image, new String[]{g.ver.logoPage, g.ver.logoClut, g.ver.logoOvl});
        if (r == null || r.length != 3) return null;
        Bitmap b = cutLogo(r[0], r[1], r[2]);
        if (b != null) { g.dir.mkdirs(); save(b, g.logoFile()); }
        return b;
    }
    static void save(Bitmap b, File f) {
        try (FileOutputStream o = new FileOutputStream(f)) { b.compress(Bitmap.CompressFormat.PNG, 100, o); }
        catch (Exception e) { f.delete(); }
    }

    /** the title screen's logo as the game draws it (tools/game_library.py logo_sprites / _compose): the title overlay
     *  (O0PTSUB0 PAL, OPTSUB00 USA / Japan) has the list TITLE_LOGO_SPRITE_RECS (src/o0ptsub0/titleLogoDraw.c) of
     *  16-byte records {x, y, u, v, w, h, tpage, clut x, clut y} ending at x = -1, the logo's with the CLUT at
     *  (128, 510); each sprite is a part of the 8 bpp page (256 texels a row) drawn at (x, y) on the screen with the
     *  page's 256-colour CLUT (RGB555, 0 = transparent).  Returns the opaque extent (about 290 x 110), or null. */
    static Bitmap cutLogo(byte[] page, byte[] clut, byte[] ovl) {
        if (page == null || clut == null || ovl == null || page.length < 256 || clut.length < 512) return null;
        int[] spr = logoSprites(ovl);
        if (spr == null) return null;
        int[] pal = new int[256];
        for (int i = 0; i < 256; i++) {
            int v = (clut[2 * i] & 255) | (clut[2 * i + 1] & 255) << 8;
            int r = v & 31, gg = v >> 5 & 31, bb = v >> 10 & 31;
            pal[i] = v == 0 ? 0 : 0xFF000000 | (r << 3 | r >> 2) << 16 | (gg << 3 | gg >> 2) << 8 | (bb << 3 | bb >> 2);
        }
        int ph = page.length / 256;
        int sx0 = Integer.MAX_VALUE, sy0 = Integer.MAX_VALUE, sx1 = Integer.MIN_VALUE, sy1 = Integer.MIN_VALUE;
        for (int k = 0; k < spr.length; k += 6) {
            sx0 = Math.min(sx0, spr[k]); sy0 = Math.min(sy0, spr[k + 1]);
            sx1 = Math.max(sx1, spr[k] + spr[k + 4]); sy1 = Math.max(sy1, spr[k + 1] + spr[k + 5]);
        }
        int w = sx1 - sx0, h = sy1 - sy0;
        int[] px = new int[w * h];
        // the sprites in list order (a later one over an earlier one), only their opaque texels
        for (int k = 0; k < spr.length; k += 6) {
            int x = spr[k] - sx0, y = spr[k + 1] - sy0, u = spr[k + 2], v = spr[k + 3], sw = spr[k + 4], sh = spr[k + 5];
            for (int yy = v; yy < Math.min(ph, v + sh); yy++)
                for (int xx = u; xx < Math.min(256, u + sw); xx++) {
                    int c = pal[page[yy * 256 + xx] & 255];
                    if (c != 0) px[(y + yy - v) * w + x + xx - u] = c;
                }
        }
        int x0 = w, x1 = -1, y0 = h, y1 = -1;
        for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) if (px[y * w + x] != 0) {
            if (x < x0) x0 = x; if (x > x1) x1 = x; if (y < y0) y0 = y; if (y > y1) y1 = y;
        }
        if (x1 < x0 + 32 || y1 < y0 + 16) return null;     // not the expected picture: no logo
        Bitmap b = Bitmap.createBitmap(x1 - x0 + 1, y1 - y0 + 1, Bitmap.Config.ARGB_8888);
        b.setPixels(px, y0 * w + x0, w, 0, 0, x1 - x0 + 1, y1 - y0 + 1);
        return b;
    }
    /** {x, y, u, v, w, h} per sprite of the first record list in the overlay whose records use the logo CLUT (128, 510)
     *  and that ends at x = -1 (tools/game_library.py logo_sprites), or null */
    static int[] logoSprites(byte[] o) {
        for (int s = 0; s + 32 < o.length; s += 2) {
            int n = 0, q = s;
            while (q + 16 <= o.length) {
                int u = o[q + 4] & 255, v = o[q + 5] & 255, w = le16(o, q + 6), h = le16(o, q + 8);
                if (le16(o, q + 12) != 128 || le16(o, q + 14) != 510 || w <= 0 || w > 256 || h <= 0 || h > 256 || u + w > 256 || v + h > 256) break;
                n++; q += 16;
            }
            if (n > 0 && q + 16 <= o.length && le16(o, q) == -1) {
                int[] r = new int[n * 6];
                for (int i = 0; i < n; i++) {
                    int b = s + 16 * i;
                    r[6 * i] = le16(o, b); r[6 * i + 1] = le16(o, b + 2); r[6 * i + 2] = o[b + 4] & 255; r[6 * i + 3] = o[b + 5] & 255;
                    r[6 * i + 4] = le16(o, b + 6); r[6 * i + 5] = le16(o, b + 8);
                }
                return r;
            }
        }
        return null;
    }
    /** a signed little-endian 16-bit value */
    static int le16(byte[] b, int o) { return (short) ((b[o] & 255) | (b[o + 1] & 255) << 8); }
    /** the file's bytes (at most max), null when it cannot be read */
    static byte[] readAll(File f, int max) {
        try (InputStream in = new FileInputStream(f)) {
            byte[] b = new byte[(int) Math.min(f.length(), max)];
            int n = 0, r;
            while (n < b.length && (r = in.read(b, n, b.length - n)) > 0) n += r;
            return n == b.length ? b : null;
        } catch (Exception e) { return null; }
    }
}

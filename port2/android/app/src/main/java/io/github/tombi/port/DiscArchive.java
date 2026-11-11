package io.github.tombi.port;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * What the player picked when adding a disc (LauncherActivity.importDisc), before the native ISO reader
 * (disccheck.c) sees it: a disc image, an archive holding one, or something else.  Plain Java (java.util.zip), no
 * Android classes, so it runs on a PC as well (port2/android/render).
 *
 * A .zip is read as a stream (ZipInputStream over the provider's InputStream: any document provider, no copy of the
 * zip itself): the disc image inside it (in any folder, any case: .bin / .img / .iso / .chd; with a .cue, the data
 * track it names, else the largest) is written to the app's cache, where the ISO reader has the random access it
 * needs, and deleted by the caller after the extraction.  The same shapes as tools/disc_unpack.sh (the CI's game zips):
 * the image in a folder with its .cue, or a zip of the disc's extracted files.  The latter cannot be played (the game
 * reads the disc by sector number, which the loose files have lost), but it is identified (SYSTEM.CNF BOOT and that
 * file's SHA-1, MAIN0.EXE on the PAL discs) so the message can name the game.  A .7z needs random access (its index
 * is at the end): it is read in place (or copied to the cache) by port2/android/un7z.c, whose answer fromNative()
 * turns into the same Result.  A .rar is recognised by its magic and refused with a message (no decoder in the app).
 */
final class DiscArchive {
    static final int IMAGE = 0, ZIP = 1, SEVENZ = 2, RAR = 3, CUE = 4, NOT_IMAGE = 5;
    /** smaller "images" are not a disc (a PlayStation disc's volume is tens of MB at least) */
    static final long MIN_IMAGE = 4L << 20;

    /** the kind of a picked file from its first bytes (at least 16 * 2352 + 48 when the file is that long) and name */
    static int kind(byte[] h, int n, String name) {
        String low = name == null ? "" : name.toLowerCase(Locale.ROOT);
        if (n >= 4 && h[0] == 'P' && h[1] == 'K' && (h[2] == 3 || h[2] == 5 || h[2] == 7) && (h[3] == 4 || h[3] == 6 || h[3] == 8)) return ZIP;
        if (n >= 6 && h[0] == '7' && h[1] == 'z' && (h[2] & 255) == 0xBC && (h[3] & 255) == 0xAF && h[4] == 0x27 && h[5] == 0x1C) return SEVENZ;
        if (n >= 4 && h[0] == 'R' && h[1] == 'a' && h[2] == 'r' && h[3] == '!') return RAR;
        if (low.endsWith(".7z")) return SEVENZ;
        if (low.endsWith(".rar")) return RAR;
        if (low.endsWith(".zip")) return ZIP;
        if (n >= 8 && new String(h, 0, 8, java.nio.charset.StandardCharsets.ISO_8859_1).equals("MComprHD")) return IMAGE;   // .chd
        if (low.endsWith(".cue")) return CUE;
        // a raw image starts with the CD sync pattern; the volume descriptor (CD001) is at sector 16 (2048 or 2352 bytes)
        boolean sync = n >= 12 && h[0] == 0 && h[11] == 0;
        for (int i = 1; sync && i < 11; i++) if ((h[i] & 255) != 0xFF) sync = false;
        if (sync) return IMAGE;
        if (cd001(h, n, 16 * 2048 + 1) || cd001(h, n, 16 * 2352 + 25) || cd001(h, n, 16 * 2352 + 17)) return IMAGE;
        return n < 16 * 2048 + 6 ? IMAGE : NOT_IMAGE;    // too short to tell: the ISO reader decides (and says why)
    }
    static int headBytes() { return 16 * 2352 + 48; }
    private static boolean cd001(byte[] h, int n, int o) {
        return n >= o + 5 && h[o] == 'C' && h[o + 1] == 'D' && h[o + 2] == '0' && h[o + 3] == '0' && h[o + 4] == '1';
    }

    /** progress and Cancel while unzipping: bytes of the zip read so far */
    interface Progress { void read(long zipBytes); boolean cancelled(); }

    /** not enough room in the cache for the image (MB) */
    static final class SpaceException extends IOException {
        final long needMb, freeMb;
        SpaceException(long need, long free) { super("space"); needMb = need >> 20; freeMb = free >> 20; }
    }

    static final class Result {
        File image;              // the disc image written to the cache (null: none in the zip)
        String entry;            // its path inside the zip
        long imageBytes;
        boolean looseFiles;      // a zip of the extracted files (SYSTEM.CNF, no image)
        String boot, bootSha1;   // looseFiles: the game executable SYSTEM.CNF names and its SHA-1 (MAIN0.EXE on PAL)
        String serial;           // looseFiles: SCUS-94236 from the boot name
        String cueMissing;       // a .cue whose data track is not in the zip (and no other image)
        boolean archiveInside;   // no image, but a .7z / .rar / .zip / .ecm inside
        int entries;
    }

    /**
     * Streams the zip, writing every disc-image entry into dir (named after the entry), hashing the small executables
     * and reading SYSTEM.CNF / .cue; then keeps the data track (the .cue's first FILE, else the largest image) and
     * deletes the others.  Room checked before each image: twice its size (the image, then its extracted files) plus
     * reserve.  Throws SpaceException, InterruptedException (cancelled),
     * java.util.zip.ZipException (a damaged zip or an unsupported compression method).
     */
    static Result unzip(InputStream raw, File dir, long reserve, final Progress p) throws IOException, InterruptedException {
        final long[] count = {0};
        InputStream counted = new FilterInputStream(raw) {
            @Override public int read() throws IOException { int b = super.read(); if (b >= 0) tick(1); return b; }
            @Override public int read(byte[] b, int o, int l) throws IOException { int n = super.read(b, o, l); if (n > 0) tick(n); return n; }
            @Override public long skip(long n) throws IOException { long k = super.skip(n); if (k > 0) tick(k); return k; }
            void tick(long n) { count[0] += n; if (p != null) p.read(count[0]); }
        };
        Result r = new Result();
        dir.mkdirs();
        Map<String, File> images = new HashMap<>();       // entry path -> file in dir
        Map<String, String> exeSha = new HashMap<>();     // upper-case basename -> sha1
        String cnf = null; int cnfDepth = 99;
        String cueFile = null;
        byte[] buf = new byte[1 << 18];
        try (ZipInputStream z = new ZipInputStream(counted)) {
            ZipEntry e;
            while ((e = z.getNextEntry()) != null) {
                if (p != null && p.cancelled()) throw new InterruptedException();
                if (e.isDirectory()) continue;
                String path = e.getName().replace('\\', '/');
                if (path.startsWith("__MACOSX/") || path.contains("/__MACOSX/")) continue;
                r.entries++;
                String base = path.substring(path.lastIndexOf('/') + 1), low = base.toLowerCase(Locale.ROOT);
                long size = e.getSize();
                if (low.endsWith(".bin") || low.endsWith(".img") || low.endsWith(".iso") || low.endsWith(".chd")) {
                    if (size >= 0 && size < MIN_IMAGE) continue;
                    long free = dir.getUsableSpace();
                    if (size >= 0 && free < 2 * size + reserve) throw new SpaceException(2 * size + reserve, free);
                    File out = new File(dir, unique(images.size(), safeName(base)));
                    long got = 0;
                    try (FileOutputStream o = new FileOutputStream(out)) {
                        int n;
                        while ((n = z.read(buf)) > 0) {
                            if (p != null && p.cancelled()) throw new InterruptedException();
                            o.write(buf, 0, n); got += n;
                            if (size < 0 && (got & ((64L << 20) - 1)) < n && dir.getUsableSpace() < got + reserve)   // (size unknown: as it grows)
                                throw new SpaceException(2 * got + reserve, got + dir.getUsableSpace());
                        }
                        o.getFD().sync();
                    } catch (IOException x) {
                        out.delete();
                        if (x instanceof SpaceException) throw x;
                        long f = dir.getUsableSpace();
                        if (f < (64L << 20)) throw new SpaceException(Math.max(size, got) + reserve, got + f);
                        throw x;
                    }
                    if (got < MIN_IMAGE) { out.delete(); continue; }
                    images.put(path, out);
                } else if (low.equals("system.cnf")) {
                    int depth = path.split("/").length;
                    String s = text(z, 1 << 16);
                    if (depth < cnfDepth) { cnf = s; cnfDepth = depth; }
                } else if (low.endsWith(".cue")) {
                    if (cueFile == null) cueFile = cueData(text(z, 1 << 16));
                } else if ((low.equals("main0.exe") || low.matches("[a-z]{4}[_-]\\d{3}\\.\\d{2}(;1)?")) && (size < 0 || size < (8L << 20))) {
                    exeSha.put(base.toUpperCase(Locale.ROOT).replace(";1", ""), sha1(z, buf));
                } else if (low.endsWith(".7z") || low.endsWith(".rar") || low.endsWith(".zip") || low.endsWith(".ecm") || low.endsWith(".pbp")) {
                    r.archiveInside = true;
                }
            }
        }
        // the data track: the .cue's first FILE, else the largest image
        String pick = null;
        if (cueFile != null) for (String k : images.keySet()) if (k.substring(k.lastIndexOf('/') + 1).equalsIgnoreCase(cueFile)) pick = k;
        if (pick == null) for (String k : images.keySet()) if (pick == null || images.get(k).length() > images.get(pick).length()) pick = k;
        for (Map.Entry<String, File> me : images.entrySet()) if (!me.getKey().equals(pick)) me.getValue().delete();
        if (pick != null) { r.image = images.get(pick); r.entry = pick; r.imageBytes = r.image.length(); return r; }
        if (cueFile != null) r.cueMissing = cueFile;
        loose(r, cnf, exeSha);
        return r;
    }

    /** the loose files of a disc: the game executable SYSTEM.CNF names and its SHA-1 (MAIN0.EXE on the PAL discs) */
    /** A zip of the disc's loose files: the members whose path ends with one of `wanted` (DIR/NAME, case-insensitive),
     *  read whole (they are small) -> {wanted name: bytes}; streamed once, nothing written. */
    static Map<String, byte[]> members(InputStream raw, String[] wanted) throws IOException {
        Map<String, byte[]> out = new HashMap<>();
        if (raw == null) return out;
        try (ZipInputStream z = new ZipInputStream(raw)) {
            ZipEntry e;
            while ((e = z.getNextEntry()) != null) {
                if (e.isDirectory()) continue;
                String path = e.getName().replace('\\', '/').toUpperCase(Locale.ROOT);
                for (String w : wanted) {
                    String wu = w.toUpperCase(Locale.ROOT);
                    if ((path.equals(wu) || path.endsWith("/" + wu)) && !out.containsKey(w)) {
                        java.io.ByteArrayOutputStream b = new java.io.ByteArrayOutputStream();
                        byte[] buf = new byte[1 << 16]; int n;
                        while ((n = z.read(buf)) > 0) { b.write(buf, 0, n); if (b.size() > (64 << 20)) throw new IOException("member too large: " + w); }
                        out.put(w, b.toByteArray());
                    }
                }
            }
        }
        return out;
    }

    static void loose(Result r, String cnf, Map<String, String> exeSha) {
        if (cnf == null) return;
        r.looseFiles = true;
        String boot = bootOf(cnf);
        if (boot == null) return;
        String b = boot.substring(boot.lastIndexOf('/') + 1).toUpperCase(Locale.ROOT);
        r.serial = serialOf(b);
        if (b.startsWith("SCES_") && exeSha.containsKey("MAIN0.EXE")) { r.boot = "EXE/MAIN0.EXE"; r.bootSha1 = exeSha.get("MAIN0.EXE"); }
        else { r.boot = boot; r.bootSha1 = exeSha.get(b); }
    }

    /** the answer of LauncherActivity.nativeUn7z (port2/android/un7z.c: "ok\n" + lines) as a Result; dir = its outDir */
    static Result fromNative(String s, File dir) {
        Result r = new Result();
        Map<String, String> exe = new HashMap<>();
        String cnf = null;
        for (String l : s.split("\n")) {
            if (l.startsWith("image ")) {
                int t = l.indexOf('\t');
                r.image = new File(dir, t > 0 ? l.substring(6, t) : l.substring(6));
                r.entry = t > 0 ? l.substring(t + 1) : r.image.getName();
                r.imageBytes = r.image.length();
            } else if (l.startsWith("cue ")) r.cueMissing = l.substring(4);
            else if (l.equals("cnf")) {
                byte[] b = Library.readAll(new File(dir, "SYSTEM.CNF"), 1 << 16);   // (its whole length, at most 64 KB)
                cnf = b == null ? "" : new String(b, java.nio.charset.StandardCharsets.ISO_8859_1);
            } else if (l.startsWith("exe ")) {
                String[] p = l.split(" ");
                if (p.length >= 3) exe.put(p[1].toUpperCase(Locale.ROOT).replace(";1", ""), p[2]);
            } else if (l.equals("nested")) r.archiveInside = true;
            else if (l.startsWith("entries ")) try { r.entries = Integer.parseInt(l.substring(8).trim()); } catch (NumberFormatException ignored) { }
        }
        if (r.image == null) loose(r, cnf, exe);
        return r;
    }

    /** "cdrom:\SCUS_942.36;1" of BOOT = ... -> "SCUS_942.36" (path with '/'), or null */
    static String bootOf(String cnf) {
        for (String l : cnf.split("[\r\n]+")) {
            String t = l.trim();
            if (!t.toUpperCase(Locale.ROOT).startsWith("BOOT") || t.indexOf('=') < 0) continue;
            String v = t.substring(t.indexOf('=') + 1).trim();
            int c = v.indexOf(':'); if (c >= 0) v = v.substring(c + 1);
            v = v.replace('\\', '/');
            while (v.startsWith("/")) v = v.substring(1);
            int sc = v.indexOf(';'); if (sc >= 0) v = v.substring(0, sc);
            int sp = v.indexOf(' '); if (sp >= 0) v = v.substring(0, sp);
            return v.isEmpty() ? null : v;
        }
        return null;
    }
    static String serialOf(String bootBase) {
        String u = bootBase.toUpperCase(Locale.ROOT).replace(".", "");
        return u.length() == 10 && u.charAt(4) == '_' ? u.substring(0, 4) + "-" + u.substring(5) : null;
    }
    /** the file name of the first FILE line of a .cue sheet (the data track), or null */
    static String cueData(String cue) {
        for (String l : cue.split("[\r\n]+")) {
            String t = l.trim();
            if (!t.toUpperCase(Locale.ROOT).startsWith("FILE ")) continue;
            t = t.substring(5).trim();
            String nm;
            if (t.startsWith("\"")) { int q = t.indexOf('"', 1); nm = q > 0 ? t.substring(1, q) : t.substring(1); }
            else { int s = t.lastIndexOf(' '); nm = s > 0 ? t.substring(0, s) : t; }
            nm = nm.replace('\\', '/');
            return nm.substring(nm.lastIndexOf('/') + 1);
        }
        return null;
    }
    static String safeName(String base) {
        String s = base.replaceAll("[^A-Za-z0-9._()\\[\\] !,+-]", "_");
        return s.isEmpty() || s.startsWith(".") ? "image" + s : s;
    }
    private static String unique(int i, String n) { return i == 0 ? n : i + "-" + n; }
    private static String text(InputStream in, int max) throws IOException {
        byte[] b = new byte[max]; int n = 0, r;
        while (n < max && (r = in.read(b, n, max - n)) > 0) n += r;
        return new String(b, 0, n, java.nio.charset.StandardCharsets.ISO_8859_1);
    }
    private static String sha1(InputStream in, byte[] buf) throws IOException {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-1");
            int n;
            while ((n = in.read(buf)) > 0) md.update(buf, 0, n);
            StringBuilder s = new StringBuilder();
            for (byte x : md.digest()) s.append(String.format(Locale.ROOT, "%02x", x & 255));
            return s.toString();
        } catch (java.security.NoSuchAlgorithmException e) { throw new IOException(e); }
    }

    /** the entries' names, for the log */
    static List<String> list(File dir) {
        List<String> l = new ArrayList<>();
        File[] k = dir.listFiles();
        if (k != null) for (File f : k) l.add(f.getName() + " " + f.length());
        return l;
    }
}

package io.github.tombi.port;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

/** Save import / export for the port's memory card (raw 128 KB tombi_mc0.mcd, the same layout as on PC; shims/card.c).
 *  Mirrors tools/save_import.py: whole cards (raw .mcd/.mcr/..., DexDrive .gme, .vgs/.mem) replace the card; single
 *  saves (.mcs/.ps1/.mcb, PS3 .psv) go into free blocks with proper directory frames, and one whose name is already on
 *  the card raises Conflict (the launcher asks: replace it / the other card / cancel).  Every write first copies the
 *  card into saves/backup/ (the last 5 of each card kept). */
final class SaveImport {
    static final int CARD = 0x20000, BLOCK = 0x2000, FRAME = 0x80;
    static final String[] TOMBI = {"BESCES-01330", "BESCES-01331", "BASCUS-94236", "BISLPS-01144"};

    static class Fail extends Exception { Fail(String m) { super(m); } }

    /** result of an import: lines to show, plus a warning (null if none) */
    static final class Result { final List<String> lines = new ArrayList<>(); String warn; }

    static byte[] readAll(InputStream in, int max) throws IOException {
        ByteArrayOutputStream o = new ByteArrayOutputStream();
        byte[] b = new byte[16384];
        int n;
        while ((n = in.read(b)) > 0) { o.write(b, 0, n); if (o.size() > max) throw new IOException("file too big"); }
        return o.toByteArray();
    }

    static int u8(byte[] a, int o) { return a[o] & 0xFF; }
    static int u16(byte[] a, int o) { return u8(a, o) | u8(a, o + 1) << 8; }
    static void p16(byte[] a, int o, int v) { a[o] = (byte) v; a[o + 1] = (byte) (v >> 8); }
    static void p32(byte[] a, int o, int v) { p16(a, o, v); p16(a, o + 2, v >> 16); }
    static boolean at(byte[] a, int o, String s) {
        if (a.length < o + s.length()) return false;
        for (int i = 0; i < s.length(); i++) if (a[o + i] != (byte) s.charAt(i)) return false;
        return true;
    }
    static void fixFrame(byte[] c, int o) { int x = 0; for (int i = 0; i < 127; i++) x ^= c[o + i]; c[o + 127] = (byte) x; }
    static byte[] slice(byte[] a, int o, int n) { byte[] r = new byte[n]; System.arraycopy(a, o, r, 0, n); return r; }

    static byte[] blankCard() {
        byte[] c = new byte[CARD];
        c[0] = 'M'; c[1] = 'C'; fixFrame(c, 0);
        for (int i = 1; i < 16; i++) { int o = i * FRAME; c[o] = (byte) 0xA0; c[o + 8] = c[o + 9] = (byte) 0xFF; fixFrame(c, o); }
        for (int i = 16; i < 36; i++) { int o = i * FRAME; for (int k = 0; k < 4; k++) c[o + k] = (byte) 0xFF; c[o + 8] = c[o + 9] = (byte) 0xFF; fixFrame(c, o); }
        c[63 * FRAME] = 'M'; c[63 * FRAME + 1] = 'C'; fixFrame(c, 63 * FRAME);
        return c;
    }

    static String name(byte[] f, int o) {
        StringBuilder s = new StringBuilder();
        for (int i = 10; i < 30 && f[o + i] != 0; i++) s.append((char) (f[o + i] & 0x7F));
        return s.toString();
    }
    /** the save's title (Shift-JIS at 'SC' + 4), fullwidth ASCII folded to plain */
    static String title(byte[] d, int o) {
        try {
            int n = 0;
            while (n < 64 && d[o + 4 + n] != 0) n++;
            String t = new String(d, o + 4, n, "Shift_JIS");
            StringBuilder s = new StringBuilder();
            for (char ch : t.toCharArray()) s.append(ch >= 0xFF01 && ch <= 0xFF5E ? (char) (ch - 0xFEE0) : ch == 0x3000 ? ' ' : ch);
            return s.toString().trim();
        } catch (Exception e) { return ""; }
    }
    static boolean isTombi(String n) {
        for (String p : TOMBI) if (n.startsWith(p)) return true;
        return n.contains("TOMBA");
    }
    static List<String> list(byte[] c) {
        List<String> r = new ArrayList<>();
        for (int i = 1; i < 16; i++) if (u8(c, i * FRAME) == 0x51) r.add(name(c, i * FRAME) + "  " + title(c, i * BLOCK));
        return r;
    }

    /** whole card from a container, or null if raw is not one */
    static byte[] asCard(byte[] raw) {
        if (raw.length == CARD && at(raw, 0, "MC")) return raw;
        if (at(raw, 0, "123-456-STD") && raw.length >= 0xF40 + CARD && at(raw, 0xF40, "MC")) return slice(raw, 0xF40, CARD);
        if (at(raw, 0, "VgsM") && raw.length >= 64 + CARD && at(raw, 64, "MC")) return slice(raw, 64, CARD);
        return null;
    }

    static final int BACKUP_KEEP = 5;
    /** a copy of the card in backup/ next to it (files/saves/backup/tombi_mc0-YYYYMMDD-HHMMSS.mcd); only the newest
     *  BACKUP_KEEP copies of that card are kept.  Returns the copy (null when there is no card yet). */
    static File backup(File f) throws IOException {
        if (!f.exists()) return null;
        File d = new File(f.getParentFile(), "backup");
        d.mkdirs();
        String n = f.getName(), stem = n.endsWith(".mcd") ? n.substring(0, n.length() - 4) : n;
        String stamp = new SimpleDateFormat("yyyyMMdd-HHmmss", Locale.ROOT).format(new Date());
        File b = new File(d, stem + "-" + stamp + ".mcd");
        for (int k = 2; b.exists(); k++) b = new File(d, stem + "-" + stamp + "-" + k + ".mcd");
        write(b, readFile(f));
        File[] all = d.listFiles();
        if (all != null) {
            List<File> mine = new ArrayList<>();
            for (File x : all) if (x.getName().startsWith(stem + "-") && x.getName().endsWith(".mcd")) mine.add(x);
            java.util.Collections.sort(mine, new java.util.Comparator<File>() {
                public int compare(File a, File c) {
                    int r = Long.compare(a.lastModified(), c.lastModified());
                    return r != 0 ? r : a.getName().compareTo(c.getName());
                } });
            for (int i = 0; i < mine.size() - BACKUP_KEEP; i++) if (!mine.get(i).equals(b)) mine.get(i).delete();
        }
        return b;
    }
    static byte[] readFile(File f) throws IOException { FileInputStream in = new FileInputStream(f); try { return readAll(in, 1 << 20); } finally { in.close(); } }
    static void write(File f, byte[] d) throws IOException { FileOutputStream o = new FileOutputStream(f); try { o.write(d); } finally { o.close(); } }

    /** card 1 = tombi_mc0.mcd, card 2 = tombi_mc1.mcd (the game's memory card 1 / 2) */
    static int cardNumber(File f) { return f.getName().contains("mc1") ? 2 : 1; }
    static File otherCard(File f) {
        String n = f.getName();
        return new File(f.getParentFile(), n.contains("mc0") ? n.replace("mc0", "mc1") : n.replace("mc1", "mc0"));
    }
    static byte[] readCard(File f) throws IOException {
        byte[] c = f.exists() ? readFile(f) : null;
        return c == null || c.length != CARD || !at(c, 0, "MC") ? blankCard() : c;
    }

    // GAME.selectedArea -> name (tools/save_import.py AREAS, tools/tombi_save_editor.py)
    static final String[] AREAS = {"Village of All Beginnings", "Forest / Watch Tower", "Caves / tunnels", "Phoenix Mountain",
            "Trick Village area", "Baccus Village", "Dirt Motocross", null, "Baccus Lake", "Mushroom Village", "Deep Jungle",
            "Lumberjack Town / Factory", null, "Pig Island", "Evil Pigs", null, "Stonetown / Clock Tower",
            "Iron Town / Iron Castle", "Civilization Town", "Dwarf Village"};
    /** one line naming a save for the user (as save_import.save_info): its slot and, when the game data parses (a
     *  Tombi! save whose GAME checksum is right, docs/SAVE_FORMAT.md), the area Continue loads and the play time;
     *  else its title.  d[o..] = the save's first block.  PS1 saves store no date. */
    static String info(String nm, byte[] d, int o, boolean es) {
        String slot = nm.length() >= 2 && isTombi(nm) && Character.isDigit(nm.charAt(nm.length() - 1))
                && Character.isDigit(nm.charAt(nm.length() - 2)) ? nm.substring(nm.length() - 2) : null;
        String head = slot != null ? (es ? "Ranura " : "Slot ") + slot : nm;
        int g = o + 0x200;
        if (slot != null && at(d, o, "SC") && d.length >= g + 0xA00) {
            int x = 0;
            for (int i = 0; i < 0x9FF; i++) x ^= d[g + i];
            if ((byte) x == d[g + 0x9FF]) {
                int area = u16(d, g);
                long t = (u16(d, g + 8) | (long) u16(d, g + 10) << 16);
                StringBuilder s = new StringBuilder(head);
                if (area < AREAS.length && AREAS[area] != null) s.append(" · ").append(AREAS[area]);
                s.append(" · ").append(t / 180000).append(':').append(String.format(Locale.ROOT, "%02d", t / 3000 % 60)).append(" h");
                return s.toString();
            }
        }
        String t = title(d, o);
        return t.isEmpty() ? head : head + "  " + t;
    }

    /** the blocks of the save that starts at b (following the directory frames' link field) */
    static List<Integer> chain(byte[] c, int b) {
        List<Integer> r = new ArrayList<>();
        while (b >= 1 && b <= 15 && !r.contains(b)) {
            r.add(b);
            int nx = u16(c, b * FRAME + 8);
            if (nx == 0xFFFF) break;
            b = nx + 1;
        }
        return r;
    }
    /** first block of the save called nm, or -1 */
    static int find(byte[] c, String nm) {
        for (int i = 1; i < 16; i++) if (u8(c, i * FRAME) == 0x51 && name(c, i * FRAME).equals(nm)) return i;
        return -1;
    }
    static List<Integer> free(byte[] c) {
        List<Integer> r = new ArrayList<>();
        for (int i = 1; i < 16; i++) { int t = u8(c, i * FRAME); if (t == 0 || (t >= 0xA0 && t <= 0xA3)) r.add(i); }
        return r;
    }
    /** frees the save's blocks (directory frames back to the formatted 'free' state, data zeroed); returns them */
    static List<Integer> delete(byte[] c, int b) {
        List<Integer> bl = chain(c, b);
        for (int x : bl) {
            int o = x * FRAME;
            java.util.Arrays.fill(c, o, o + FRAME, (byte) 0);
            c[o] = (byte) 0xA0; c[o + 8] = c[o + 9] = (byte) 0xFF;
            fixFrame(c, o);
            java.util.Arrays.fill(c, x * BLOCK, (x + 1) * BLOCK, (byte) 0);
        }
        return bl;
    }

    /** the target card already holds a save with the imported save's name: the launcher asks (Replace / the other
     *  card when otherOk / Cancel) and imports again with overwrite or into otherCard(dest) */
    static final class Conflict extends Fail {
        final File dest; final String existing, incoming; final boolean otherOk;
        Conflict(File dest, String existing, String incoming, boolean otherOk, boolean es) {
            super(es ? "Ya hay una partida «" + existing + "» en la tarjeta " + cardNumber(dest)
                     : "Memory card " + cardNumber(dest) + " already holds the save “" + existing + "”");
            this.dest = dest; this.existing = existing; this.incoming = incoming; this.otherOk = otherOk;
        }
        String question(boolean es) {
            // no line break inside "tarjeta 1" (a no-break space)
            return es ? "Ya hay una partida «" + existing + "» en la tarjeta " + cardNumber(dest) + ". ¿Reemplazarla?\n\nPartida importada: «" + incoming + "»"
                      : "Memory card " + cardNumber(dest) + " already holds the save “" + existing + "”. Replace it?\n\nImported save: “" + incoming + "”";
        }
    }

    /** raw into the card file dest.  A single save whose name is already on the card throws Conflict unless
     *  overwrite (then the old save is removed and its blocks are reused first; any block count).  The card is copied
     *  into backup/ before it is written; nothing is written when the import fails. */
    static Result importBytes(byte[] raw, File dest, boolean es, boolean overwrite) throws Fail, IOException {
        return importBytes(raw, dest, es, overwrite, false);
    }
    /** the Conflict importing raw into dest would raise (nothing is written), or null */
    static Conflict conflict(byte[] raw, File dest, boolean es) throws IOException {
        try { importBytes(raw, dest, es, false, true); return null; }
        catch (Conflict c) { return c; }
        catch (Fail f) { return null; }
    }
    /** dry: only the checks (returns null instead of writing) */
    private static Result importBytes(byte[] raw, File dest, boolean es, boolean overwrite, boolean dry) throws Fail, IOException {
        Result r = new Result();
        byte[] card = asCard(raw);
        if (card != null) {
            if (dry) return null;
            backup(dest);
            write(dest, card);
            r.lines.add(es ? "Tarjeta completa importada en la tarjeta " + cardNumber(dest) + " (se guardó una copia de la anterior)"
                           : "Whole card imported as memory card " + cardNumber(dest) + " (a copy of the old one was kept)");
            List<String> l = list(card);
            boolean any = false;
            for (String s : l) { r.lines.add(s); any |= isTombi(s); }
            if (l.isEmpty()) r.lines.add(es ? "(tarjeta vacía)" : "(empty card)");
            else if (!any) r.warn = es ? "La tarjeta no contiene partidas de Tombi!/Tomba!" : "The card holds no Tombi!/Tomba! saves";
            return r;
        }
        byte[] frame, data;
        if (at(raw, 0, "\0VSP") && raw.length > 0x84) {
            data = slice(raw, 0x84, raw.length - 0x84);
            if (!at(data, 0, "SC") || data.length % BLOCK != 0) throw new Fail(es ? "Archivo .psv no reconocido" : "Unrecognised .psv file");
            frame = new byte[FRAME];
            frame[0] = 0x51;
            for (int i = 0; i < 20 && raw[0x64 + i] != 0; i++) frame[10 + i] = raw[0x64 + i];
        } else if (raw.length > FRAME && u8(raw, 0) == 0x51 && (raw.length - FRAME) % BLOCK == 0 && at(raw, FRAME, "SC")) {
            frame = slice(raw, 0, FRAME);
            data = slice(raw, FRAME, raw.length - FRAME);
        } else if (raw.length == CARD) {
            throw new Fail(es ? "La imagen de tarjeta no empieza por \"MC\" (no formateada o dañada)" : "The card image does not start with \"MC\" (unformatted or damaged)");
        } else {
            throw new Fail(es ? "Formato no reconocido. Se aceptan .mcd/.mcr/.mc/.srm/.ddf/.ps/.bin, .gme, .vgs/.mem, .mcs/.ps1 y .psv"
                              : "Unknown format. Accepted: .mcd/.mcr/.mc/.srm/.ddf/.ps/.bin, .gme, .vgs/.mem, .mcs/.ps1 and .psv");
        }
        String nm = name(frame, 0);
        if (!isTombi(nm)) r.warn = es ? "La partida (" + nm + ") no parece de Tombi!/Tomba!" : "The save (" + nm + ") does not look like Tombi!/Tomba!";
        int n = data.length / BLOCK;
        byte[] c = readCard(dest);
        int old = find(c, nm);
        String replaced = null;
        List<Integer> prefer = new ArrayList<>();
        if (old >= 0) {
            replaced = info(nm, c, old * BLOCK, es);
            if (!overwrite) {
                File o = otherCard(dest);
                boolean otherOk = false;
                if (!o.getName().equals(dest.getName())) {
                    byte[] oc = readCard(o);
                    otherOk = find(oc, nm) < 0 && free(oc).size() >= n;
                }
                throw new Conflict(dest, replaced, info(nm, data, 0, es), otherOk, es);
            }
            prefer = delete(c, old);          // in memory: the file is written only once the new save fits
        }
        if (dry) return null;
        List<Integer> fr = free(c), blocks = new ArrayList<>();
        for (int b : prefer) if (fr.contains(b)) blocks.add(b);
        for (int b : fr) if (!blocks.contains(b)) blocks.add(b);
        if (blocks.size() < n)
            throw new Fail(es ? "No hay sitio en la tarjeta " + cardNumber(dest) + ": hacen falta " + n + " bloques libres y hay " + blocks.size()
                              : "No room on memory card " + cardNumber(dest) + ": " + n + " free blocks needed, " + blocks.size() + " available");
        for (int k = 0; k < n; k++) {
            int b = blocks.get(k), o = b * FRAME;
            byte[] f = k == 0 ? frame.clone() : new byte[FRAME];
            f[0] = (byte) (k == 0 ? 0x51 : k == n - 1 ? 0x53 : 0x52);
            p32(f, 4, k == 0 ? n * BLOCK : 0);
            p16(f, 8, k == n - 1 ? 0xFFFF : blocks.get(k + 1) - 1);
            System.arraycopy(f, 0, c, o, FRAME);
            fixFrame(c, o);
            System.arraycopy(data, k * BLOCK, c, b * BLOCK, BLOCK);
        }
        backup(dest);
        write(dest, c);
        r.lines.add((es ? "Partida importada en la tarjeta " + cardNumber(dest) + ", bloque " : "Save imported into memory card " + cardNumber(dest) + ", block ")
                + blocks.get(0) + ": " + info(nm, data, 0, es));
        if (replaced != null)
            r.lines.add(es ? "Sustituye a «" + replaced + "» (se guardó una copia de la tarjeta)"
                           : "Replaces “" + replaced + "” (a copy of the card was kept)");
        return r;
    }
    /** the card's bytes for export (a formatted blank card if it does not exist yet) */
    static byte[] exportCard(File src) throws IOException, Fail {
        if (!src.exists()) return blankCard();
        byte[] c = readFile(src);
        if (c.length != CARD || !at(c, 0, "MC")) throw new Fail("not a valid 128 KB card");
        return c;
    }
}

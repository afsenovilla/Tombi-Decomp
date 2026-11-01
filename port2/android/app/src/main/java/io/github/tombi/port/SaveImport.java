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
 *  Mirrors tools/save_import.py: whole cards (raw .mcd/.mcr/..., DexDrive .gme, .vgs/.mem) replace the card (the old
 *  one kept as .bak); single saves (.mcs/.ps1/.mcb, PS3 .psv) go into free blocks with proper directory frames. */
final class SaveImport {
    static final int CARD = 0x20000, BLOCK = 0x2000, FRAME = 0x80;
    static final String[] TOMBI = {"BESCES-01330", "BESCES-01331", "BASCUS-94236", "BISLPS-01144"};

    static final class Fail extends Exception { Fail(String m) { super(m); } }

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

    static void backup(File f) throws IOException {
        if (!f.exists()) return;
        File b = new File(f.getPath() + ".bak");
        if (b.exists()) b = new File(f.getPath() + "." + new SimpleDateFormat("yyyyMMdd-HHmmss", Locale.ROOT).format(new Date()) + ".bak");
        write(b, readFile(f));
    }
    static byte[] readFile(File f) throws IOException { FileInputStream in = new FileInputStream(f); try { return readAll(in, 1 << 20); } finally { in.close(); } }
    static void write(File f, byte[] d) throws IOException { FileOutputStream o = new FileOutputStream(f); try { o.write(d); } finally { o.close(); } }

    static Result importBytes(byte[] raw, File dest, boolean es) throws Fail, IOException {
        Result r = new Result();
        byte[] card = asCard(raw);
        if (card != null) {
            backup(dest);
            write(dest, card);
            r.lines.add(es ? "Tarjeta completa importada (la anterior se guardó como .bak)" : "Whole card imported (the old one kept as .bak)");
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
        byte[] c = dest.exists() ? readFile(dest) : null;
        if (c == null || c.length != CARD || !at(c, 0, "MC")) c = blankCard();
        for (int i = 1; i < 16; i++)
            if (u8(c, i * FRAME) == 0x51 && name(c, i * FRAME).equals(nm))
                throw new Fail(es ? "La tarjeta ya tiene una partida con ese nombre (" + nm + ")" : "The card already has a save named " + nm);
        int n = data.length / BLOCK;
        List<Integer> free = new ArrayList<>();
        for (int i = 1; i < 16; i++) { int t = u8(c, i * FRAME); if (t == 0 || (t >= 0xA0 && t <= 0xA3)) free.add(i); }
        if (free.size() < n)
            throw new Fail(es ? "No hay sitio en la tarjeta: hacen falta " + n + " bloques libres y hay " + free.size()
                              : "No room on the card: " + n + " free blocks needed, " + free.size() + " available");
        for (int k = 0; k < n; k++) {
            int b = free.get(k), o = b * FRAME;
            byte[] f = k == 0 ? frame.clone() : new byte[FRAME];
            f[0] = (byte) (k == 0 ? 0x51 : k == n - 1 ? 0x53 : 0x52);
            if (k == 0) p32(f, 4, n * BLOCK);
            p16(f, 8, k == n - 1 ? 0xFFFF : free.get(k + 1) - 1);
            System.arraycopy(f, 0, c, o, FRAME);
            fixFrame(c, o);
            System.arraycopy(data, k * BLOCK, c, b * BLOCK, BLOCK);
        }
        backup(dest);
        write(dest, c);
        r.lines.add((es ? "Partida importada en el bloque " : "Save imported into block ") + free.get(0) + ": " + nm + "  " + title(data, 0));
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

/* Memory card for the port: one 128 KB raw card image (the common emulator ".mcd" layout: 16 blocks of 8 KB,
 * block 0 = header + directory) kept in memory and written through to the host file tombi_mc0.mcd in --save-dir
 * (default: next to the disc image).  A missing or unreadable file starts as a formatted empty card.  Only memory
 * card 1 is emulated: port 2 ("bu10:", chan 0x10) answers as an empty slot (the BIOS "timeout" event of both card
 * event classes, every file call fails), so the game's load / save screens offer card 1 only.  A tombi_mc1.mcd left
 * by older versions (which emulated two cards) is merged into card 1 once (card_migrate).
 *
 * The game (SYSTEM/D0SPSUB2, the save/load screen) uses the BIOS memory-card file system: the low-level
 * _card_info/_card_load/_card_clear with their completion events (polled with TestEvent), then
 * open/read/write/lseek/close/erase/format and firstfile/nextfile on "bu00:" / "bu10:".  Here the card
 * calls complete at once: they raise the "I/O end" event of both the SwCARD and HwCARD event classes, and the
 * file functions work on the image directly.  The event table below is the BIOS one for every event class
 * (only the card raises any).  The BIOS file functions carry the prefix port_bu_ here (gen2.py renames the
 * game's calls), so the host libc's open/read/write stay the host's. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

/* ================================================================== BIOS events (OpenEvent/TestEvent) */
#define NEV 32
static struct { u32 desc, spec; int used, pending; } events[NEV];
extern void *port_vsync_event_fn;          /* psyq.c: the game's vblankHandler (OpenEvent(RCntCNT3)) */
int OpenEvent(u32 desc, int spec, int mode, void *func) {
    if (port_verbose) port_printf("[port] OpenEvent(%08x, %x, %x, %p)\n", desc, spec, mode, func);
    if (desc == 0xF2000003u && func) port_vsync_event_fn = func;      /* RCntCNT3 = vertical blank */
    for (int i = 0; i < NEV; i++)
        if (!events[i].used) { events[i].used = 1; events[i].desc = desc; events[i].spec = spec; events[i].pending = 0; return 0xF1000000 | i; }
    return 0xF1000000 | (desc & 0xFF);
}
int CloseEvent(int ev) { int i = ev & 0xFF; if ((ev & 0xFFFFFF00u) == 0xF1000000u && i < NEV) events[i].used = 0; return 1; }
int TestEvent(int ev) {
    int i = ev & 0xFF;
    if ((ev & 0xFFFFFF00u) != 0xF1000000u || i >= NEV || !events[i].used || !events[i].pending) return 0;
    events[i].pending = 0;
    return 1;
}
void DeliverEvent(u32 desc, u32 spec) {
    for (int i = 0; i < NEV; i++) if (events[i].used && events[i].desc == desc && events[i].spec == spec) events[i].pending = 1;
}
void UnDeliverEvent(u32 desc, u32 spec) {
    for (int i = 0; i < NEV; i++) if (events[i].used && events[i].desc == desc && events[i].spec == spec) events[i].pending = 0;
}

/* ================================================================== the card images */
#define CARD_SIZE 0x20000
#define BLOCK 0x2000
#define FRAME 0x80
static u8 cards[2][CARD_SIZE];
static int card_loaded[2];
const char *port_save_dir;                 /* --save-dir; 0 = directory of the disc image */
static char card_path[2][1024];
static const char *disc_dir_from(const char *disc, char *buf, int n) {
    int len = 0; while (disc[len]) len++;
    while (len > 0 && disc[len - 1] != '/' && disc[len - 1] != '\\') len--;      /* (C:\... paths on Windows) */
    if (len >= n) len = n - 1;
    memcpy(buf, disc, len); buf[len] = 0;
    return buf;
}
/* a port file next to the memory cards (--save-dir, else as the cards): the second screen's hidden events */
const char *port_save_file(const char *base, char *out, int n) {
    char dir[1024]; int o = 0;
    const char *d = port_save_dir ? port_save_dir : disc_dir_from(port_disc_path ? port_disc_path : port_data_dir ? port_data_dir : "", dir, sizeof dir);
    for (int i = 0; d[i] && o < n - 2; i++) out[o++] = d[i];
    if (o && out[o - 1] != '/' && out[o - 1] != '\\' && o < n - 1) out[o++] = '/';
    for (int i = 0; base[i] && o < n - 1; i++) out[o++] = base[i];
    out[o] = 0;
    return out;
}
static u8 frame_sum(const u8 *f) { u8 x = 0; for (int i = 0; i < FRAME - 1; i++) x ^= f[i]; return x; }
static void format_card(u8 *c) {
    memset(c, 0, CARD_SIZE);
    c[0] = 'M'; c[1] = 'C'; c[FRAME - 1] = frame_sum(c);
    for (int i = 1; i < 16; i++) {           /* directory: all blocks free */
        u8 *f = c + i * FRAME;
        f[0] = 0xA0; f[8] = 0xFF; f[9] = 0xFF; f[FRAME - 1] = frame_sum(f);
    }
    for (int i = 16; i < 36; i++) {          /* broken-sector list: none */
        u8 *f = c + i * FRAME;
        f[0] = f[1] = f[2] = f[3] = 0xFF; f[8] = 0xFF; f[9] = 0xFF; f[FRAME - 1] = frame_sum(f);
    }
    for (int i = 36; i < 56; i++) memset(c + i * FRAME, 0xFF, FRAME);   /* broken-sector replacement data */
    c[63 * FRAME] = 'M'; c[63 * FRAME + 1] = 'C'; c[63 * FRAME + FRAME - 1] = frame_sum(c + 63 * FRAME);
}
static void card_flush(int n) {
    FILE *f = fopen(card_path[n], "wb");
    if (!f) { port_printf("[port] cannot write memory card %s\n", card_path[n]); return; }
    fwrite(cards[n], 1, CARD_SIZE, f);
    fclose(f);
}
/* path of a file in the cards' folder: --save-dir, else next to the disc image, else next to the data/ folder (its
   parent: the exe's folder or the cwd) */
static void card_dir_file(const char *base, char *out, int n) {
    char dir[1024];
    const char *d = port_save_dir ? port_save_dir : disc_dir_from(port_disc_path ? port_disc_path : port_data_dir, dir, sizeof dir);
    int o = 0;
    for (int i = 0; d[i] && o < n - 40; i++) out[o++] = d[i];
    if (o && out[o - 1] != '/' && out[o - 1] != '\\') out[o++] = '/';
    for (int i = 0; base[i] && o < n - 1; i++) out[o++] = base[i];
    out[o] = 0;
}
static void card_migrate(void);
static u8 *card(int n) {
    if (n != 0) return 0;                  /* memory card 2: no card */
    if (card_loaded[n]) return cards[n];
    card_loaded[n] = 1;
    card_dir_file("tombi_mc0.mcd", card_path[n], sizeof card_path[n]);
    FILE *f = fopen(card_path[n], "rb");
    if (f && fread(cards[n], 1, CARD_SIZE, f) == CARD_SIZE && cards[n][0] == 'M' && cards[n][1] == 'C') {
        fclose(f);
        port_printf("[port] memory card %d: %s\n", n + 1, card_path[n]);
    } else {
        if (f) fclose(f);
        format_card(cards[n]);
        port_printf("[port] memory card %d: %s not found or not a 128 KB card image; a new, formatted card\n", n + 1, card_path[n]);
        card_flush(n);
    }
    card_migrate();
    return cards[n];
}

/* "bu00:NAME" / "bu10:NAME" -> card number and the name (which may end in "*" for firstfile) */
static int parse_name(const char *s, const char **name) {
    if (!s || (s[0] | 32) != 'b' || (s[1] | 32) != 'u' || s[4] != ':') return -1;
    *name = s + 5;
    return s[2] == '1' ? 1 : 0;
}
static int name_eq(const u8 *dirname, const char *name) {
    for (int i = 0; i < 20; i++) {
        char a = (char)dirname[i], b = name[i];
        if (a != b) return 0;
        if (!a) return 1;
    }
    return 1;
}
static int name_match(const u8 *dirname, const char *pat) {       /* "*" wildcard as in the BIOS (prefix only) */
    for (int i = 0; i < 20; i++) {
        if (pat[i] == '*') return 1;
        if ((char)dirname[i] != pat[i]) return 0;
        if (!pat[i]) return 1;
    }
    return 1;
}
#define DIR(c, b) ((c) + (b) * FRAME)      /* directory frame of block b (1..15) */
static int find_file(const u8 *c, const char *name) {
    for (int b = 1; b < 16; b++) if (DIR(c, b)[0] == 0x51 && name_eq(DIR(c, b) + 0x0A, name)) return b;
    return -1;
}
static u32 dir_size(const u8 *f) { return f[4] | (f[5] << 8) | (f[6] << 16) | ((u32)f[7] << 24); }
static int dir_next(const u8 *f) { int n = f[8] | (f[9] << 8); return n == 0xFFFF ? -1 : n + 1; }

/* ================================================================== tombi_mc1.mcd -> card 1 (once)
   Older versions emulated two cards.  When tombi_mc1.mcd holds Tombi! saves, they are copied into free blocks of
   card 1 (a save whose name card 1 already has goes in as the first free slot: file name and the title's fullwidth
   slot digits, as tools/save_import.py renumber; a save identical to the one on card 1 is skipped).  All or nothing:
   when they do not all fit, both files stay as they are and a warning is logged (again at each start).  After a
   merge both cards were first copied to backup/tombi_mcN-YYYYMMDD-HHMMSS.mcd, card 1 is written and tombi_mc1.mcd
   is renamed tombi_mc1.mcd.old. */
extern void port_mkdir(const char *dir);   /* shims/gpu.c */
static int tombi_name(const u8 *nm) {
    static const char *prod[] = { "BESCES-01330", "BESCES-01331", "BASCUS-94236", "BISLPS-01144" };
    char s[21]; memcpy(s, nm, 20); s[20] = 0;
    for (int i = 0; i < 4; i++) if (!strncmp(s, prod[i], 12)) return 1;
    return strstr(s, "TOMBA") != 0;
}
static int block_free(const u8 *c, int b) { u8 t = DIR(c, b)[0]; return t == 0 || (t >= 0xA0 && t <= 0xA3); }
static int save_chain(const u8 *c, int b, int *out) {
    int n = 0;
    while (b >= 1 && b <= 15 && n < 15) {
        for (int i = 0; i < n; i++) if (out[i] == b) return n;
        out[n++] = b; b = dir_next(DIR(c, b));
    }
    return n;
}
static int card_backup(const char *path, const char *stem) {
    char dir[1024], out[1100]; u8 *buf = (u8 *)malloc(CARD_SIZE);
    FILE *f = fopen(path, "rb");
    int n = f && buf ? (int)fread(buf, 1, CARD_SIZE, f) : 0;
    if (f) fclose(f);
    if (n <= 0) { free(buf); return 0; }
    card_dir_file("backup", dir, sizeof dir);
    port_mkdir(dir);
    time_t t = time(0); char stamp[32];
    strftime(stamp, sizeof stamp, "%Y%m%d-%H%M%S", localtime(&t));
    snprintf(out, sizeof out, "%s/%s-%s.mcd", dir, stem, stamp);
    for (int k = 2; (f = fopen(out, "rb")) != 0 && k < 100; k++) { fclose(f); snprintf(out, sizeof out, "%s/%s-%s-%d.mcd", dir, stem, stamp, k); }
    f = fopen(out, "wb");
    int ok = f && fwrite(buf, 1, n, f) == (size_t)n;
    if (f) fclose(f);
    free(buf);
    if (!ok) port_printf("[port] memory card: cannot write the backup %s\n", out);
    return ok;
}
static void card_migrate(void) {
    char p1[1024], old[1100];
    card_dir_file("tombi_mc1.mcd", p1, sizeof p1);
    FILE *f = fopen(p1, "rb");
    if (!f) return;
    u8 *c1 = (u8 *)malloc(CARD_SIZE), *m = (u8 *)malloc(CARD_SIZE);
    int ok = c1 && m && fread(c1, 1, CARD_SIZE, f) == CARD_SIZE && c1[0] == 'M' && c1[1] == 'C';
    fclose(f);
    int moved = 0, skipped = 0, fail = 0;
    if (ok) {
        memcpy(m, cards[0], CARD_SIZE);
        for (int b = 1; b < 16 && !fail; b++) {
            const u8 *df = DIR(c1, b);
            if (df[0] != 0x51 || !tombi_name(df + 0x0A)) continue;
            int ch[15], n = save_chain(c1, b, ch);
            char name[21]; memcpy(name, df + 0x0A, 20); name[20] = 0;
            int len = (int)strlen(name), have = find_file(m, name);
            if (have >= 0) {                     /* the same save already on card 1? */
                int ch0[15], n0 = save_chain(m, have, ch0), same = n0 == n;
                for (int k = 0; same && k < n; k++) same = !memcmp(m + ch0[k] * BLOCK, c1 + ch[k] * BLOCK, BLOCK);
                if (same) { skipped++; continue; }
                int nn = -1;
                if (len >= 2 && name[len - 2] >= '0' && name[len - 2] <= '9' && name[len - 1] >= '0' && name[len - 1] <= '9')
                    for (int k = 0; k < 15 && nn < 0; k++) { name[len - 2] = '0' + k / 10; name[len - 1] = '0' + k % 10; if (find_file(m, name) < 0) nn = k; }
                if (nn < 0) { port_printf("[port] memory card 2: no free slot on card 1 for %.20s\n", (const char *)(df + 0x0A)); fail = 1; break; }
            }
            int fb[15], nf = 0;
            for (int x = 1; x < 16 && nf < n; x++) if (block_free(m, x)) fb[nf++] = x;
            if (nf < n) { port_printf("[port] memory card 2: no room on card 1 for %s (%d blocks)\n", name, n); fail = 1; break; }
            for (int k = 0; k < n; k++) {
                u8 *t = DIR(m, fb[k]);
                if (k == 0) { memcpy(t, df, FRAME); memset(t + 0x0A, 0, 20); memcpy(t + 0x0A, name, strlen(name)); }
                else memset(t, 0, FRAME);
                t[0] = k == 0 ? 0x51 : k == n - 1 ? 0x53 : 0x52;
                u32 size = k == 0 ? (u32)n * BLOCK : 0;
                t[4] = size; t[5] = size >> 8; t[6] = size >> 16; t[7] = size >> 24;
                int nx = k == n - 1 ? 0xFFFF : fb[k + 1] - 1;
                t[8] = nx & 0xFF; t[9] = nx >> 8;
                t[FRAME - 1] = frame_sum(t);
                memcpy(m + fb[k] * BLOCK, c1 + ch[k] * BLOCK, BLOCK);
            }
            u8 *d = m + fb[0] * BLOCK;           /* the title's slot digits (save +0x16, fullwidth = 0x824F + n) */
            if (d[0x16] == 0x82 && d[0x17] >= 0x4F && d[0x17] <= 0x58 && d[0x18] == 0x82 && d[0x19] >= 0x4F && d[0x19] <= 0x58) {
                d[0x17] = 0x4F + (name[len - 2] - '0'); d[0x19] = 0x4F + (name[len - 1] - '0');
            }
            int renamed = strncmp(name, (const char *)df + 0x0A, 20) != 0;
            port_printf("[port] memory card 2: %.20s -> card 1, block %d%s%s\n", (const char *)(df + 0x0A), fb[0],
                        renamed ? " as " : "", renamed ? name : "");
            moved++;
        }
    }
    if (ok && !fail && (moved || skipped)) {
        if (card_backup(card_path[0], "tombi_mc0") && card_backup(p1, "tombi_mc1")) {
            memcpy(cards[0], m, CARD_SIZE);
            if (moved) card_flush(0);
            snprintf(old, sizeof old, "%s.old", p1);
            /* (not rename(): the game's BIOS rename stub takes the name in the port's link) */
            FILE *o = fopen(old, "wb");
            int w = o && fwrite(c1, 1, CARD_SIZE, o) == CARD_SIZE;
            if (o && fclose(o)) w = 0;
            if (!w || remove(p1)) port_printf("[port] memory card 2: merged, but %s could not be moved to %s\n", p1, old);
            port_printf("[port] memory card 2 (%s) merged into card 1: %d saves copied, %d already there; the file is now %s "
                        "(backups in backup/)\n", p1, moved, skipped, old);
        } else port_printf("[port] memory card 2: WARNING: no backup could be written; %s left as it is\n", p1);
    } else if (ok && fail)
        port_printf("[port] memory card 2: WARNING: its Tombi! saves do not fit on card 1; %s left as it is (only card 1 "
                    "is emulated: free blocks on card 1, or import its saves with the launcher)\n", p1);
    free(c1); free(m);
}

/* ================================================================== low-level card calls + their events */
static void card_done(void) {
    DeliverEvent(0xF0000011u, 4);          /* HwCARD: I/O end */
    DeliverEvent(0xF4000001u, 4);          /* SwCARD: I/O end */
}
static void card_absent(void) {           /* an empty card slot: the BIOS raises "timeout" */
    DeliverEvent(0xF0000011u, 0x0100);     /* HwCARD: timeout */
    DeliverEvent(0xF4000001u, 0x0100);     /* SwCARD: timeout */
}
static int chan_card(int chan) { return (chan >> 4) & 1; }
static void card_call(int chan) { if (card(chan_card(chan))) card_done(); else card_absent(); }
void _card_info(int chan) { card_call(chan); }
void _card_load(int chan) { card_call(chan); }
void _card_clear(int chan) { card_call(chan); }
int _card_status(int chan) { return 1; }
int _card_wait(int chan) { return 1; }
int _card_chan(void) { return 0; }
void _new_card(void) {}
int _card_format(int chan) { int n = chan_card(chan); u8 *c = card(n); if (!c) { card_absent(); return 0; } format_card(c); card_flush(n); card_done(); return 1; }
void InitCARD(int v) {}
int StartCARD(void) { return 1; }
int StopCARD2(void) { return 1; }
void _bu_init(void) {}
void _patch_card2(void) {}
void _patch_card(void) {}
int _card_read(int chan, int block, void *buf) { u8 *c = card(chan_card(chan)); if (!c) { card_absent(); return 0; } memcpy(buf, c + block * FRAME, FRAME); card_done(); return 1; }
int _card_write(int chan, int block, const void *buf) { int n = chan_card(chan); u8 *c = card(n); if (!c) { card_absent(); return 0; } memcpy(c + block * FRAME, buf, FRAME); card_flush(n); card_done(); return 1; }

/* ================================================================== BIOS file system on the cards */
#define NFD 8
static struct { int used, card, head, mode; u32 pos; } fds[NFD];
int port_bu_open(const char *path, int mode) {
    const char *name = 0; int n = parse_name(path, &name);
    u8 *c = card(n);
    if (!c) return -1;
    int head = find_file(c, name);
    if (mode & 0x200) {                                    /* create with (mode >> 16) blocks */
        int blocks = (mode >> 16) & 0xFFFF, free = 0, prev = -1, first = -1;
        if (head >= 0 || blocks < 1) return -1;
        for (int b = 1; b < 16; b++) if ((DIR(c, b)[0] & 0xF0) == 0xA0) free++;
        if (free < blocks) return -1;
        for (int b = 1; b < 16 && blocks; b++) {
            u8 *f = DIR(c, b);
            if ((f[0] & 0xF0) != 0xA0) continue;
            memset(f, 0, FRAME);
            f[0] = first < 0 ? 0x51 : 0x52;
            f[8] = 0xFF; f[9] = 0xFF;
            if (first < 0) {
                first = b;
                u32 size = (u32)((mode >> 16) & 0xFFFF) * BLOCK;
                f[4] = size; f[5] = size >> 8; f[6] = size >> 16; f[7] = size >> 24;
                for (int i = 0; i < 20 && name[i]; i++) f[0x0A + i] = name[i];
            } else { u8 *p = DIR(c, prev); p[8] = (b - 1) & 0xFF; p[9] = (b - 1) >> 8; p[FRAME - 1] = frame_sum(p); }
            f[FRAME - 1] = frame_sum(f);
            prev = b; blocks--;
        }
        if (prev != first) DIR(c, prev)[0] = 0x53;
        DIR(c, prev)[FRAME - 1] = frame_sum(DIR(c, prev));
        memset(c + first * BLOCK, 0, BLOCK);
        card_flush(n);
        head = first;
        if (port_verbose) port_printf("[port] card %d: created %s (%d blocks)\n", n + 1, name, (mode >> 16) & 0xFFFF);
    } else if (head < 0) return -1;
    for (int i = 0; i < NFD; i++)
        if (!fds[i].used) { fds[i].used = 1; fds[i].card = n; fds[i].head = head; fds[i].mode = mode; fds[i].pos = 0; return i; }
    return -1;
}
int port_bu_close(int fd) { if (fd < 0 || fd >= NFD || !fds[fd].used) return -1; fds[fd].used = 0; return fd; }
int port_bu_lseek(int fd, int ofs, int whence) {
    if (fd < 0 || fd >= NFD || !fds[fd].used) return -1;
    u32 size = dir_size(DIR(cards[fds[fd].card], fds[fd].head));
    u32 p = whence == 0 ? (u32)ofs : whence == 1 ? fds[fd].pos + ofs : size + ofs;
    if (p > size) return -1;
    fds[fd].pos = p;
    return (int)p;
}
/* the file's bytes [pos, pos+len) live in its chain of blocks */
static int file_xfer(int fd, u8 *buf, int len, int write_) {
    if (fd < 0 || fd >= NFD || !fds[fd].used || len < 0) return -1;
    int n = fds[fd].card; u8 *c = cards[n];
    u32 size = dir_size(DIR(c, fds[fd].head)), pos = fds[fd].pos;
    if (pos + (u32)len > size) return -1;
    int done = 0;
    while (done < len) {
        int b = fds[fd].head; u32 off = pos + done;
        while (off >= BLOCK && b > 0) { off -= BLOCK; b = dir_next(DIR(c, b)); }
        if (b <= 0 || b > 15) return -1;
        int chunk = len - done; if ((u32)chunk > BLOCK - off) chunk = BLOCK - off;
        if (write_) memcpy(c + b * BLOCK + off, buf + done, chunk); else memcpy(buf + done, c + b * BLOCK + off, chunk);
        done += chunk;
    }
    fds[fd].pos += len;
    if (write_) card_flush(n);
    return len;
}
int port_bu_read(int fd, void *buf, int len) { return file_xfer(fd, buf, len, 0); }
int port_bu_write(int fd, const void *buf, int len) { return file_xfer(fd, (u8 *)buf, len, 1); }
int port_bu_erase(const char *path) {
    const char *name = 0; int n = parse_name(path, &name);
    u8 *c = card(n);
    if (!c) return 0;
    int b = find_file(c, name);
    if (b < 0) return 0;
    while (b > 0 && b < 16) {
        u8 *f = DIR(c, b); int next = dir_next(f);
        f[0] = 0xA0 | (f[0] & 0x0F);            /* 0x51/52/53 -> 0xA1/A2/A3: deleted */
        f[FRAME - 1] = frame_sum(f);
        b = next;
    }
    card_flush(n);
    if (port_verbose) port_printf("[port] card %d: erased %s\n", n + 1, name);
    return 1;
}
int port_bu_format(const char *path) {
    const char *name = 0; int n = parse_name(path, &name);
    u8 *c = card(n);
    if (!c) return 0;
    format_card(c); card_flush(n);
    port_printf("[port] card %d: formatted\n", n + 1);
    return 1;
}
struct DIRENTRY { char name[20]; int attr; int size; struct DIRENTRY *next; int head; char system[4]; };
static struct { int card, block; char pat[21]; } dirwalk;
static struct DIRENTRY *dir_fill(struct DIRENTRY *d) {
    u8 *c = cards[dirwalk.card];
    for (; dirwalk.block < 16; dirwalk.block++) {
        u8 *f = DIR(c, dirwalk.block);
        if (f[0] != 0x51 || !name_match(f + 0x0A, dirwalk.pat)) continue;
        memset(d, 0, sizeof *d);
        memcpy(d->name, f + 0x0A, 20);
        d->attr = 0x50;                                      /* a plain file, in the BIOS's attr bits */
        d->size = (int)dir_size(f);
        d->head = dirwalk.block;
        dirwalk.block++;
        return d;
    }
    return 0;
}
struct DIRENTRY *port_bu_firstfile(const char *path, struct DIRENTRY *d) {
    const char *name = 0; int n = parse_name(path, &name);
    if (!card(n)) return 0;
    dirwalk.card = n; dirwalk.block = 1;
    int i = 0; for (; i < 20 && name[i]; i++) dirwalk.pat[i] = name[i]; dirwalk.pat[i] = 0;
    return dir_fill(d);
}
struct DIRENTRY *port_bu_nextfile(struct DIRENTRY *d) { return dir_fill(d); }
/* the game calls firstfile by its retail address name */
struct DIRENTRY *psyq_8005ba74(const char *path, struct DIRENTRY *d) __attribute__((alias("port_bu_firstfile")));

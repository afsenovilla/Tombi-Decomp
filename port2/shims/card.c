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

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

/* ================================================================== BIOS events (OpenEvent/TestEvent) */
#define NEV 32
static struct {
    u32 desc, spec;
    int used, pending;
} events[NEV];
extern void *port_vsync_event_fn;          /* psyq.c: the game's vblankHandler (OpenEvent(RCntCNT3)) */
int OpenEvent(u32 desc, int spec, int mode, void *func) {
    if (port_verbose) port_printf("[port] OpenEvent(%08x, %x, %x, %p)\n", desc, spec, mode, func);
    if (desc == 0xF2000003u && func) port_vsync_event_fn = func;      /* RCntCNT3 = vertical blank */
    for (int i = 0; i < NEV; i++)
        if (!events[i].used) {
            events[i].used = 1;
            events[i].desc = desc;
            events[i].spec = spec;
            events[i].pending = 0;
            return 0xF1000000 | i;
        }
    return 0xF1000000 | (desc & 0xFF);
}
int CloseEvent(int ev) {
    int i = ev & 0xFF;
    if ((ev & 0xFFFFFF00u) == 0xF1000000u && i < NEV) events[i].used = 0;
    return 1;
}
int TestEvent(int ev) {
    int i = ev & 0xFF;
    if ((ev & 0xFFFFFF00u) != 0xF1000000u || i >= NEV || !events[i].used || !events[i].pending) return 0;
    events[i].pending = 0;
    return 1;
}
void DeliverEvent(u32 desc, u32 spec) {
    for (int i = 0; i < NEV; i++)
        if (events[i].used && events[i].desc == desc && events[i].spec == spec) events[i].pending = 1;
}
void UnDeliverEvent(u32 desc, u32 spec) {
    for (int i = 0; i < NEV; i++)
        if (events[i].used && events[i].desc == desc && events[i].spec == spec) events[i].pending = 0;
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
    int len = 0;
    while (disc[len]) len++;
    while (len > 0 && disc[len - 1] != '/' && disc[len - 1] != '\\') len--;      /* (C:\... paths on Windows) */
    if (len >= n) len = n - 1;
    memcpy(buf, disc, len);
    buf[len] = 0;
    return buf;
}
/* a port file next to the memory cards (--save-dir, else as the cards): the second screen's hidden events */
const char *port_save_file(const char *base, char *out, int n) {
    char dir[1024];
    int out_len = 0;
    const char *folder = port_save_dir ? port_save_dir
                                       : disc_dir_from(port_disc_path ? port_disc_path
                                                                      : port_data_dir ? port_data_dir : "",
                                                       dir, sizeof dir);
    for (int i = 0; folder[i] && out_len < n - 2; i++) out[out_len++] = folder[i];
    if (out_len && out[out_len - 1] != '/' && out[out_len - 1] != '\\' && out_len < n - 1) out[out_len++] = '/';
    for (int i = 0; base[i] && out_len < n - 1; i++) out[out_len++] = base[i];
    out[out_len] = 0;
    return out;
}
/* a directory frame's checksum: the XOR of its first 127 bytes */
static u8 frame_sum(const u8 *f) {
    u8 sum = 0;
    for (int i = 0; i < FRAME - 1; i++) sum ^= f[i];
    return sum;
}
static void format_card(u8 *c) {
    memset(c, 0, CARD_SIZE);
    c[0] = 'M';
    c[1] = 'C';
    c[FRAME - 1] = frame_sum(c);
    for (int i = 1; i < 16; i++) {           /* directory: all blocks free */
        u8 *f = c + i * FRAME;
        f[0] = 0xA0;
        f[8] = 0xFF;
        f[9] = 0xFF;
        f[FRAME - 1] = frame_sum(f);
    }
    for (int i = 16; i < 36; i++) {          /* broken-sector list: none */
        u8 *f = c + i * FRAME;
        f[0] = f[1] = f[2] = f[3] = 0xFF;
        f[8] = 0xFF;
        f[9] = 0xFF;
        f[FRAME - 1] = frame_sum(f);
    }
    for (int i = 36; i < 56; i++) memset(c + i * FRAME, 0xFF, FRAME);   /* broken-sector replacement data */
    c[63 * FRAME] = 'M';
    c[63 * FRAME + 1] = 'C';
    c[63 * FRAME + FRAME - 1] = frame_sum(c + 63 * FRAME);
}
static void card_flush(int n) {
    FILE *f = fopen(card_path[n], "wb");
    if (!f) {
        port_printf("[port] cannot write memory card %s\n", card_path[n]);
        return;
    }
    fwrite(cards[n], 1, CARD_SIZE, f);
    fclose(f);
}
/* path of a file in the cards' folder: --save-dir, else next to the disc image, else next to the data/ folder (its
   parent: the exe's folder or the cwd) */
static void card_dir_file(const char *base, char *out, int n) {
    char dir[1024];
    const char *folder = port_save_dir ? port_save_dir
                                       : disc_dir_from(port_disc_path ? port_disc_path : port_data_dir,
                                                       dir, sizeof dir);
    int out_len = 0;
    for (int i = 0; folder[i] && out_len < n - 40; i++) out[out_len++] = folder[i];
    if (out_len && out[out_len - 1] != '/' && out[out_len - 1] != '\\') out[out_len++] = '/';
    for (int i = 0; base[i] && out_len < n - 1; i++) out[out_len++] = base[i];
    out[out_len] = 0;
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
        port_printf("[port] memory card %d: %s not found or not a 128 KB card image; a new, formatted card\n", n + 1,
                    card_path[n]);
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
        char a = (char)dirname[i],
             b = name[i];
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
static int dir_next(const u8 *f) {
    int n = f[8] | (f[9] << 8);
    return n == 0xFFFF ? -1 : n + 1;
}

/* ================================================================== tombi_mc1.mcd -> card 1 (once)
   Older versions emulated two cards.  When tombi_mc1.mcd holds Tombi! saves, they are copied into free blocks of
   card 1 (a save whose name card 1 already has goes in as the first free slot: file name and the title's fullwidth
   slot digits, as tools/save_import.py renumber; a save identical to the one on card 1 is skipped).  All or nothing:
   when they do not all fit, both files stay as they are and a warning is logged (again at each start).  After a
   merge both cards were first copied to backup/tombi_mcN-YYYYMMDD-HHMMSS.mcd, card 1 is written and tombi_mc1.mcd
   is renamed tombi_mc1.mcd.old. */
extern void port_mkdir(const char *dir);   /* shims/gpu.c */
/* a save of Tombi! (any region) by its directory file name */
static int tombi_name(const u8 *nm) {
    static const char *product_ids[] = { "BESCES-01330", "BESCES-01331", "BASCUS-94236", "BISLPS-01144" };
    char fname[21];
    memcpy(fname, nm, 20);
    fname[20] = 0;
    for (int i = 0; i < 4; i++) if (!strncmp(fname, product_ids[i], 12)) return 1;
    return strstr(fname, "TOMBA") != 0;
}
static int block_free(const u8 *c, int b) {
    u8 dir_type = DIR(c, b)[0];
    return dir_type == 0 || (dir_type >= 0xA0 && dir_type <= 0xA3);
}
/* the blocks of the save that starts at block b, in order (at most 15; stops at a loop) */
static int save_chain(const u8 *c, int b, int *out) {
    int count = 0;
    while (b >= 1 && b <= 15 && count < 15) {
        for (int i = 0; i < count; i++) if (out[i] == b) return count;
        out[count++] = b;
        b = dir_next(DIR(c, b));
    }
    return count;
}
/* copies a card file to backup/STEM-YYYYMMDD-HHMMSS.mcd; 0 when the file is missing or the copy fails */
static int card_backup(const char *path, const char *stem) {
    char dir[1024], out[1100];
    u8 *buf = (u8 *)malloc(CARD_SIZE);
    FILE *f = fopen(path, "rb");
    int got_len = f && buf ? (int)fread(buf, 1, CARD_SIZE, f) : 0;
    if (f) fclose(f);
    if (got_len <= 0) {
        free(buf);
        return 0;
    }
    card_dir_file("backup", dir, sizeof dir);
    port_mkdir(dir);
    time_t clock_now = time(0);
    char stamp[32];
    strftime(stamp, sizeof stamp, "%Y%m%d-%H%M%S", localtime(&clock_now));
    snprintf(out, sizeof out, "%s/%s-%s.mcd", dir, stem, stamp);
    for (int k = 2; (f = fopen(out, "rb")) != 0 && k < 100; k++) {
        fclose(f);
        snprintf(out, sizeof out, "%s/%s-%s-%d.mcd", dir, stem, stamp, k);
    }
    f = fopen(out, "wb");
    int ok = f && fwrite(buf, 1, got_len, f) == (size_t)got_len;
    if (f) fclose(f);
    free(buf);
    if (!ok) port_printf("[port] memory card: cannot write the backup %s\n", out);
    return ok;
}
static void card_migrate(void) {
    char card2_path[1024], old_path[1100];
    card_dir_file("tombi_mc1.mcd", card2_path, sizeof card2_path);
    FILE *f = fopen(card2_path, "rb");
    if (!f) return;
    u8 *card2 = (u8 *)malloc(CARD_SIZE),
       *card1_copy = (u8 *)malloc(CARD_SIZE);
    int ok = card2 && card1_copy && fread(card2, 1, CARD_SIZE, f) == CARD_SIZE && card2[0] == 'M' && card2[1] == 'C';
    fclose(f);
    int moved = 0, skipped = 0, fail = 0;
    if (ok) {
        memcpy(card1_copy, cards[0], CARD_SIZE);
        for (int b = 1; b < 16 && !fail; b++) {
            const u8 *dir_frame = DIR(card2, b);
            if (dir_frame[0] != 0x51 || !tombi_name(dir_frame + 0x0A)) continue;
            int chain[15],
                chain_len = save_chain(card2, b, chain);
            char name[21];
            memcpy(name, dir_frame + 0x0A, 20);
            name[20] = 0;
            int len = (int)strlen(name),
                have = find_file(card1_copy, name);
            if (have >= 0) {                     /* the same save already on card 1? */
                int have_chain[15],
                    have_len = save_chain(card1_copy, have, have_chain),
                    same = have_len == chain_len;
                for (int k = 0; same && k < chain_len; k++)
                    same = !memcmp(card1_copy + have_chain[k] * BLOCK, card2 + chain[k] * BLOCK, BLOCK);
                if (same) {
                    skipped++;
                    continue;
                }
                int new_slot = -1;
                /* a name ending in two digits: the first free number 00..14 */
                if (len >= 2 && name[len - 2] >= '0' && name[len - 2] <= '9'
                    && name[len - 1] >= '0' && name[len - 1] <= '9')
                    for (int k = 0; k < 15 && new_slot < 0; k++) {
                        name[len - 2] = '0' + k / 10;
                        name[len - 1] = '0' + k % 10;
                        if (find_file(card1_copy, name) < 0) new_slot = k;
                    }
                if (new_slot < 0) {
                    port_printf("[port] memory card 2: no free slot on card 1 for %.20s\n",
                                (const char *)(dir_frame + 0x0A));
                    fail = 1;
                    break;
                }
            }
            /* copy the chain into free blocks of card 1, relinking the directory frames */
            int free_blk[15], free_found = 0;
            for (int x = 1; x < 16 && free_found < chain_len; x++)
                if (block_free(card1_copy, x)) free_blk[free_found++] = x;
            if (free_found < chain_len) {
                port_printf("[port] memory card 2: no room on card 1 for %s (%d blocks)\n", name, chain_len);
                fail = 1;
                break;
            }
            for (int k = 0; k < chain_len; k++) {
                u8 *dir_ent = DIR(card1_copy, free_blk[k]);
                if (k == 0) {
                    memcpy(dir_ent, dir_frame, FRAME);
                    memset(dir_ent + 0x0A, 0, 20);
                    memcpy(dir_ent + 0x0A, name, strlen(name));
                } else memset(dir_ent, 0, FRAME);
                dir_ent[0] = k == 0 ? 0x51 : k == chain_len - 1 ? 0x53 : 0x52;
                u32 size = k == 0 ? (u32)chain_len * BLOCK : 0;
                dir_ent[4] = size;
                dir_ent[5] = size >> 8;
                dir_ent[6] = size >> 16;
                dir_ent[7] = size >> 24;
                int next_link = k == chain_len - 1 ? 0xFFFF : free_blk[k + 1] - 1;
                dir_ent[8] = next_link & 0xFF;
                dir_ent[9] = next_link >> 8;
                dir_ent[FRAME - 1] = frame_sum(dir_ent);
                memcpy(card1_copy + free_blk[k] * BLOCK, card2 + chain[k] * BLOCK, BLOCK);
            }
            /* the title's slot digits (save +0x16, fullwidth = 0x824F + n) */
            u8 *save_data = card1_copy + free_blk[0] * BLOCK;
            if (save_data[0x16] == 0x82 && save_data[0x17] >= 0x4F && save_data[0x17] <= 0x58
                && save_data[0x18] == 0x82 && save_data[0x19] >= 0x4F && save_data[0x19] <= 0x58) {
                save_data[0x17] = 0x4F + (name[len - 2] - '0');
                save_data[0x19] = 0x4F + (name[len - 1] - '0');
            }
            int renamed = strncmp(name, (const char *)dir_frame + 0x0A, 20) != 0;
            port_printf("[port] memory card 2: %.20s -> card 1, block %d%s%s\n", (const char *)(dir_frame + 0x0A),
                        free_blk[0], renamed ? " as " : "", renamed ? name : "");
            moved++;
        }
    }
    if (ok && !fail && (moved || skipped)) {
        if (card_backup(card_path[0], "tombi_mc0") && card_backup(card2_path, "tombi_mc1")) {
            memcpy(cards[0], card1_copy, CARD_SIZE);
            if (moved) card_flush(0);
            snprintf(old_path, sizeof old_path, "%s.old", card2_path);
            /* (not rename(): the game's BIOS rename stub takes the name in the port's link) */
            FILE *old_file = fopen(old_path, "wb");
            int old_ok = old_file && fwrite(card2, 1, CARD_SIZE, old_file) == CARD_SIZE;
            if (old_file && fclose(old_file)) old_ok = 0;
            if (!old_ok || remove(card2_path))
                port_printf("[port] memory card 2: merged, but %s could not be moved to %s\n", card2_path, old_path);
            port_printf("[port] memory card 2 (%s) merged into card 1: %d saves copied, %d already there; "
                        "the file is now %s (backups in backup/)\n", card2_path, moved, skipped, old_path);
        } else port_printf("[port] memory card 2: WARNING: no backup could be written; %s left as it is\n", card2_path);
    } else if (ok && fail)
        port_printf("[port] memory card 2: WARNING: its Tombi! saves do not fit on card 1; %s left as it is "
                    "(only card 1 is emulated: free blocks on card 1, or import its saves with the launcher)\n",
                    card2_path);
    free(card2);
    free(card1_copy);
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
static void card_call(int chan) {
    if (card(chan_card(chan))) card_done();
    else card_absent();
}
void _card_info(int chan) { card_call(chan); }
void _card_load(int chan) { card_call(chan); }
void _card_clear(int chan) { card_call(chan); }
int _card_status(int chan) { return 1; }
int _card_wait(int chan) { return 1; }
int _card_chan(void) { return 0; }
void _new_card(void) {}
int _card_format(int chan) {
    int n = chan_card(chan);
    u8 *c = card(n);
    if (!c) {
        card_absent();
        return 0;
    }
    format_card(c);
    card_flush(n);
    card_done();
    return 1;
}
void InitCARD(int v) {}
int StartCARD(void) { return 1; }
int StopCARD2(void) { return 1; }
void _bu_init(void) {}
void _patch_card2(void) {}
void _patch_card(void) {}
int _card_read(int chan, int block, void *buf) {
    u8 *c = card(chan_card(chan));
    if (!c) {
        card_absent();
        return 0;
    }
    memcpy(buf, c + block * FRAME, FRAME);
    card_done();
    return 1;
}
int _card_write(int chan, int block, const void *buf) {
    int n = chan_card(chan);
    u8 *c = card(n);
    if (!c) {
        card_absent();
        return 0;
    }
    memcpy(c + block * FRAME, buf, FRAME);
    card_flush(n);
    card_done();
    return 1;
}

/* ================================================================== BIOS file system on the cards */
#define NFD 8
static struct {
    int used, card, head, mode;
    u32 pos;
} fds[NFD];
int port_bu_open(const char *path, int mode) {
    const char *name = 0;
    int n = parse_name(path, &name);
    u8 *c = card(n);
    if (!c) return -1;
    int head = find_file(c, name);
    if (mode & 0x200) {                                    /* create with (mode >> 16) blocks */
        int blocks = (mode >> 16) & 0xFFFF,
            free_count = 0,
            prev = -1,
            first = -1;
        if (head >= 0 || blocks < 1) return -1;
        for (int b = 1; b < 16; b++) if ((DIR(c, b)[0] & 0xF0) == 0xA0) free_count++;
        if (free_count < blocks) return -1;
        for (int b = 1; b < 16 && blocks; b++) {
            u8 *f = DIR(c, b);
            if ((f[0] & 0xF0) != 0xA0) continue;
            memset(f, 0, FRAME);
            f[0] = first < 0 ? 0x51 : 0x52;
            f[8] = 0xFF;
            f[9] = 0xFF;
            if (first < 0) {
                first = b;
                u32 size = (u32)((mode >> 16) & 0xFFFF) * BLOCK;
                f[4] = size;
                f[5] = size >> 8;
                f[6] = size >> 16;
                f[7] = size >> 24;
                for (int i = 0; i < 20 && name[i]; i++) f[0x0A + i] = name[i];
            } else {
                u8 *prev_dir = DIR(c, prev);
                prev_dir[8] = (b - 1) & 0xFF;
                prev_dir[9] = (b - 1) >> 8;
                prev_dir[FRAME - 1] = frame_sum(prev_dir);
            }
            f[FRAME - 1] = frame_sum(f);
            prev = b;
            blocks--;
        }
        if (prev != first) DIR(c, prev)[0] = 0x53;
        DIR(c, prev)[FRAME - 1] = frame_sum(DIR(c, prev));
        memset(c + first * BLOCK, 0, BLOCK);
        card_flush(n);
        head = first;
        if (port_verbose) port_printf("[port] card %d: created %s (%d blocks)\n", n + 1, name, (mode >> 16) & 0xFFFF);
    } else if (head < 0) return -1;
    for (int i = 0; i < NFD; i++)
        if (!fds[i].used) {
            fds[i].used = 1;
            fds[i].card = n;
            fds[i].head = head;
            fds[i].mode = mode;
            fds[i].pos = 0;
            return i;
        }
    return -1;
}
int port_bu_close(int fd) {
    if (fd < 0 || fd >= NFD || !fds[fd].used) return -1;
    fds[fd].used = 0;
    return fd;
}
int port_bu_lseek(int fd, int ofs, int whence) {
    if (fd < 0 || fd >= NFD || !fds[fd].used) return -1;
    u32 size = dir_size(DIR(cards[fds[fd].card], fds[fd].head));
    u32 new_pos = whence == 0 ? (u32)ofs : whence == 1 ? fds[fd].pos + ofs : size + ofs;
    if (new_pos > size) return -1;
    fds[fd].pos = new_pos;
    return (int)new_pos;
}
/* the file's bytes [pos, pos+len) live in its chain of blocks */
static int file_xfer(int fd, u8 *buf, int len, int write_) {
    if (fd < 0 || fd >= NFD || !fds[fd].used || len < 0) return -1;
    int n = fds[fd].card;
    u8 *c = cards[n];
    u32 size = dir_size(DIR(c, fds[fd].head)),
        pos = fds[fd].pos;
    if (pos + (u32)len > size) return -1;
    int done = 0;
    while (done < len) {
        int blk = fds[fd].head;
        u32 off = pos + done;
        while (off >= BLOCK && blk > 0) {
            off -= BLOCK;
            blk = dir_next(DIR(c, blk));
        }
        if (blk <= 0 || blk > 15) return -1;
        int chunk = len - done;
        if ((u32)chunk > BLOCK - off) chunk = BLOCK - off;
        if (write_) memcpy(c + blk * BLOCK + off, buf + done, chunk);
        else memcpy(buf + done, c + blk * BLOCK + off, chunk);
        done += chunk;
    }
    fds[fd].pos += len;
    if (write_) card_flush(n);
    return len;
}
int port_bu_read(int fd, void *buf, int len) { return file_xfer(fd, buf, len, 0); }
int port_bu_write(int fd, const void *buf, int len) { return file_xfer(fd, (u8 *)buf, len, 1); }
int port_bu_erase(const char *path) {
    const char *name = 0;
    int n = parse_name(path, &name);
    u8 *c = card(n);
    if (!c) return 0;
    int blk = find_file(c, name);
    if (blk < 0) return 0;
    while (blk > 0 && blk < 16) {
        u8 *f = DIR(c, blk);
        int next = dir_next(f);
        f[0] = 0xA0 | (f[0] & 0x0F);            /* 0x51/52/53 -> 0xA1/A2/A3: deleted */
        f[FRAME - 1] = frame_sum(f);
        blk = next;
    }
    card_flush(n);
    if (port_verbose) port_printf("[port] card %d: erased %s\n", n + 1, name);
    return 1;
}
int port_bu_format(const char *path) {
    const char *name = 0;
    int n = parse_name(path, &name);
    u8 *c = card(n);
    if (!c) return 0;
    format_card(c);
    card_flush(n);
    port_printf("[port] card %d: formatted\n", n + 1);
    return 1;
}
struct DIRENTRY {
    char name[20];
    int attr;
    int size;
    struct DIRENTRY *next;
    int head;
    char system[4];
};
static struct {
    int card, block;
    char pat[21];
} dirwalk;
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
    const char *name = 0;
    int n = parse_name(path, &name);
    if (!card(n)) return 0;
    dirwalk.card = n;
    dirwalk.block = 1;
    int i = 0;
    for (; i < 20 && name[i]; i++) dirwalk.pat[i] = name[i];
    dirwalk.pat[i] = 0;
    return dir_fill(d);
}
struct DIRENTRY *port_bu_nextfile(struct DIRENTRY *d) { return dir_fill(d); }
/* the game calls firstfile by its retail address name */
struct DIRENTRY *psyq_8005ba74(const char *path, struct DIRENTRY *d) __attribute__((alias("port_bu_firstfile")));

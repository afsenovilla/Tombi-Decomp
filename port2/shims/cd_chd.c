/* .chd disc images (MAME's compressed hunks; PS1 CDs from `chdman createcd`, CHD v5 with the CD codecs cdlz / cdzl /
   cdfl / cdzs, also v3/v4), read natively through libchdr (port2/third_party/libchdr, BSD-3).  No conversion: each
   read decodes the hunk that holds the sector (one hunk cached: the game reads sequentially).

   CD layout in a CHD: every frame is 2448 bytes (2352 sector + 96 subcode), a hunk holds hunkbytes / 2448 frames, and
   every track starts on a multiple of CD_TRACK_PADDING (4) frames (chdman pads the previous track).  The track list
   is the CHT2 (or old CHTR) metadata: TYPE (MODE2_RAW, MODE1_RAW, MODE1, MODE2_FORM1, AUDIO, ...), FRAMES, PREGAP and
   PGTYPE ("V..." = the pregap frames are stored in the image, before the track's index 1).  Disc LBA n (0 = track 1
   index 1, as in a .bin) is the n-th frame after track 1's stored pregap; later tracks follow at their padded offset
   (their LBAs continue from the previous track's frames).  A raw track yields the 2352 bytes as stored (libchdr
   rebuilds sync / ECC that the cd codecs drop); a cooked 2048 track (MODE1 / MODE2_FORM1) is returned as a Mode 2
   Form 1 raw sector (sync, header, zero subheader, user data at +24), the layout the rest of the CD layer expects.

   Built where PORT_HAVE_CHD is defined (PC and Android); elsewhere (wasm) port_chd_open fails with a message. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_chd.h"

#ifdef PORT_HAVE_CHD
#include <libchdr/chd.h>
#include <libchdr/cdrom.h>

typedef struct { int lba, frames, chd_frame, cooked; } ChdTrack;
static chd_file *chd;
static unsigned char *hunk; static unsigned hunk_bytes, hunk_frames; static int hunk_cached = -1;
static ChdTrack tracks[99]; static int ntracks, total_sectors;

void port_chd_close(void) {
    if (chd) chd_close(chd);
    chd = 0; free(hunk); hunk = 0; hunk_cached = -1; ntracks = 0; total_sectors = 0;
}

int port_chd_open(const char *path, int *sectors, char *err, int errlen) {
    port_chd_close();
    chd_error e = chd_open(path, CHD_OPEN_READ, NULL, &chd);
    if (e != CHDERR_NONE) { snprintf(err, errlen, "chd_open: %s", chd_error_string(e)); chd = 0; return -1; }
    const chd_header *h = chd_get_header(chd);
    hunk_bytes = h->hunkbytes; hunk_frames = hunk_bytes / CD_FRAME_SIZE;
    if (!hunk_frames || hunk_bytes % CD_FRAME_SIZE) {
        snprintf(err, errlen, "not a CD image (hunk of %u bytes is not a multiple of %d)", hunk_bytes, CD_FRAME_SIZE);
        port_chd_close(); return -1;
    }
    hunk = malloc(hunk_bytes);
    int chd_frame = 0, lba = 0;
    for (int i = 0; i < 99; i++) {
        char meta[256] = "", type[32] = "", sub[32] = "", pgtype[32] = "", pgsub[32] = "";
        int tn = 0, frames = 0, pregap = 0, postgap = 0; uint32_t len = 0, tag = 0; uint8_t fl = 0;
        if (chd_get_metadata(chd, CDROM_TRACK_METADATA2_TAG, i, meta, sizeof meta - 1, &len, &tag, &fl) == CHDERR_NONE) {
            meta[len < sizeof meta ? len : sizeof meta - 1] = 0;
            if (sscanf(meta, CDROM_TRACK_METADATA2_FORMAT, &tn, type, sub, &frames, &pregap, pgtype, pgsub, &postgap) < 4) break;
        } else if (chd_get_metadata(chd, CDROM_TRACK_METADATA_TAG, i, meta, sizeof meta - 1, &len, &tag, &fl) == CHDERR_NONE) {
            meta[len < sizeof meta ? len : sizeof meta - 1] = 0;
            if (sscanf(meta, CDROM_TRACK_METADATA_FORMAT, &tn, type, sub, &frames) < 4) break;
        } else break;
        int vpregap = pgtype[0] == 'V' ? pregap : 0;          /* pregap frames stored before index 1 */
        ChdTrack *t = &tracks[ntracks++];
        t->lba = lba; t->frames = frames - vpregap; t->chd_frame = chd_frame + vpregap;
        t->cooked = !strcmp(type, "MODE1") || !strcmp(type, "MODE2_FORM1");
        lba += t->frames;
        chd_frame += (frames + CD_TRACK_PADDING - 1) / CD_TRACK_PADDING * CD_TRACK_PADDING;
    }
    if (!ntracks) {                                           /* no track metadata: one raw track over every frame */
        ChdTrack *t = &tracks[ntracks++];
        t->lba = 0; t->chd_frame = 0; t->cooked = 0;
        t->frames = (int)(h->logicalbytes / CD_FRAME_SIZE);
        lba = t->frames;
    }
    total_sectors = lba;
    if (sectors) *sectors = total_sectors;
    return 0;
}

int port_chd_read_raw(int lba, unsigned char *out) {
    if (!chd || lba < 0) return -1;
    const ChdTrack *t = 0;
    for (int i = 0; i < ntracks; i++) if (lba >= tracks[i].lba && lba < tracks[i].lba + tracks[i].frames) { t = &tracks[i]; break; }
    if (!t) return -2;                                        /* beyond the last track */
    unsigned frame = (unsigned)(t->chd_frame + (lba - t->lba)), hn = frame / hunk_frames;
    if ((int)hn != hunk_cached) {
        if (chd_read(chd, hn, hunk) != CHDERR_NONE) { hunk_cached = -1; return -1; }
        hunk_cached = (int)hn;
    }
    const unsigned char *f = hunk + (size_t)(frame % hunk_frames) * CD_FRAME_SIZE;
    if (!t->cooked) { memcpy(out, f, 2352); return 0; }
    /* cooked 2048: a Mode 2 Form 1 sector around it (MSF header of the LBA + 150, mode 2, zero subheader) */
    static const unsigned char sync[12] = { 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0 };
    memset(out, 0, 2352); memcpy(out, sync, 12);
    int a = lba + 150, m = a / 4500, s = a / 75 % 60, fr = a % 75;
    out[12] = (unsigned char)(m / 10 << 4 | m % 10); out[13] = (unsigned char)(s / 10 << 4 | s % 10);
    out[14] = (unsigned char)(fr / 10 << 4 | fr % 10); out[15] = 2;
    memcpy(out + 24, f, 2048);
    return 0;
}
#else
int port_chd_open(const char *path, int *sectors, char *err, int errlen) {
    (void)path; (void)sectors; snprintf(err, errlen, "this build has no .chd support"); return -1;
}
int port_chd_read_raw(int lba, unsigned char *out) { (void)lba; (void)out; return -1; }
void port_chd_close(void) {}
#endif

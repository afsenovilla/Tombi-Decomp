/* The DSP firmware for the 3DS's sound (n3ds/main_3ds.c, docs/PORT.md "Nintendo 3DS"): libctru's ndsp (SDL's audio
 * on the 3DS) loads sdmc:/3ds/dspfirm.cdc, which is Nintendo's and is not shipped.  When it is missing it is taken
 * from the console's own Home Menu, as the DSP1 tool does (https://github.com/zoogie/DSP1, MIT licence, (C) 2017
 * zoogie): the Home Menu's .code (ExeFS, LZSS-compressed) is read through the "code" archive, decompressed, and the
 * block that starts 0x100 bytes before the "DSP1" magic is written out (its size is the word after the magic). */
#define _GNU_SOURCE
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define DSPFIRM "sdmc:/3ds/dspfirm.cdc"

static u32 le32(const u8 *p) { return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24; }
/* the ExeFS "reverse LZSS" (ctrtool's, as DSP1 has it) */
static int lzss_decompress(const u8 *in, u32 insize, u8 *out, u32 outsize) {
    const u8 *footer = in + insize - 8;
    u32 topbottom = le32(footer), index = insize - ((topbottom >> 24) & 0xFF), stop = insize - (topbottom & 0xFFFFFF), o = outsize;
    memset(out, 0, outsize);
    memcpy(out, in, insize);
    while (index > stop) {
        u8 control = in[--index];
        for (int i = 0; i < 8; i++) {
            if (index <= stop || index == 0 || o == 0) break;
            if (control & 0x80) {
                if (index < 2) return -1;
                index -= 2;
                u32 seg = in[index] | in[index + 1] << 8, size = ((seg >> 12) & 15) + 3;
                seg = (seg & 0x0FFF) + 2;
                if (o < size) return -1;
                for (u32 j = 0; j < size; j++) {
                    if (o + seg >= outsize) return -1;
                    u8 d = out[o + seg];
                    out[--o] = d;
                }
            } else {
                if (o < 1) return -1;
                out[--o] = in[--index];
            }
            control <<= 1;
        }
    }
    return 0;
}
static int dump_from(u64 tid) {
    u32 archive[] = { (u32)(tid & 0xFFFFFFFF), (u32)(tid >> 32), 0 /* NAND */, 0 };
    static const u32 file[] = { 0, 0, 2, 0x646F632E, 0x00000065 };   /* ExeFS ".code" */
    Handle h;
    if (FSUSER_OpenFileDirectly(&h, (FS_ArchiveID)0x2345678a, (FS_Path){ PATH_BINARY, 0x10, archive },
                                (FS_Path){ PATH_BINARY, 0x14, file }, FS_OPEN_READ, 0)) return -1;
    u64 size = 0; u32 got = 0; int rc = -1;
    u8 *buf = 0, *dec = 0;
    if (FSFILE_GetSize(h, &size) || !size || !(buf = malloc((size_t)size))) goto out;
    if (FSFILE_Read(h, &got, 0, buf, (u32)size) || got != size) goto out;
    u32 decsize = le32(buf + size - 4) + (u32)size;
    if (!(dec = malloc(decsize)) || lzss_decompress(buf, (u32)size, dec, decsize)) goto out;
    u8 *m = memmem(dec, decsize, "DSP1", 4);
    if (!m || m - dec < 0x100) goto out;
    u32 fwsize = le32(m + 4);
    if (fwsize < 0x1000 || fwsize > 0x100000 || (u32)(m - 0x100 - dec) + fwsize > decsize) goto out;
    FILE *f = fopen(DSPFIRM, "wb");
    if (!f) goto out;
    rc = fwrite(m - 0x100, 1, fwsize, f) == fwsize ? 0 : -1;
    fclose(f);
    if (rc) remove(DSPFIRM);
out:
    FSFILE_Close(h);
    free(buf); free(dec);
    return rc;
}
/* 0: the firmware is there (already, or dumped now) */
int n3ds_dspfirm(void) {
    struct stat st;
    if (stat(DSPFIRM, &st) == 0 && st.st_size > 0) return 0;
    static const u64 home_menus[] = { 0x0004003000008F02ull, 0x0004003000008202ull, 0x0004003000009802ull,   /* USA JPN EUR */
                                      0x000400300000A102ull, 0x000400300000A902ull, 0x000400300000B102ull }; /* CHN KOR TWN */
    mkdir("sdmc:/3ds", 0777);
    for (unsigned i = 0; i < sizeof home_menus / sizeof home_menus[0]; i++)
        if (dump_from(home_menus[i]) == 0) { fprintf(stderr, "[3ds] dspfirm.cdc taken from the Home Menu (%016llx)\n", home_menus[i]); return 0; }
    fprintf(stderr, "[3ds] no dspfirm.cdc and none could be taken from the Home Menu: no sound\n");
    return -1;
}

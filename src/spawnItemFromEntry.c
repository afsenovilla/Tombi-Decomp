// FUNC 8003ecb0 256 MAIN0
// MATCHING 8003ecb0 256
// VERSION SLPM_80165 slpm80165 8003C028 292
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8003ECB0 (spawnItemFromEntry.c): animation set byte from the 20-byte item table. takes an entry index into the current section's item list.
extern unsigned char D_8006DDEF[];
#include "raw7.h"
extern char *alloc();
extern unsigned char t1[];
extern unsigned char *t2[];
extern unsigned short D_8009A810, D_8009A812;
extern char **D_8006DFF0[];
void spawnItemFromEntry(int idx, char *b, short c, short d, char *e)
{
    char *a = D_8006DFF0[D_8009A810][D_8009A812];
    char *o;
    a += idx * 4;
    o = alloc();
    if (o != 0) {
        U8(o, 0) = 4;
        U8(o, 2) = 9;
        U8(o, 3) = U8(a, 1);
        U8(o, 0xc) = U8(a, 2);
        U8(o, 0xf) = D_8006DDEF[(U8(a, 1)) * 20];
        U16(o, 0x2e) = 0;
        **(int **)(o + 0x40) = S16(b, 2) << 16;
        S32(o, 0x14) = S16(b, 6) << 16;
        **(int **)(o + 0x44) = S16(b, 10) << 16;
        S16(o, 0x80) = c;
        S16(o, 0x82) = d;
        U8(o, 0x6b) = U8(e, 0x6b);
    }
}
#else
#include "raw7.h"
extern char *alloc();
extern unsigned char t1[];
extern unsigned char *t2[];
void spawnItemFromEntry(char *a, char *b, short c, short d, char *e)
{
    char *o = alloc();
    if (o != 0) {
        U8(o, 0) = 4;
        U8(o, 2) = 9;
        U8(o, 3) = U8(a, 1);
        U8(o, 0xc) = U8(a, 2);
        U8(o, 0xf) = t2[t1[U8(a, 1)]][3];
        U16(o, 0x2e) = 0;
        **(int **)(o + 0x40) = S16(b, 2) << 16;
        S32(o, 0x14) = S16(b, 6) << 16;
        **(int **)(o + 0x44) = S16(b, 10) << 16;
        S16(o, 0x80) = c;
        S16(o, 0x82) = d;
        U8(o, 0x6b) = U8(e, 0x6b);
    }
}
#endif

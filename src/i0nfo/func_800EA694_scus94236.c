// VERSION SCUS_94236 info 800EA694 276
#if defined(VERSION_SCUS_94236)
// USA version of PAL i0nfo 800eb384 (overheadLoadMapTiles.c)
#include "spad.h"
typedef struct { unsigned short w, h; unsigned char data[1]; } MapHdr;
extern short func_8005DBCC(int addr, int a, int b);
extern short func_8005DDA8(int a, int b, int c, int d);
extern short func_8005DE78(int a, int b);
extern short D_800A39D0, D_800A39D2, D_800A39D4, D_800A39D6, D_800A4670, D_800A4672;
extern unsigned short D_800A39D8[], D_800A39DA[];
extern short D_800A39DC[], D_800A39DE[];
extern int D_800A39E0[2];
extern unsigned char *OVERHEAD_MAP_TILES;

void overheadLoadMapTiles(void)
{
    unsigned short *map = *(unsigned short **)0x1F800390;
    unsigned char *src;
    int i;
    D_800A39D8[0] = map[0];
    D_800A39DA[0] = map[1];
    D_800A39DC[0] = map[0] >> 2;
    D_800A39DE[0] = map[1] >> 2;
    src = (unsigned char *)(map + 2);
    for (i = 0; i < 0x4000; i++) {
        OVERHEAD_MAP_TILES[i] = *src++;
    }
    D_800A39E0[0] = *(int *)0x1F800394;
    D_800A39E0[1] = D_800A39E0[0] + 0x220;
    D_800A39D0 = func_8005DDA8(1, 0, 0x300, 0);
    D_800A39D2 = func_8005DBCC(D_800A39E0[0] + 0x14, 0x80, 0x1FE);
    D_800A39D4 = func_8005DDA8(1, 0, 0x380, 0);
    D_800A39D6 = func_8005DE78(0x80, 0x1FE);
}
#endif

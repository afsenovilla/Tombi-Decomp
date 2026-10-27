// FUNC 800eb384 324 I0NFO
// MATCHING 800eb384 324
#include "spad.h"
typedef struct { unsigned short w, h; unsigned char data[1]; } MapHdr;
extern short func_8005E174(int addr, int a, int b);
extern short func_8005E350(int a, int b, int c, int d);
extern short func_8005E420(int a, int b);
extern short D_800A4668, D_800A466A, D_800A466C, D_800A466E, D_800A4670, D_800A4672;
extern unsigned short D_800A4674[], D_800A4676[];
extern short D_800A4678[], D_800A467A[];
extern int D_800A467C[2];
extern unsigned char *OVERHEAD_MAP_TILES;

void overheadLoadMapTiles(void)
{
    unsigned short *map = *(unsigned short **)0x1F800390;
    unsigned char *src;
    int i;
    D_800A4674[0] = map[0];
    D_800A4676[0] = map[1];
    D_800A4678[0] = map[0] >> 2;
    D_800A467A[0] = map[1] >> 2;
    src = (unsigned char *)(map + 2);
    for (i = 0; i < 0x4000; i++) {
        OVERHEAD_MAP_TILES[i] = *src++;
    }
    D_800A467C[0] = *(int *)0x1F800394;
    D_800A467C[1] = D_800A467C[0] + 0x220;
    D_800A4668 = func_8005E350(1, 0, 0x300, 0);
    D_800A466A = func_8005E174(D_800A467C[0] + 0x14, 0x80, 0x1FE);
    D_800A466C = func_8005E350(1, 0, 0x380, 0);
    D_800A466E = func_8005E420(0x80, 0x1FE);
    D_800A4670 = func_8005E350(1, 0, 0x40, 0);
    D_800A4672 = func_8005E420(0x80, 0x1FE);
}

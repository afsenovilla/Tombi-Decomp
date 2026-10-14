// FUNC 800352f4 308 MAIN0
// MATCHING 800352f4 308
#include "player.h"
extern unsigned char *D_8009C338;
extern unsigned char D_800A6100;
extern unsigned char *D_800A6078;
extern int D_8009C934;
void playSFX(int id);
int abs(int);
int csqrt(int x);
short angleToPlayer(unsigned char *o);
void func_800352F4(unsigned char *o)
{
    int dx, dy, d, n;

    D_8009C338[0xd] = 0;
    playSFX(0x26);
    D_800A6100 = 1;
    PLAYER.obj.d30 = *(short *)(*(unsigned char **)(o + 0x40) + 2);
    PLAYER.obj.d34 = *(short *)(o + 0x16) + 4;
    dx = *(short *)(*(unsigned char **)(o + 0x40) + 2) - *(short *)(D_800A6078 + 2);
    dx = abs(dx);
    dy = *(short *)(o + 0x16) - PLAYER.obj.y.p.whole;
    dy = abs(dy);
    d = csqrt((dx * dx + dy * dy) << 12) >> 12;
    PLAYER.obj.wb4 = d;
    if (PLAYER.obj.b9c == 0) PLAYER.obj.wb4 = d - 0x10;
    if (PLAYER.obj.wb4 < 0x28) PLAYER.obj.wb4 = 0x28;
    d = angleToPlayer(o);
    n = D_8009C934;
    PLAYER.obj.w7a = d;
    o[5] = 3;
    o[6] = 0;
    *(int *)(o + 0x90) = n;
}

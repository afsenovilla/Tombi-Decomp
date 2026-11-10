// VERSION PCPX_96084 tomba 80034500 308
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Spawns a type-0x13 child of parent at the player's x / z, height -208 on the high ledge (y < -225, x >= 251)
// else -20; CLUT word 0x783E, depth -5, scratchpad word 0x1F800154.
#include "TOBJ.H"
typedef struct { int a, b, c; } V3;
typedef struct { short frac, whole; } F16;
extern TObj *func_8001A464_pcpx96084(void);
extern int func_8008A79C_pcpx96084(void);
extern unsigned short D_800F4FEA, D_800F4FF2;
extern short D_800F4FEE;
extern F16 *D_800F5018;
extern int D_1F800154;

void func_80034500_pcpx96084(TObj *parent)
{
    TObj *n = func_8001A464_pcpx96084();
    if (n != 0) {
        n->active = 1;
        n->type = 0x13;
        n->animFrame = parent->animFrame;
        *(V3 *)((char *)n + 0x10) = *(V3 *)((char *)parent + 0x10);
        {
            unsigned short c = (func_8008A79C_pcpx96084() == 1 || func_8008A79C_pcpx96084() == 2) ? 0x783e : 0x783e;
            *(volatile unsigned short *)&n->w1e = 0;
            *(volatile unsigned short *)((char *)n + 8) = c;
        }
        *(unsigned short *)((char *)n + 0x12) = D_800F4FEA;
        *(unsigned short *)((char *)n + 0x1a) = D_800F4FF2;
        *(volatile short *)((char *)n + 0x16) = (D_800F4FEE >= -225 || D_800F5018->whole < 251) ? -20 : -208;
        *(volatile unsigned char *)&n->b0d = 1;
        *(short *)&n->b0a = 8;
        n->subtype = 0;
        *(signed char *)&n->b0f = -5;
        n->d3c = D_1F800154;
        ((unsigned char *)n)[0x1d] = 0x4d;
        n->d90 = (int)parent;
        n->category |= 0x80;
    }
}
#endif

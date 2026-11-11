// VERSION PCPX_96084 tomba 8006809C 220
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Item icons from D_800AE6BC ({id, a, dx, slot} until id -1): slot + 16 x the halfword +0x14 indexes the item list
// D_800DE3A0 (count D_800DE31E); each present item is drawn by func_80068BD0 with the sprite set up in D_800E2454.
#include "TOBJ.H"
typedef struct { short id, a, x, z; void *img; int clut; } SPR;
extern short D_800AE6BC[];
extern SPR D_800E2454;
extern int D_1F800174;
extern unsigned short D_800DE31E;
extern unsigned char D_800DE3A0[];
extern void func_80068BD0(int);
extern void func_8006897C(void);

void func_8006809C_pcpx96084(TObj *o)
{
    short *p = D_800AE6BC;
    while (p[0] != -1) {
        D_800E2454.id = p[0];
        D_800E2454.a = p[1];
        D_800E2454.x = *(unsigned short *)((char *)o + 0x10) + p[2];
        D_800E2454.z = 0;
        {
            short k = p[3] + (*(short *)((char *)o + 0x14) * 16);
            if (k < D_800DE31E) func_80068BD0(D_800DE3A0[k]);
        }
        p += 4;
    }
}
#endif

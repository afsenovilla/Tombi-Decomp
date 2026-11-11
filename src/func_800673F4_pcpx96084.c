// VERSION PCPX_96084 tomba 800673F4 200
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Sprite parts from the list D_800AE24C ({id, a, dx, frame} until id -1): each one is set up in D_800E2454
// (x = object +0x10 + dx, image = object's frame table at +0x40) and submitted by func_8006897C.
#include "TOBJ.H"
typedef struct { short id, a, x, z; void *img; int clut; } SPR;
extern short D_800AE24C[];
extern SPR D_800E2454;
extern int D_1F800174;
extern void func_8006897C(void);

void func_800673F4_pcpx96084(TObj *o)
{
    short *p = D_800AE24C;
    while (p[0] != -1) {
        D_800E2454.id = p[0];
        D_800E2454.a = p[1];
        D_800E2454.x = *(unsigned short *)((char *)o + 0x10) + p[2];
        D_800E2454.z = 0;
        D_800E2454.img = (*(void ***)((char *)o + 0x40))[p[3]];
        D_800E2454.clut = D_1F800174;
        func_8006897C();
        p += 4;
    }
}
#endif

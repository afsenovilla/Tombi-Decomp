// VERSION PCPX_96084 tomba 80068890 236
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Sprite parts from list 0 (D_800AE83C) or 1 (D_800AE854), {id, a, dx, frame} until id -1, images from the table
// at +0x58; set up in D_800E2454 and submitted by func_8006897C.
#include "TOBJ.H"
typedef struct { short id, a, x, z; void *img; int clut; } SPR;
extern short D_800AE83C[], D_800AE854[];
extern SPR D_800E2454;
extern int D_1F800174;
extern void func_8006897C(void);

void func_80068890_pcpx96084(TObj *o, int which)
{
    short *p;
    switch (which) {
    case 0:
        p = D_800AE83C;
        break;
    case 1:
        p = D_800AE854;
        break;
    }
    while (p[0] != -1) {
        D_800E2454.id = p[0];
        D_800E2454.a = p[1];
        D_800E2454.x = *(unsigned short *)((char *)o + 0x10) + p[2];
        D_800E2454.z = 0;
        D_800E2454.img = (*(void ***)((char *)o + 0x58))[p[3]];
        D_800E2454.clut = D_1F800174;
        func_8006897C();
        p += 4;
    }
}
#endif

// VERSION PCPX_96084 tomba 800684FC 288
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Sprite parts from D_800AE804 ({id, a, dx, frame} until id -1) with images from the table at +0x54: frame -2
// takes the image sel, -1 the one after the halfword +0x14.
#include "TOBJ.H"
typedef struct { short id, a, x, z; void *img; int clut; } SPR;
extern short D_800AE804[];
extern SPR D_800E2454;
extern int D_1F800174;
extern void func_8006897C(void);

void func_800684FC_pcpx96084(TObj *o, int sel)
{
    short *p = D_800AE804;
    while (p[0] != -1) {
        D_800E2454.id = p[0];
        D_800E2454.a = p[1];
        D_800E2454.x = *(unsigned short *)((char *)o + 0x10) + p[2];
        D_800E2454.z = 0;
        {
            short k = p[3];
            switch (k) {
            case -1:
                D_800E2454.img = (*(void ***)((char *)o + 0x54))[*(short *)((char *)o + 0x14) + 1];
                break;
            case -2:
                D_800E2454.img = (*(void ***)((char *)o + 0x54))[sel];
                break;
            default:
                D_800E2454.img = (*(void ***)((char *)o + 0x54))[k];
                break;
            }
        }
        D_800E2454.clut = D_1F800174;
        func_8006897C();
        p += 4;
    }
}
#endif

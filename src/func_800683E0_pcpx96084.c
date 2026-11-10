// VERSION PCPX_96084 tomba 800683E0 284
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Arrow sprites from D_800AE7EC ({id, a, dx, kind} until id -1): kind 0 is hidden at page 0, kind 1 on the last
// page (n - 1, page in the halfword +0x14); images from the table at +0x50.
#include "TOBJ.H"
typedef struct { short id, a, x, z; void *img; int clut; } SPR;
extern short D_800AE7EC[];
extern SPR D_800E2454;
extern int D_1F800174;
extern void func_8006897C(void);

void func_800683E0_pcpx96084(TObj *o, int n)
{
    short *p = D_800AE7EC;
    while (p[0] != -1) {
        if ((p[3] == 0 && *(short *)((char *)o + 0x14) == 0) || (p[3] == 1 && *(short *)((char *)o + 0x14) >= n - 1)) {
            p += 4;
            continue;
        }
        D_800E2454.id = p[0];
        D_800E2454.a = p[1];
        D_800E2454.x = *(unsigned short *)((char *)o + 0x10) + p[2];
        D_800E2454.z = 0;
        D_800E2454.img = (*(void ***)((char *)o + 0x50))[p[3]];
        D_800E2454.clut = D_1F800174;
        func_8006897C();
        p += 4;
    }
}
#endif

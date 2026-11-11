// VERSION PCPX_96084 tomba 80067D84 284
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Sprite parts from D_800AE50C ({id, a, dx, frame} until id -1); the frame is offset by 16 x the halfword +0x14
// (0 outside 0..23) and the part is drawn only when D_800DE21C[frame] is 2 (image from the table at +0x5C).
#include "TOBJ.H"
typedef struct { short id, a, x, z; void *img; int clut; } SPR;
extern short D_800AE50C[];
extern SPR D_800E2454;
extern int D_1F800174;
extern unsigned char D_800DE21C[];
extern void func_8006897C(void);

void func_80067D84_pcpx96084(TObj *o)
{
    short *p = D_800AE50C;
    while (p[0] != -1) {
        D_800E2454.id = p[0];
        D_800E2454.a = p[1];
        D_800E2454.x = *(unsigned short *)((char *)o + 0x10) + p[2];
        D_800E2454.z = 0;
        {
            short k = p[3] + (*(short *)((char *)o + 0x14) * 16);
            if (k < 0) k = 0;
            if (k >= 24) k = 0;
            if (D_800DE21C[k] == 2) {
                D_800E2454.img = (*(void ***)((char *)o + 0x5c))[k - 1];
                D_800E2454.clut = D_1F800174;
                func_8006897C();
            }
        }
        p += 4;
    }
}
#endif

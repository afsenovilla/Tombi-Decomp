// FUNC 800ee7b8 500 D0SPSUB
// MATCHING 800ee7b8 500
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short x0, x1, y0, y1, p[2]; } Limit;
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
extern short D_801110F8, D_80111100;
extern unsigned short D_8009D2C4[];
extern Limit D_800FCED0[];

void func_800EE7B8(TObj *o)
{
    SVECTOR v;
    int sxy;
    short sx, sy;
    short i, k;
    v.vx = o->a.p.whole;
    v.vy = o->y.p.whole;
    v.vz = o->b.p.whole;
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&sxy);
    sx = sxy;
    sy = sxy >> 16;
    if (sx < 0x30) D_80111100 += 2;
    else if (sx >= 0x111) D_80111100 -= 2;
    if (sy < 0x50) D_801110F8 += 2;
    else if (sy >= 0xB1) D_801110F8 -= 2;
    k = 0;
    for (i = 0; i < 8; i++) {
        if (D_8009D2C4[i] != 0) k = i;
    }
    if (D_800FCED0[k].y1 < D_80111100) D_80111100 = D_800FCED0[k].y1;
    if (D_80111100 < D_800FCED0[k].y0) D_80111100 = D_800FCED0[k].y0;
    if (D_800FCED0[k].x1 < D_801110F8) D_801110F8 = D_800FCED0[k].x1;
    if (D_801110F8 < D_800FCED0[k].x0) D_801110F8 = D_800FCED0[k].x0;
}

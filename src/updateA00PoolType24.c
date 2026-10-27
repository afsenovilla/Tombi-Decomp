// FUNC 8011b2e8 456 X000
// MATCHING 8011b2e8 456
#include "TOBJ.H"
extern short A00_POOL_TYPE24_CLUT_X[];
extern short A00_POOL_TYPE24_CLUT_Y[];
extern unsigned short A00_POOL_TYPE24_DRAW_DEPTH[];
extern char D_8013BF48[];
extern char D_8013BF4C[];
extern char D_8013BF50[];
extern char D_8013BF54[];
static void **PTR_80138680[4] = {
    (void **)D_8013BF48, (void **)D_8013BF4C, (void **)D_8013BF50, (void **)D_8013BF54,
};
extern int DAT_1f8002e4[];
extern short FUN_8005e420(int, int);
extern void cullObjectToDrawList(TObj *, int);
extern void FUN_800187e4(TObj *);

void updateA00PoolType24(TObj *o)
{
    unsigned char t = o->b04;
    unsigned char u;
    unsigned int v;
    unsigned short x;
    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->w1e = 7;
        o->b0d = 0;
        if (A00_POOL_TYPE24_CLUT_Y[o->subtype] != 0) {
            o->b0d = 1;
            o->w08 = FUN_8005e420(A00_POOL_TYPE24_CLUT_X[o->subtype], A00_POOL_TYPE24_CLUT_Y[o->subtype] + o->b0c);
        }
        o->d3c = DAT_1f8002e4[0];
        x = (unsigned short)o->box0 >> 4 & 0xf;
        u = x;
        o->anim = *PTR_80138680[o->subtype];
        if (u < 0xc) {
            o->d64 = u * 0x400 + 0x1000;
            o->w74 = (unsigned char)x;
        } else {
            o->d64 = 0x1000 - (u - 0xb) * 0x200;
            o->w74 = -((u - 0xb) >> 1);
        }
        v = *(unsigned char *)&o->box0 & 0xf;
        if (v < 8)
            o->d8c = v << 2;
        else
            o->d8c = (unsigned char)(((int)(v - 7) * -0x20) / 8);
        break;
    case 1:
        cullObjectToDrawList(o, (short)(A00_POOL_TYPE24_DRAW_DEPTH[o->subtype] + o->w74 * 0x20));
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}

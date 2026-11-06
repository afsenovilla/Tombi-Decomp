// VERSION SLPS_01144 x10 8011A5D0 584
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct {
    unsigned char b0;
    char pad1[7];
    unsigned char b8;
    char pad9[0x1f];
    unsigned short w28;
    unsigned short w2a;
    unsigned short w2c;
    unsigned short w2e;
} G330;
#define OB(o, n) (((unsigned char *)(o))[n])
extern G330 *D_8009DEE0;
extern short D_8009E4EC;
extern struct { short s; } GVARS_PLAYER_DRIFT_Y;
extern int D_8009EE90;
extern void func_80110398(TObj *);
extern void func_800EED18(TObj *, int);

void func_8011A5D0_slps01144(TObj *o)
{
    switch (o->state) {
    case 0:
        o->timer = 10;
        o->d84 = 0;
        o->d88 = (o->animFrame & 1) ? 240 : 16;
        OB(o, 0x9d) = 1;
        OB(o, 0xc8) = 0;
        OB(o, 0xa0) = 0;
        OB(o, 0xa1) = 0;
        o->d8c = 0;
        o->wb0 = 0;
        ((short *)o)[0x5b] = 0;
        D_8009DEE0->b0 = 2;
        D_8009DEE0->w2e = -1;
        D_8009DEE0->w28 = -1;
        D_8009DEE0->w2a = -1;
        o->state = 2;
        break;
    case 2:
        o->h->raw += D_8009E4EC << 8;
        o->y.raw += GVARS_PLAYER_DRIFT_Y.s << 8;
        func_80110398(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (OB(o, 0xac) == 2) {
            D_8009EE90 = *(int *)&OB(o, 0xd4);
            D_8009DEE0->b8 = 0;
            OB(o, 0xa5) = 0;
            o->step = 14;
            o->state = 0;
            break;
        } else if (o->velY > 0) {
            o->timer = 10;
            o->d84 = 0;
            o->d88 = (o->animFrame & 1) ? 240 : 16;
            OB(o, 0xac) = 1;
            OB(o, 0x9c) = 2;
            o->step = 4;
            o->d8c = 0;
            o->state = 3;
        }
        if (D_8009DEE0->b0 == 0) {
            if (OB(o, 0xc6) == 0) {
                OB(o, 0xac) = 0;
                func_800EED18(o, 4);
                OB(o, 0x9d) = 0;
                OB(o, 0x9c) = 1;
                o->d84 = 0;
                o->d88 = (o->animFrame & 1) ? 240 : 16;
                o->d8c = 0;
                o->step = 2;
                o->state = 2;
            }
        } else if (OB(o, 0xc8)) {
            D_8009DEE0->b8 = 0;
            OB(o, 0xac) = 0;
            OB(o, 0xa7) = 0;
            OB(o, 0x9c) = 0;
            o->step = 50;
            o->state = 0;
        }
        break;
    }
}
#endif

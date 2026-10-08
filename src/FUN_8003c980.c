// FUNC 8003c980 696 MAIN0
// MATCHING 8003c980 696
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7;
    short cx, cy;
    unsigned char bx0, bx1, bx2, bx3;
    void **anims;
} Desc;
typedef struct { short x, y, p0, p1; } ClutPos;
extern unsigned char DAT_8007b084[];
extern Desc *DAT_8007b14c[];
extern ClutPos DAT_8007b2d4[];
extern unsigned char DAT_8009ce5b;
extern unsigned char DAT_8009d0c1;
extern unsigned char DAT_8009d0bc;
extern int D_1f8002c8[];
extern unsigned short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);

void FUN_8003c980(TObj *o)
{
    Desc *d;
    int v;

    if (o->subtype == 100 && DAT_8009ce5b == 0) {
        o->b04 = 2;
        return;
    }
    o->b68 = 0;
    o->b69 = 0;
    d = DAT_8007b14c[DAT_8007b084[o->subtype]];
    o->b0a = d->b0;
    o->w1e = d->b2;
    S8(o, 0xf) = -9;
    o->b0d = d->b5;
    o->box0 = d->bx0;
    o->box1 = d->bx1;
    o->box2 = d->bx2;
    o->box3 = d->bx3;
    o->d3c = D_1f8002c8[d->b4];
    o->animFrame = 1;
    if (o->subtype == 2) {
        if ((o->b0c & 0x7f) >= 4) {
            o->d64 = 0x2000;
            o->box0 = d->bx0 * 2;
            o->box1 = d->bx1 * 2;
            o->box2 = d->bx2 * 2;
            o->box3 = d->bx3 * 2;
        } else {
            o->d64 = 0x1000;
        }
    }
    switch (d->b6) {
    case 0:
        o->w08 = FUN_8005e420(d->cx, d->cy);
        break;
    case 1:
        o->w08 = FUN_8005e420(d->cx, d->cy + (o->b0c & 0x7f));
        break;
    case 2:
        o->w08 = FUN_8005e420(DAT_8007b2d4[o->b0c & 0x7f].x, DAT_8007b2d4[o->b0c & 0x7f].y);
        break;
    case 3:
        if (DAT_8009d0c1 == 0)
            v = 0;
        else if (DAT_8009d0bc == 0)
            v = 1;
        else
            v = 2;
        o->w08 = FUN_8005e420(d->cx, d->cy + v);
        break;
    }
    switch (d->b7) {
    case 0:
        o->anim = d->anims[0];
        break;
    case 1:
        o->anim = d->anims[o->b0c & 0x7f];
        break;
    }
    FUN_8001fe6c(o);
}

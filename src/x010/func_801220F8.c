// FUNC 801220f8 1076 X010
// MATCHING 801220f8 1076
#include "TOBJ.H"

typedef struct { short x, y, z, pad; } V801220F8;

extern void *D_80131CD4[];
extern void *D_80131CC8[];
extern void FUN_8001e4f0(int);

static __inline__ int nearCam(TObj *o)
{
    short dv;
    if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F800172 + 45) >= 91) return 0;
    dv = *(unsigned short *)0x1F80016E - o->y.p.whole;
    if (dv >= -31 || (unsigned short)(dv + 172) >= 173) return 0;
    return (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 16) < 33;
}

void func_801220F8(TObj *o)
{
    V801220F8 *a = (V801220F8 *)&o->wb4;
    V801220F8 *b = (V801220F8 *)&o->wbc;
    V801220F8 *c = (V801220F8 *)((char *)o + 0xc4);
    V801220F8 *d = (V801220F8 *)((char *)o + 0xcc);
    TObj *p;

    switch (o->state) {
    case 0:
        o->velV = -0x800;
        o->velY = 0x80;
        o->d8c = 0;
        o->b69 = 0;
        o->timer = 0;
        o->w22 = 0;
        a->x = -4;
        a->y = 0;
        a->z = 0;
        b->x = 4;
        b->y = 0;
        b->z = 0;
        c->x = -4;
        c->y = 0x20;
        c->z = 0;
        d->x = 4;
        d->y = 0x20;
        d->z = 0;
        o->b0a = 4;
        o->state++;
        break;
    case 1:
        if (o->b69 == 1) {
            o->step = 3;
            o->state = 0;
            FUN_8001e4f0(0xae);
        } else {
            o->y.raw += o->velV << 8;
            o->velV += o->velY;
            c->y -= o->velV >> 9;
            d->y -= o->velV >> 9;
            if (o->velV >= 0) {
                o->state = 2;
                o->timer = 0;
                o->w22 = 0;
                o->velV = 0;
                o->velY = 0x30;
                o->anim = D_80131CD4[0];
            }
            switch (o->animFrame) {
            case 1:
                o->d8c = (o->d8c + 6) & 0xff;
                if (++o->w22 >= 4)
                    o->animFrame = 3;
                break;
            case 3:
                o->d8c = (o->d8c - 6) & 0xff;
                if (--o->w22 < -3)
                    o->animFrame = 1;
                break;
            }
        }
        break;
    case 2:
        if (o->b69 == 1) {
            o->step = 3;
            o->state = 0;
            FUN_8001e4f0(0xae);
        } else {
            o->y.raw += o->velV << 8;
            o->velV += o->velY;
            c->y -= o->velV >> 10;
            d->y -= o->velV >> 10;
            if (o->velV >= 0x200)
                o->anim = D_80131CC8[o->subtype];
            if (o->d34 - o->y.p.whole < 25)
                o->state = 3;
        }
        break;
    case 3:
        if (o->b69 == 1) {
            o->step = 3;
            o->state = 0;
            FUN_8001e4f0(0xae);
        } else {
            o->y.raw += o->velV << 8;
            o->velV += o->velY;
            if (nearCam(o)) {
                o->velV = -0x700;
                o->velY = 0x80;
                o->state = 1;
                a->x = -4;
                a->y = 0;
                a->z = 0;
                b->x = 4;
                b->y = 0;
                b->z = 0;
                c->x = -4;
                c->y = 0x20;
                c->z = 0;
                d->x = 4;
                d->y = 0x20;
                d->z = 0;
            }
            if (o->y.p.whole >= o->d34) {
                o->state++;
                p = (TObj *)o->d94;
                p->b6a = 0;
                p = (TObj *)p->d94;
                p->b6a = 0;
            }
        }
        break;
    case 4:
        o->step = 0;
        o->state = 0;
        o->b0a = 2;
        o->y.raw = o->d34 << 16;
        break;
    }
}

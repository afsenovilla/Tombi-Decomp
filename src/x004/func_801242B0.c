// FUNC 801242b0 1320 X004
// MATCHING 801242b0 1320
#include "TOBJ.H"

typedef struct { short x, y, z, pad; } V3S;

extern void *D_80134D44[];
extern void *D_80134D38[];
extern unsigned short D_1F8001C8;
extern unsigned short D_8009C962;
extern void playSFX(int);

static __inline__ int nearCam(TObj *o)
{
    short dv;
    if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F800172 + 45) >= 91) return 0;
    dv = *(unsigned short *)0x1F80016E - o->y.p.whole;
    if (dv >= -31 || (unsigned short)(dv + 172) >= 173) return 0;
    return (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 16) < 33;
}

void func_801242B0(TObj *o)
{
    V3S *a = (V3S *)&o->wb4;
    V3S *b = (V3S *)&o->wbc;
    V3S *c = (V3S *)((char *)o + 0xc4);
    V3S *d = (V3S *)((char *)o + 0xcc);
    TObj *p;

    switch (o->state) {
    case 0:
        o->b69 = 0;
        o->velV = -0x800;
        o->d8c = 0;
        o->velY = 0x80;
        o->timer = 0;
        o->w22 = 0;
        if (D_1F8001C8 & 1) {
            a->z = -4;
            a->y = 0;
            a->x = 0;
            b->z = 4;
            b->y = 0;
            b->x = 0;
            c->z = -4;
            c->y = 0x20;
            c->x = 0;
            d->z = 4;
            d->y = 0x20;
            d->x = 0;
        } else {
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
        o->b0a = 4;
        o->state++;
        break;
    case 1:
        if (o->b69 == 1) {
            o->step = 3;
            o->state = 0;
            if (D_8009C962 < 4) playSFX(0xf8);
            else playSFX(0xfa);
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
                o->anim = D_80134D44[0];
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
            if (D_8009C962 < 4) playSFX(0xf8);
            else playSFX(0xfa);
        } else {
            o->y.raw += o->velV << 8;
            o->velV += o->velY;
            c->y -= o->velV >> 10;
            d->y -= o->velV >> 10;
            if (o->velV >= 0x200)
                o->anim = D_80134D38[o->subtype];
            if (o->d34 - o->y.p.whole < 25)
                o->state = 3;
        }
        break;
    case 3:
        if (o->b69 == 1) {
            o->step = 3;
            o->state = 0;
            if (D_8009C962 < 4) playSFX(0xf8);
            else playSFX(0xfa);
        } else {
            o->y.raw += o->velV << 8;
            o->velV += o->velY;
            if (nearCam(o)) {
                o->velV = -0x700;
                o->velY = 0x80;
                o->state = 1;
                if (D_1F8001C8 & 1) {
                    a->z = -4;
                    a->y = 0;
                    a->x = 0;
                    b->z = 4;
                    b->y = 0;
                    b->x = 0;
                    c->z = -4;
                    c->y = 0x20;
                    c->x = 0;
                    d->z = 4;
                    d->y = 0x20;
                    d->x = 0;
                } else {
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
            }
            if (o->y.p.whole >= o->d34) {
                o->state++;
                o->b6a = 0;
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

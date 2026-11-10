// VERSION SLPM_80165 x00 80125034 1044
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of bitingPlantLungeHead (src/bitingPlantLungeHead.c): the rising step reacts only to a hit of kind 1,
// the falling step to none; a wider vertical window for the next lunge; the chain's +0x6A flags are not cleared.
#include "TOBJ.H"

typedef struct { short x, y, z, pad; } V8012F3F4;

extern void *D_8013B230[];
extern void *BITING_PLANT_ANIMS[];
extern void FUN_8001e4f0(int);

static __inline__ int nearCam(TObj *o)
{
    if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F800172 + 45) >= 91) return 0;
    if ((unsigned short)(o->y.p.whole - *(unsigned short *)0x1F80016E + 0xb0) >= 0x161) return 0;
    return (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 16) < 33;
}

void func_80125034(TObj *o)
{
    V8012F3F4 *a = (V8012F3F4 *)&o->wb4;
    V8012F3F4 *b = (V8012F3F4 *)&o->wbc;
    V8012F3F4 *c = (V8012F3F4 *)((char *)o + 0xc4);
    V8012F3F4 *d = (V8012F3F4 *)((char *)o + 0xcc);

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
            FUN_8001e4f0(0x2c);
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
                o->anim = D_8013B230[0];
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
            FUN_8001e4f0(0x2c);
        } else if (o->b69 == 2) {
            o->step = 4;
            o->state = 0;
            FUN_8001e4f0(0x2c);
        } else {
            o->y.raw += o->velV << 8;
            o->velV += o->velY;
            c->y -= o->velV >> 10;
            d->y -= o->velV >> 10;
            if (o->velV >= 0x200)
                o->anim = BITING_PLANT_ANIMS[o->subtype];
            if (o->d34 - o->y.p.whole < 25)
                o->state = 3;
        }
        break;
    case 3:
        {
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
            if (o->y.p.whole >= o->d34)
                o->state++;
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
#endif

// FUNC 8012781c 2404 X009
// MATCHING 8012781c 2404
#include "TOBJ.H"

extern void *D_8012EEA8[], *D_8012EEAC[], *D_8012EEA4[];
extern unsigned char D_8009D2C3;
extern unsigned char D_8009D083;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);
extern int func_801276CC(TObj *);
extern void FUN_8003e3cc(int, int, Fix16 *, int, int);

static __inline__ int wall(TObj *o, unsigned short d)
{
    unsigned char b = o->b9d;
    short v, s;

    if ((b & 2) && o->animFrame == (b & 1))
        return 1;
    s = d;
    v = 0x10;
    if (s)
        v = -0x10;
    return func_8004065C(o, o->h->p.whole + v, o->y.p.whole + 0x30, s);
}

#define BOUNCE(o) \
    if (wall(o, o->w74)) { \
        short t; \
        int u; \
        o->w74 = 1 - o->w74; \
        u = o->velH; \
        t = -u; \
        o->velH = t; \
        if (t < 0) \
            o->velH = t + o->velX; \
        else \
            o->velH = t - o->velX; \
    }

void func_8012781C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->active = 4;
        o->state++;
        if (--o->w98 == 0) {
            o->active = 2;
        }
        o->wac = 0;
        o->anim = D_8012EEA8[0];
        AnimLoadDuration(o);
        if (o->b6b) {
            (*(TObj **)&o->wa8)->animFrame = 1 - o->animFrame;
            o->b6b = 0;
            o->w9a = 0;
            playSFX(0x9d);
        }
        if (o->animFrame) {
            o->velH = 0x180;
            o->velX = 0x80;
            o->w74 = 0;
        } else {
            o->velH = -0x180;
            o->velX = 0x80;
            o->w74 = 1;
        }
        o->velV = -0x400;
        o->velY = 0x20;
        o->b0a = 0;
        break;
    case 1:
        o->h->raw += o->velH << 8;
        BOUNCE(o);
        o->y.raw += o->velV << 8;
        if (o->velV < 0x400) {
            o->velV += o->velY;
        }
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
        if (AnimAdvance(o)) {
            o->wac = 1;
            o->anim = D_8012EEAC[0];
            AnimLoadDuration(o);
            if (o->w98 == 0) {
                o->state = 10;
                o->b0b = 1;
                *(signed char *)&o->b0f = 100;
            } else {
                o->state++;
            }
        }
        break;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        BOUNCE(o);
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        if (o->velV > 0 && TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->velV = -0x400;
            o->velY = 0x20;
            if (o->velH < 0) {
                o->velH = o->velH + o->velX;
            } else {
                o->velH = o->velH - o->velX;
            }
            o->state++;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        BOUNCE(o);
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        if (o->velV > 0 && TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->velV = -0x300;
            o->velY = 0x20;
            if (o->velH < 0) {
                o->velH = o->velH + o->velX;
            } else {
                o->velH = o->velH - o->velX;
            }
            o->state++;
        }
        break;
    case 4:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        BOUNCE(o);
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        if (o->velV > 0 && TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->state++;
        }
        break;
    case 5:
        o->timer = 0x20;
        o->b0a = 2;
        o->d8c = 0x80;
        o->wac = 0;
        o->state++;
        o->anim = D_8012EEA8[0];
        AnimLoadDuration(o);
    case 6:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        wall(o, o->w74);
        o->y.raw += -0x18000;
        o->d8c -= 4;
        if (--o->timer == -1) {
            o->state++;
        }
        break;
    case 7:
        o->d8c = 0;
        o->b0a = 0;
        o->wac = 1;
        o->state++;
        o->anim = D_8012EEA4[0];
        AnimLoadDuration(o);
        break;
    case 8:
        o->y.raw += -0x8000;
        if (AnimAdvance(o)) {
            o->state++;
        }
        break;
    case 9:
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->active = 1;
        o->b0b = 0;
        o->b0f = 0;
        break;
    case 10:
        if (func_801276CC(o)) {
            if (o->subtype == 0) {
                if (!(D_8009D2C3 & 1)) {
                    FUN_8003e3cc(0x8a, 0, &o->a, 0, 0);
                    D_8009D083 = 1;
                } else {
                    FUN_8003e3cc(0xc, 0, &o->a, 0, 0);
                    D_8009D083 = 2;
                }
            }
            o->state++;
        }
        break;
    case 11:
        if (o->visible == 0) {
            o->b04 = 3;
            break;
        }
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        break;
    }
}

// FUNC 80126688 1480 X004
// MATCHING 80126688 1480
#include "TOBJ.H"

typedef struct {
    TObj t;
    unsigned short wc0, wc2, wc4, wc6, wc8, wca, wcc;
} BX;
typedef struct { short x, y; } P2;

extern short *D_800A6078;
extern short D_800A604E;
extern short D_1F8001C8;
extern void *D_801345B8[], *D_801345E8[], *D_801345EC[], *D_80134604[];
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern short FUN_8002078c(P2, P2);
extern short MulCos(int, short);
extern short MulNegSinScaled(int, short);
extern short FUN_800411cc(TObj *, short, short);

static __inline__ short wall(TObj *o)
{
    if (FUN_800411cc(o, o->h->p.whole + 0x20, o->y.p.whole + 0x18)) return 1;
    if (FUN_800411cc(o, o->h->p.whole - 0x20, o->y.p.whole + 0x18)) return 1;
    return 0;
}

#define o (&b->t)
void func_80126688(BX *b)
{
    P2 p, q;

    switch (o->substep) {
    case 0:
        if (o->h->p.whole >= D_800A6078[1]) o->animFrame = 1;
        else o->animFrame = 0;
        o->w22 = 0x38;
        o->wac = 0x14;
        b->wcc = o->animFrame;
        o->anim = D_80134604[0];
        AnimLoadDuration(o);
        if (b->wca) playSFX(0x13);
        o->substep++;
        break;
    case 1:
        if (--o->w22 != 0) break;
        o->velV = -0x400;
        o->w22 = 0x38;
        o->velH = 0;
        o->wac = 1;
        o->anim = D_801345B8[0];
        AnimLoadDuration(o);
        o->substep++;
        break;
    case 2:
        if (o->velV >= 0) {
            o->w22 = 0x1e;
            o->substep++;
        }
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        break;
    case 3:
        if (--o->w22 != 0) break;
        o->w22 = 0x78;
        o->wb6 = 0;
        o->wac = 0xd;
        o->anim = D_801345E8[0];
        AnimLoadDuration(o);
        p.x = o->h->p.whole;
        p.y = o->y.p.whole;
        q.x = D_800A6078[1];
        q.y = D_800A604E;
        *(short *)&o->bbe = FUN_8002078c(p, q) + 0x100;
        if (o->h->p.whole >= D_800A6078[1]) o->animFrame = 1;
        else o->animFrame = 0;
        o->substep++;
        break;
    case 4:
        o->h->raw += MulCos(*(unsigned short *)&o->bbe & 0xf8, o->wb6) << 8;
        o->y.raw += MulNegSinScaled(*(unsigned short *)&o->bbe & 0xf8, o->wb6) << 8;
        if ((unsigned short)o->wb6 < 0x400) o->wb6 += 0x20;
        if (--o->w22 != 0 && !wall(o) && b->wc0 == D_1F8001C8) break;
        if (b->wc0 != D_1F8001C8) {
            o->velH = 0;
        } else if (o->animFrame) {
            o->velH = -0x300;
        } else {
            o->velH = 0x300;
        }
        o->velV = -0x400;
        o->timer = 0x1e;
        o->wac = 0xe;
        o->anim = D_801345EC[0];
        AnimLoadDuration(o);
        o->substep++;
        break;
    case 5:
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        if (--o->timer != 0 && !wall(o) && b->wc0 == D_1F8001C8) break;
        o->active = 1;
        o->timer = 1;
        o->substep++;
        break;
    case 6:
        if (--o->timer != 0 && !wall(o) && b->wc0 == D_1F8001C8) break;
        if (D_1F8001C8 == 0) {
            if (b->wc4 < o->h->p.whole) o->animFrame = 0;
            else o->animFrame = 1;
        } else {
            if (b->wc8 < o->h->p.whole) o->animFrame = 0;
            else o->animFrame = 1;
        }
        o->state = 3;
        o->step = 0;
        o->substep = 0;
        o->wb4 = 1;
        break;
    }
}

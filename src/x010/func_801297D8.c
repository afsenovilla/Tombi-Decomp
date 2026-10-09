// FUNC 801297d8 1956 X010
// MATCHING 801297d8 1956
#include "TOBJ.H"

extern const unsigned char D_8012F3D4[];
extern void *D_80132310[];
extern unsigned char D_8012F474[], D_8012F47C[], D_8012F484[];
extern unsigned short D_1F800172, D_1F80016A, D_1F80016E;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001e4f0(int);
extern int Rand(void);
extern short FUN_80040278(TObj *, int, int);

#define SETA(K)                                  \
    {                                            \
        const unsigned char *p;                  \
        o->wac = K;                              \
        p = &D_8012F3D4[(K) * 4];                \
        o->box0 = p[0];                          \
        o->box1 = p[1];                          \
        o->box2 = p[2];                          \
        o->box3 = p[3];                          \
        o->anim = D_80132310[K];                 \
        AnimLoadDuration(o);                     \
    }

static __inline__ int near(TObj *o)
{
    unsigned short t;
    short lim = 0x90;
    t = D_1F800172 - o->d->p.whole + 0x2d;
    if (t >= 0x5b) return 0;
    t = D_1F80016A - o->h->p.whole + 0x48;
    if (t > lim) return 0;
    t = D_1F80016E - o->y.p.whole + 0x80;
    return t <= lim + 0x70;
}

void func_801297D8(TObj *o)
{
    int r;

    FUN_8001f8e4(o);
    switch (o->state) {
    case 0:
        o->state++;
        o->b9c = 0;
        if (*(unsigned short *)&o->wb4) SETA(0xf)
        else SETA(0xb)
        break;
    case 1:
        if (!AnimAdvance(o)) break;
        if (o->visible) FUN_8001e4f0(0xa4);
        o->b9c = 1;
        o->velV = -0x400;
        o->state++;
        if (*(unsigned short *)&o->wb4) SETA(0x10)
        else SETA(0xc)
        break;
    case 2:
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        break;
    case 3:
        o->velV += 0x20;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->b69 == 1) {
            o->b69 = 0;
            r = 1;
        } else if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x1a))) {
            o->b69 = 0;
            r = 1;
        } else {
            r = 0;
        }
        if (r) {
            o->state++;
            o->b9c = 0;
            if (*(unsigned short *)&o->wb4) SETA(0xf)
            else SETA(0xb)
        }
        break;
    case 4:
        if (!AnimAdvance(o)) break;
        if (!near(o)) {
            o->step = 1;
            o->state = 0;
            break;
        }
        if (Rand() & 1) {
            unsigned char t = D_8012F484[Rand() & 0xf];
            o->timer = t;
            if (t == 0) {
                if (o->visible) FUN_8001e4f0(0xa4);
                o->b9c = 1;
                o->state = 2;
                o->velV = -0x400;
                if (*(unsigned short *)&o->wb4) SETA(0x10)
                else SETA(0xc)
            } else {
                o->state = 5;
                if (*(unsigned short *)&o->wb4) SETA(1)
                else SETA(0x16)
            }
        } else {
            o->timer = D_8012F474[Rand() & 7];
            o->state = 6;
            if (*(unsigned short *)&o->wb4) SETA(1)
            else SETA(0x16)
        }
        break;
    case 5:
        if (--o->timer != -1) break;
        o->b9c = 0;
        o->state = 1;
        if (*(unsigned short *)&o->wb4) SETA(0xf)
        else SETA(0xb)
        break;
    case 6:
        if (--o->timer != -1) break;
        if (!near(o) || D_8012F47C[Rand() & 7] == 1) {
            o->step = 1;
            o->state = 0;
            break;
        }
        o->b9c = 0;
        o->state = 1;
        if (*(unsigned short *)&o->wb4) SETA(0xf)
        else SETA(0xb)
        break;
    }
}

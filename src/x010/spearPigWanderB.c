// FUNC 801291c4 1556 X010
// MATCHING 801291c4 1556
#include "spad.h"
#include "TOBJ.H"

extern const unsigned char D_8012F3D4[];
extern void *D_80132310[];
extern unsigned char D_8012F474[], D_8012F484[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);
extern int Rand(void);
extern short FUN_80040278(TObj *, int, int);
extern short probeWallAt(TObj *, short, short, short);

#define SETA(K)                                  \
    {                                            \
        const unsigned char *p;                        \
        o->wac = K;                              \
        p = &D_8012F3D4[(K) * 4];             \
        o->box0 = p[0];                          \
        o->box1 = p[1];                          \
        o->box2 = p[2];                          \
        o->box3 = p[3];                          \
        o->anim = D_80132310[K];            \
        AnimLoadDuration(o);                     \
    }

static __inline__ int near(TObj *o)
{
    unsigned short t;
    short lim = 0x90;
    t = (*(unsigned short*)((char*)&SPAD.i170 + 2)) - o->d->p.whole + 0x2d;
    if (t >= 0x5b) return 0;
    t = (*(unsigned short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole + 0x48;
    if (t > lim) return 0;
    t = (*(unsigned short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole + 0x80;
    return t <= lim + 0x70;
}

static __inline__ int blocked(TObj *o, short f)
{
    short d;
    if (f & 1) d = -0x10;
    else d = 0x10;
    return probeWallAt(o, o->h->p.whole + d, o->y.p.whole, f) != 0;
}

void spearPigWanderB(TObj *o)
{
    short yy;
    int r;

    switch (o->state) {
    case 0:
        o->ba7 = 0;
        o->state++;
    case 1:
        o->state++;
        FUN_8001f8e4(o);
        o->w7a = o->animFrame;
        o->timer = D_8012F474[Rand() & 7];
        if (*(unsigned short *)&o->wb4) SETA(1)
        else SETA(0x16)
        break;
    case 2:
        FUN_8001f8e4(o);
        if (--o->timer == -1) {
            o->timer = 0x78;
            if (Rand() & 1) {
                if (*(unsigned short *)&o->wb4) SETA(1)
                else SETA(0)
                o->state = 3;
            } else {
                o->w7a = 1 - o->animFrame;
                if (*(unsigned short *)&o->wb4) SETA(0x25)
                else SETA(0x24)
                o->state = 4;
            }
        }
        break;
    case 3:
        FUN_8001f8e4(o);
        AnimAdvance(o);
        if (o->animFrame) {
            o->h->raw -= 0x8000;
        } else {
            o->h->raw += 0x8000;
            if (o->h->p.whole > 0xb68) o->h->p.whole = 0xb68;
        }
        if (--o->timer == -1) {
            o->state = 1;
        } else {
            if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) r = 1;
            else r = blocked(o, o->animFrame);
            if (r) o->state = 1;
        }
        break;
    case 4:
        FUN_8001f8e4(o);
        o->w7a = 1 - o->animFrame;
        AnimAdvance(o);
        if (o->w7a) {
            o->h->raw -= 0x8000;
        } else {
            o->h->raw += 0x8000;
            if (o->h->p.whole > 0xb68) o->h->p.whole = 0xb68;
        }
        if (--o->timer == -1) {
            o->state = 1;
        } else {
            r = blocked(o, o->w7a);
            if (r) o->state = 1;
        }
        break;
    }
    yy = o->y.p.whole;
    o->y.p.whole = yy + 2;
    if (o->b69 == 1 || FUN_80040278(o, o->h->p.whole, (short)(yy + 0x1c))) o->b69 = 0;
    if (o->ba7 == 0) {
        if (((SPAD.s1f8 + SPAD.i198) & 3) == 0 && near(o)) {
            o->ba7 = 1;
            o->w22 = D_8012F484[Rand() & 0xf];
        }
    } else if (--o->w22 == -1) {
        o->state = 0;
        o->step++;
    }
}

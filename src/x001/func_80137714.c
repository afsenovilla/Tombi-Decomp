// FUNC 80137714 596 X001
// MATCHING 80137714 596
#include "TOBJ.H"

extern short D_8007A3F0[];
extern unsigned short D_1F80016A, D_1F80016E;
extern unsigned char D_8009C959;
extern int AnimAdvance(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

static __inline__ int inView(TObj *o)
{
    if ((unsigned short)(o->y.p.whole - D_1F80016E + 0xb0) >= 0x161) return 0;
    return (unsigned short)(o->h->p.whole - D_1F80016A + 0x12c) < 0x259;
}

void func_80137714(TObj *o)
{
    switch (o->state) {
    case 0:
        o->timer = 4;
        if (o->animFrame) o->velH = -0x100;
        else o->velH = 0x100;
        o->velY = 0;
        o->velX = 0;
        o->state++;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw = o->y.raw + (D_8007A3F0[o->d84] * o->velH >> 4) - 0x8000;
        o->d84 = (o->d84 + 2) & 0xff;
        func_8004065C(o, o->h->p.whole, o->y.p.whole, o->animFrame);
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 8);
        if (o->d84 == 0) {
            if (--o->timer == -1) {
                o->timer = 8;
                o->state++;
            }
        }
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->state = 0;
            o->velH = -o->velH;
            o->animFrame = 1 - o->animFrame;
        }
        break;
    }
    if (o->visible == 0 && !inView(o)) {
        o->active = 2;
        o->b04 = 3;
        { unsigned char *c = &D_8009C959; *c = *c - 1; }
    }
}

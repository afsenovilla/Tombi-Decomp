// FUNC 8012724c 576 X003
// MATCHING 8012724c 576
#include "TOBJ.H"

extern char D_80077D3C[];
extern unsigned char D_80135CB0[];
extern void *D_80139510;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_800A4582;
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void playSFX(int);
extern void FUN_8001faf4(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short isObjectBelowGround(TObj *);

void func_8012724C(TObj *o)
{
    unsigned char *b;

    switch (o->state) {
    case 0:
        o->b9c = 1;
        o->movetab = D_80077D3C;
        o->wac = 4;
        o->animFrame = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->state++;
        o->anim = D_80139510;
        b = &D_80135CB0[o->wac * 4];
        o->box0 = *b++;
        o->box1 = *b++;
        o->box2 = *b++;
        o->box3 = *b++;
        FUN_8001fe6c(o);
    case 1:
        AnimAdvance(o);
        if (o->visible && ((D_1F8001F8 + D_1F800198) & 0x1f) == 0) playSFX(0x76);
        FUN_8001faf4(o);
        if (func_8004065C(o, o->h->p.whole + 0x18, o->y.p.whole, 0)) {
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        if (o->visible && ((D_1F8001F8 + D_1F800198) & 0x1f) == 0) playSFX(0x76);
        AnimAdvance(o);
        o->y.p.whole++;
        func_8004065C(o, o->h->p.whole + 0x18, o->y.p.whole, 0);
        if (o->y.p.whole > D_800A4582 + 0xa0) o->b04 = 3;
        break;
    }
    if (isObjectBelowGround(o) == 1 && o->visible == 0) o->b04 = 3;
}

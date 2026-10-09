/* score 40: only case 0 differs: game reloads lbu state first (before sb b9c) and keeps the box-table pointer in v0 (ours: lbu after sh wac, regs swapped). Tried all positions/forms of state++, early s = o->state (CSEs with the switch value), anim/table order, [0] anim. */
// FUNC 80126f38 788 X003
#include "TOBJ.H"

extern char D_80077D3C[];
extern void *D_8013950C;
extern unsigned char D_80135CB0[];
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_1F80016A, D_1F80027E;
extern short D_800A4582;
extern void playSFX(int);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void FUN_8001faf4(TObj *);
extern short FUN_8004065c(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

void func_80126F38(TObj *o)
{
    unsigned char *b;
    int v;

    switch (o->state) {
    case 0:
        o->b9c = 1;
        o->movetab = D_80077D3C;
        o->wac = 4;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->state++;
        b = &D_80135CB0[o->wac * 4];
        o->anim = D_8013950C;
        o->box0 = *b++;
        o->box1 = *b++;
        o->box2 = *b;
        o->box3 = b[1];
        AnimLoadDuration(o);
    case 1:
        if (o->visible && ((D_1F8001F8 + D_1F800198) & 0x1f) == 0) playSFX(0x76);
        AnimAdvance(o);
        FUN_8001faf4(o);
        if (o->animFrame & 1)
            v = -0x18;
        else
            v = 0x18;
        if (FUN_8004065c(o, o->h->p.whole + v, o->y.p.whole, o->animFrame)
            || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0xe)
            || (unsigned short)(v = (unsigned short)D_1F80016A - (unsigned short)o->h->p.whole + 0x40) < 0x80) {
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        if (o->visible && ((D_1F8001F8 + D_1F800198) & 0x1f) == 0) playSFX(0x76);
        AnimAdvance(o);
        o->y.p.whole++;
        if (o->animFrame & 1)
            v = -0x18;
        else
            v = 0x18;
        FUN_8004065c(o, o->h->p.whole + v, o->y.p.whole, o->animFrame);
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0xe)) {
            o->d8c = (-D_1F80027E << 2) & 0xff;
            o->state = 1;
            o->b9c = 0;
            o->step++;
        }
        break;
    }
    if (((D_1F8001F8 + D_1F800198) & 0xf) == 0 && D_800A4582 + 0xa0 < o->y.p.whole) o->b04 = 3;
}

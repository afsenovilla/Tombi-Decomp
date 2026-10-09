// FUNC 80126d2c 352 X001
/* score 49: logic complete (subtype-7 skip, d/d38 range checks, surface height table D_8013C6A4[subtype][-dy>>3],
   box0/box1 side clamp, carry state). Left: game reads e->h->p.whole with lhu and keeps h unsigned (sll/sra only
   for the d30 store, done after the compare value), loads box1 with lh after the d30 store and copies it to t0
   (the subtype register), e->y read before o->y; the 8-byte frame (first draft had it) is lost. Tried: types of
   t/h/c, compare/statement orders, volatile table read. */
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short e8;
    unsigned short ea;
} P;
extern short D_1F80019E;
extern TObj *D_1F8003C0;
extern unsigned short *D_8013C6A4[];

void func_80126D2C(P *o, TObj *e)
{
    int t;
    int dy;
    int h;
    unsigned short b0;
    int x;
    int c;

    t = e->subtype;
    if (t == 7) return;
    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    dy = e->d34;
    dy = o->ea - dy;
    if ((unsigned short)(dy + e->d38) > e->d38) return;
    h = D_8013C6A4[t][(short)-dy >> 3];
    b0 = e->box0;
    e->d30 = (short)h;
    c = b0 + (o->e8 - (e->h->p.whole + h));
    t = e->box1;
    if (t < (unsigned short)c) return;
    if (!(o->t.animFrame & 1)) x = -b0;
    else x = t - b0;
    o->t.h->p.whole = x + (e->h->p.whole + h);
    o->t.b9e = 7;
    o->t.wb8 = x;
    D_1F80019E = 0;
    D_1F8003C0 = e;
    o->t.wba = o->t.y.p.whole - e->y.p.whole;
    o->t.velY = 0;
    if (((unsigned char *)o)[0xa9] == 0) {
        e->b68 = 2;
        e->animFrame = o->t.animFrame & 1;
    }
}

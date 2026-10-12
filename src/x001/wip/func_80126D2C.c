// FUNC 80126d2c 352 X001
/* score 22 (o32): only the middle block: game loads box0 (lhu a0, no andi copy) before the volatile h read and
   box1 (lh v0 + move t0) after the d30 store; ours hoists lh box1 to the top and copies box0 with andi.
   Found: h reused for dy + d38 (a3), short dy / unsigned short h (type brute force), volatile h read keeps
   lhu + sll/sra for d30 (plain read is combined into an lh reload), char pad[8] for the frame.
   Tried: statement orders of b0/d30/c/box1, raw d30 store, short s copy for box1, b0 int/short.
   n55 (still 22, different residue): `int b0 = (unsigned short)e->box0` removes the andi copy and lets the lhu box0
   hoist above the volatile h read when the d30 store comes right after h; `int s = e->box1; t = s;` gives the
   `move t0` copy but cse rewrites the compare to t (lh hoisted to the top, slt on t0). A trailing dead `x = s`
   or computing the compare into a flag before the copy flips it the other way (compare on s, lh placed late as
   in the game, but the copy vanishes: 33). The game needs both: compare on the lh reg and a copy into t0 for
   the else branch. Moving the d30 store after the c computation loses the sll/sra pair (52). */
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
    short dy;
    unsigned short h;
    int b0;
    int s;
    int x;
    int c;
    char pad[8];

    t = e->subtype;
    if (t == 7) return;
    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    dy = e->d34;
    dy = o->ea - dy;
    h = dy + e->d38;
    if ((unsigned short)h > e->d38) return;
    h = *(volatile unsigned short *)&D_8013C6A4[t][(short)-dy >> 3];
    b0 = (unsigned short)e->box0;
    e->d30 = (short)h;
    c = b0 + (o->e8 - ((unsigned short)e->h->p.whole + h));
    s = e->box1; t = s;
    if (s < (unsigned short)c) return;
    if (!(o->t.animFrame & 1)) x = -b0;
    else x = t - b0;
    o->t.h->p.whole = x + ((unsigned short)e->h->p.whole + h);
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

// FUNC 800447c0 480 MAIN0
// score 70: s-reg assignment reversed (game a=s0,b=s1,r=s2); same issue as func_80042D5C
#include "TOBJ.H"
typedef struct { unsigned short x, y; } Ofs;
extern Ofs D_8007B610[];
int func_800425C4(TObj *a, TObj *b);
void playSFX(int);
void FUN_8001f96c(int, int, int, int);
static __inline__ int react(TObj *a, TObj *b)
{
    int r;
    r = 1;
    b->b68 = 1;
    b->b9e = 0;
    switch (func_800425C4(a, b)) {
    case 1:
    case 7:
        r = 1;
        break;
    case 4:
    case 5:
    case 10:
        r = 1;
        goto common;
    case 0:
    case 3:
    case 9:
        r = 2;
        break;
    case 13:
        b->b68 = 0;
        r = -1;
        break;
    case 6:
    case 11:
    case 12:
        r = 2;
    common:
        a->b6a = 1;
        a->active = 2;
        a->wa8 = 0x4ff;
        break;
    case 2:
    case 8:
        r = 0;
        b->b9e = 1;
        b->b9f = 0;
        break;
    }
    return r;
}
void func_800447C0(TObj *a, TObj *b)
{
    Ofs *t;
    int dx, dy;
    int r;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return;
    t = &D_8007B610[b->b0c];
    dx = t->x;
    dy = t->y;
    if ((unsigned short)(a->h->p.whole - (b->h->p.whole + dx) + (b->box0 + a->box0)) > b->box1 + a->box1) return;
    if ((unsigned short)(a->y.p.whole - (b->y.p.whole + dy) - 4 + (b->box2 + a->box2)) > a->box3 + b->box3 - 4) return;
    r = react(a, b);
    if (r >= 0) {
        FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
        if ((b->category & 0x7f) == 4) playSFX(6);
        else playSFX(7);
    }
    *(short *)0x1F80019E = 0;
}

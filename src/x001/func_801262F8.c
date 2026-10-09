// FUNC 801262f8 620 X001
// MATCHING 801262f8 620
#include "TOBJ.H"

extern unsigned char D_1F8001A4;
extern unsigned char D_800A6038, D_800A603C, D_800A603D, D_800A603E;
extern short func_8012614C(TObj *, TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8004258c(TObj *, int);

#define BAC(o) (*(unsigned char *)&(o)->wac)
#define PE4(o) (*(TObj **)((char *)(o) + 0xe4))

void func_801262F8(TObj *o, TObj *p)
{
    short r;
    unsigned char k;
    short s;

    r = func_8012614C(o, p);
    if (r == -1) return;
    k = BAC(o);
    if (k == 2) s = k;
    else if (p->wac == 0x13) s = 0;
    else s = k;
    switch (s) {
    case 1:
        if (r < 3) {
            p->active = 2;
            p->b6a = 0;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            p->animFrame = o->animFrame & 1;
            PE4(o) = p;
            BAC(o) = 2;
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            break;
        }
    case 0:
    case 3:
        if (p->active & 2) break;
        if (D_1F8001A4) break;
        if (o->active & 2) break;
        if (r == 3 && p->b6a == 1) {
            if (BAC(o) == 1) BAC(o) = 0;
            p->active = 4;
            p->b6a = 2;
            o->active = 2;
            o->b04 = 1;
            o->step = 0x40;
            o->state = 0;
            o->substep = 0;
            break;
        }
        o->active = 2;
        o->animFrame = p->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        FUN_8004258c(o, 1);
        break;
    case 2:
        if (p->active == 3) break;
        p->active = 3;
        if (p->b6a == 2) {
            D_800A6038 = 1;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
        }
        p->b6a = 0;
        p->b04 = 2;
        p->step = 0;
        p->state = 0;
        p->w7a = o->h->p.whole > p->h->p.whole;
        break;
    }
}

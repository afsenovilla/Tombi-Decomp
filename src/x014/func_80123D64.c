// FUNC 80123d64 1016 X014
// MATCHING 80123d64 1016
#include "TOBJ.H"
extern void *D_80129FA8[];
extern unsigned short D_801266E0[];
extern unsigned short D_1F800176, D_1F80017A[], D_1F800172[], D_1F80016A[], D_1F80016E[];
extern short D_8007A5F0[], D_8007A1F0[];
extern short D_1F800284;
extern short D_800A457C, D_800A457E, D_800A4582;
extern short D_800A4580[]; /* debt: second name for D_800A4582 keeps the game's reload */
extern void FUN_800202b4(TObj *);
extern void AnimLoadDuration(TObj *);
extern int FUN_8001f9e0(void);
extern int FUN_800205d8(short, short);
extern TObj *FUN_800183b8(void);
extern short FUN_80041240(TObj *, int, int);
extern short FUN_80041ebc(TObj *, int, int);

static __inline__ short Out(TObj *o)
{
    if (o->h->p.whole < D_800A457C - 0x9e) return 1;
    if (o->h->p.whole > D_800A457E + 0x9e) return 1;
    return o->y.p.whole > D_800A4580[1] + 0x80;
}

static __inline__ int Chk(TObj *o, short k)
{
    short r = FUN_80041240(o, o->h->p.whole, o->y.p.whole);
    int n = k;
    if (r != 0 && n == r && D_1F800284 == 0)
        n++;
    else if (FUN_80041ebc(o, o->h->p.whole, o->y.p.whole) == 0)
        n = 0;
    else
        n = D_1F800284 == 0;
    return n;
}

void func_80123D64(TObj *o)
{
    TObj *n;
    TObj *p;
    short s;

    switch (o->step) {
    case 0:
        FUN_800202b4(o);
        o->y.p.whole -= 4;
        if (o->visible) break;
        o->timer = 0x1e;
        o->step++;
        break;
    case 1:
        if (--o->timer != -1) break;
        o->step++;
        o->anim = D_80129FA8[0];
        AnimLoadDuration(o);
        o->velH = 0x480;
        o->h->p.whole = D_1F800176 + D_801266E0[FUN_8001f9e0() & 7];
        o->y.p.whole = D_1F80017A[0] - 0xf0;
        o->d->p.whole = D_1F800172[0];
        o->d38 = FUN_800205d8(D_1F80016A[0] - o->h->p.whole, D_1F80016E[0] - o->y.p.whole);
        o->d8c = (unsigned char)(o->d38 + 0x40);
        o->d38 = *(unsigned char *)&o->d38;
        n = FUN_800183b8();
        if (n) {
            unsigned char f;
            n->active = 2;
            n->type = 0x4b;
            f = o->b0f;
            n->b0a = 10;
            n->subtype = 0;
            n->b0c = 0;
            n->ba5 = 0;
            n->ba6 = 0;
            n->ba7 = 0;
            n->velX = 0x50;
            n->b0f = f - 4;
            n->d8c = o->d8c;
            n->a.p.whole = o->a.p.whole;
            n->y.p.whole = o->y.p.whole - 10;
            n->b.p.whole = o->b.p.whole;
            o->d94 = (int)n;
        }
        o->active = 1;
        break;
    case 2:
        FUN_800202b4(o);
        p = (TObj *)o->d94;
        {
            int i = o->d38;
            short v = o->velH;
            int a = v * D_8007A5F0[i];
            int b = v * D_8007A1F0[i];
            o->h->raw += (a << 4 >> 16) << 8;
            o->y.raw += (b << 4 >> 16) << 8;
        }
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole - 10;
        s = 2;
        if (((o->d38 - 0x40) & 0xff) > 0x80) s = 1;
        if ((o->b69 = Chk(o, s)) != 0) {
            o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            break;
        }
        if (o->y.p.whole > D_800A4582 + 0xa0 || Out(o)) {
            o->active = 2;
            o->b04 = 3;
            p->b04 = 3;
        }
        break;
    }
}

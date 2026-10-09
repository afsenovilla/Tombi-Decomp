// FUNC 8013b344 448 X001
// MATCHING 8013b344 448
#include "TOBJ.H"

extern unsigned char D_8009CE56;
extern unsigned char D_8009CEAF[];
extern int *D_1F800338[];
extern TObj *FUN_800185f8(void);
extern void FUN_801148d0(TObj *, char, int, int, short);

void func_8013B344(TObj *o)
{
    TObj *e, *f;
    short i;
    short n;

    o->b04 = 3;
    if (D_8009CE56 == 0xff) return;
    e = FUN_800185f8();
    if (e) {
        int **g;
        int *b;
        int t;
        g = D_1F800338;
        b = *g;
        e->active = 1;
        e->type = 6;
        e->animFrame = 0;
        e->b0a = 0x10;
        e->subtype = 0;
        e->b0c = 0;
        e->b0f = 0;
        t = (int)*g + b[1];
        e->a.p.whole = 0xdc2;
        e->y.p.whole = -0x3a5;
        e->ba4 = 1;
        e->b.p.whole = 0x880;
        e->da0 = t;
        *(TObj **)&o->category = e;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
    }
    f = FUN_800185f8();
    if (f) {
        int **g;
        int *b;
        int t;
        g = D_1F800338;
        b = *g;
        f->active = 1;
        f->type = 6;
        f->animFrame = 0;
        f->b0a = 0x10;
        f->subtype = 1;
        f->b0c = 0;
        f->b0f = 0;
        t = (int)*g + b[2];
        f->a.p.whole = 0xdc2;
        f->y.p.whole = -0x3a5;
        f->ba4 = 1;
        f->b.p.whole = 0x880;
        f->da0 = t;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
    }
    e->d94 = (int)f;
    f->d90 = (int)e;
    n = D_8009CEAF[0];
    if (n > 30) n = 30;
    for (i = 0; i < n; i++) FUN_801148d0(e, 2, e->a.p.whole, e->y.p.whole, 0x87c);
}

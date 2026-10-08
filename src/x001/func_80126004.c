// FUNC 80126004 328 X001
// MATCHING 80126004 328
#include "TOBJ.H"
extern unsigned char D_8009CEB0;
extern short func_80044550(TObj *, TObj *);
extern void func_800428C0(TObj *, TObj *);
extern int func_800429D0(TObj *, TObj *);
extern void playSFX(int);
extern void FUN_8001f96c(int, int, int, int);
extern void func_80132698(int, int, int);
extern void FUN_8002ee50(int, int, int, int);

void func_80126004(TObj *o, TObj *e)
{
    int r;

    if (func_80044550(o, e) < 0) {
        return;
    }
    if (D_8009CEB0) {
        func_800428C0(o, e);
        return;
    }
    r = func_800429D0(o, e);
    if (r == 0) {
        return;
    }
    if (r >= 3) {
        if (r != 6) {
            playSFX(5);
        }
        FUN_8001f96c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        func_80132698(e->h->p.whole, e->y.p.whole, e->d->p.whole);
        FUN_8002ee50(0, e->h->p.whole, e->y.p.whole, e->d->p.whole);
        return;
    }
    if (r == 2) {
        e->w98 = 0;
        playSFX(5);
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    } else {
        if (--e->w98 <= 0) {
            e->w98 = 0;
        }
        playSFX(5);
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    }
}

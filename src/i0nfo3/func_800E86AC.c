// FUNC 800e86ac 568 I0NFO3
// MATCHING 800e86ac 568
#include "TOBJ.H"
extern void func_800270A0(TObj *o, int a, int b);
extern void func_80027810(TObj *o);
extern void func_8002A4D0(TObj *o);
extern void func_800E847C(TObj *o);
extern void (*D_80079AD0[])(TObj *o);
extern unsigned char D_8009C93A;
extern unsigned char D_8009CFED;
extern TObj D_800A6038;
extern short D_800A604A;
extern short D_800A604E;

void func_800E86AC(TObj *o)
{
    TObj *p = &D_800A6038;
    switch (o->step) {
    case 0:
        *(int *)&o->anim = 0;
        *(int *)&o->timer = -0x1E00;
        if (D_8009C93A != 0) o->step++;
        func_80027810(o);
        break;
    case 1:
        func_800E847C(p);
        D_80079AD0[o->subtype](o);
        if (D_800A604A >= 0x448 && D_800A604A < 0x478 && D_800A604E >= -0x474 && D_800A604E < -0x460) {
            func_800270A0(o, 1, 0);
        } else if (p->a.p.whole < 0x99 && p->y.p.whole >= -0x4FE && p->y.p.whole < -0x47E) {
            func_800270A0(o, 1, 1);
        } else if (p->a.p.whole < 0x99 && p->y.p.whole >= -0x2E8 && p->y.p.whole < -0x268) {
            D_8009CFED = 1;
            func_800270A0(o, 1, 2);
        } else if (p->a.p.whole >= 0x65F && p->a.p.whole < 0x6A7 && p->y.p.whole >= -0x2D0 && p->y.p.whole < -0x2BC) {
            func_800270A0(o, 1, 3);
        } else if (p->a.p.whole >= 0x667 && p->a.p.whole < 0x6AF && p->y.p.whole >= -0x4B0 && p->y.p.whole < -0x497) {
            func_800270A0(o, 1, 4);
        }
        break;
    case 2:
        func_8002A4D0(o);
        break;
    }
}

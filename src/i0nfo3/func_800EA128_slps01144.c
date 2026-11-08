// VERSION SLPS_01144 info3 800EA128 568
// JP version of PAL i0nfo3 800e86ac (tunnelSectionStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern void func_80027EA8(TObj *o, int a, int b);
extern void func_80028620(TObj *o);
extern void func_8002B2D4(TObj *o);
extern void func_800E9F0C(TObj *o);
extern void (*D_80075864[])(TObj *o);
extern unsigned char D_8009E4E2;
extern unsigned char D_8009EB95;
extern TObj D_800A7B78;
extern short D_800A7B8A;
extern short D_800A7B8E;

void func_800EA128(TObj *o)
{
    TObj *p = &D_800A7B78;
    switch (o->step) {
    case 0:
        *(int *)&o->anim = 0;
        *(int *)&o->timer = -0x1E00;
        if (D_8009E4E2 != 0) o->step++;
        func_80028620(o);
        break;
    case 1:
        func_800E9F0C(p);
        D_80075864[o->subtype](o);
#ifdef PORT2
        { extern int port_noclip; if (port_noclip) break; }   /* --noclip: the exits do not fire */
#endif
        if (D_800A7B8A >= 0x450 && D_800A7B8A < 0x470 && D_800A7B8E >= -0x46B && D_800A7B8E < -0x44F) {
            func_80027EA8(o, 1, 0);
        } else if (p->a.p.whole < 0x99 && p->y.p.whole >= -0x4FE && p->y.p.whole < -0x47E) {
            func_80027EA8(o, 1, 1);
        } else if (p->a.p.whole < 0x99 && p->y.p.whole >= -0x2E8 && p->y.p.whole < -0x268) {
            D_8009EB95 = 1;
            func_80027EA8(o, 1, 2);
        } else if (p->a.p.whole >= 0x66B && p->a.p.whole < 0x69B && p->y.p.whole >= -0x2D0 && p->y.p.whole < -0x2BC) {
            func_80027EA8(o, 1, 3);
        } else if (p->a.p.whole >= 0x673 && p->a.p.whole < 0x6A3 && p->y.p.whole >= -0x4B0 && p->y.p.whole < -0x497) {
            func_80027EA8(o, 1, 4);
        }
        break;
    case 2:
        func_8002B2D4(o);
        break;
    }
}
#endif

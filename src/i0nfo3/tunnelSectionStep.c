// FUNC 800e86ac 568 I0NFO3
// MATCHING 800e86ac 568
#include "TOBJ.H"
extern void func_800270A0(TObj *o, int a, int b);
extern void func_80027810(TObj *o);
extern void func_8002A4D0(TObj *o);
extern void tunnelClampPlayerToGates(TObj *o);
extern void (*D_80079AD0[])(TObj *o);
extern unsigned char D_8009C93A;
extern unsigned char D_8009CFED;
extern TObj PLAYER;
extern short PLAYER_X;
extern short PLAYER_Y;

void tunnelSectionStep(TObj *o)
{
    TObj *p = &PLAYER;
    switch (o->step) {
    case 0:
        *(int *)&o->anim = 0;
        *(int *)&o->timer = -0x1E00;
        if (D_8009C93A != 0) o->step++;
        func_80027810(o);
        break;
    case 1:
        tunnelClampPlayerToGates(p);
        D_80079AD0[o->subtype](o);
        if (PLAYER_X >= 0x448 && PLAYER_X < 0x478 && PLAYER_Y >= -0x474 && PLAYER_Y < -0x460) {
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

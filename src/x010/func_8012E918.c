// FUNC 8012e918 188 X010
// MATCHING 8012e918 188
#include "TOBJ.H"
extern TObj D_800A6038;
extern unsigned char D_800A603D, D_800A603E;
extern short D_800A604E, D_800A60B0;

void func_8012E918(TObj *o)
{
    TObj *p = &D_800A6038;

    switch (D_800A603D) {
    case 0x3e:
        if (D_800A604E < o->y.raw) {
            D_800A604E = o->y.raw;
            D_800A603D = 0x3d;
            D_800A603E = 0;
            D_800A60B0 = o->y.raw;
        }
        break;
    case 2:
    case 0xd:
        if (o->y.raw < p->y.p.whole) {
            p->y.p.whole = o->y.raw;
            p->w78 = o->y.raw;
            p->step = 0x3d;
            p->state = 0;
        }
        break;
    }
}

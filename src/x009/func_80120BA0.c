// FUNC 80120ba0 200 X009
// MATCHING 80120ba0 200
#include "TOBJ.H"

extern short D_1F80019E;
extern unsigned char D_800A6038, D_800A603C, D_800A603D, D_800A603E;
extern short func_8004461C(TObj *, TObj *);
extern int func_80042AAC(TObj *, TObj *);

void func_80120BA0(TObj *o, TObj *e)
{
    if (func_8004461C(o, e) == -1) return;
    if (func_80042AAC(o, e)) {
        if (!(e->active & 2)) {
            unsigned short f;
            e->active = 6;
            f = o->animFrame;
            e->b04 = 2;
            e->step = 0;
            e->state = 0;
            e->animFrame = f & 1;
        }
        if (e->b6a == 2) {
            D_800A6038 = 1;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
        }
        e->b6a = 0;
    }
    D_1F80019E = 0;
}

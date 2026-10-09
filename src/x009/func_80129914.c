// FUNC 80129914 440 X009
// MATCHING 80129914 440
#include "TOBJ.H"

extern unsigned char D_8009C942[], D_8009C93F[];
extern TObj D_800A6038;
extern void FUN_800eeae4(TObj *, int, int);

void func_80129914(TObj *o)
{
    switch (o->step) {
    case 0:
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_800A6038.animFrame = 1;
        D_800A6038.b04 = 5;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
        D_800A6038.visible = 1;
        FUN_800eeae4(&D_800A6038, 2, 0);
        D_800A6038.b69 = 0;
        *(signed char *)&D_800A6038.b0f = -8;
        o->step++;
    case 1:
        if (D_800A6038.b69) {
            D_800A6038.b04 = 5;
            D_800A6038.step = 100;
            D_800A6038.state = 0;
            D_800A6038.y.p.whole = -0x1d0;
            FUN_800eeae4(&D_800A6038, 2, 0);
            o->step++;
        }
        break;
    case 2:
        D_800A6038.h->raw -= 0x10000;
        if (D_800A6038.h->p.whole < 0x1068) {
            D_800A6038.h->p.whole = 0x1068;
            FUN_800eeae4(&D_800A6038, 0x2c, 0);
            o->step++;
        }
        break;
    case 3:
        D_800A6038.d->raw -= 0x10000;
        break;
    }
}

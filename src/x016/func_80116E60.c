// FUNC 80116e60 144 X016
// MATCHING 80116e60 144
#include "TOBJ.H"

extern unsigned char D_800A60E0;
extern TObj D_800A6038;

void func_80116E60(TObj *o)
{
    char pad[16];

    switch (o->state) {
    case 0:
        D_800A60E0 = 8;
        break;
    case 1:
        D_800A6038.d->p.whole--;
        break;
    case 2:
        D_800A6038.d->p.whole++;
        break;
    }
}

// FUNC 8012f10c 360 X004
// MATCHING 8012f10c 360
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009C93E[], D_8009C93F[], D_8009C942[];
extern void FUN_8005a9a4(int, int);
extern void addItemToInventory(int, int, int);

void func_8012F10C(TObj *o)
{
    switch (o->state) {
    case 0:
        D_800A6038.b04 = 5;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        break;
    case 1:
        D_8009C93E[0] = 1;
        o->w08 = 0xc8;
        o->state++;
        FUN_8005a9a4(0x38, 0);
        break;
    case 2:
        if (--o->w08 > 0) break;
        D_800A6038.b04 = 1;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        o->state++;
        break;
    case 3:
        addItemToInventory(0x35, 1, 1);
        o->w08 = 0x3c;
        o->state = 0xf;
        break;
    case 15:
        if (--o->w08 > 0) break;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        break;
    }
}

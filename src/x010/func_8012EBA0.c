// FUNC 8012eba0 312 X010
// MATCHING 8012eba0 312
#include "TOBJ.H"
extern unsigned char D_8009C93A, D_8009C93E, D_8009C93F, D_8009C942;
extern unsigned char D_800A6038, D_800A603C, D_800A603D, D_800A603E;
extern void FUN_8005a8a8(int, int, int);

void func_8012EBA0(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        break;
    case 1:
        if (D_8009C93A == 0) break;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 2:
        FUN_8005a8a8(0x7b, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 3:
        if (--o->w08 == 0) o->state = 15;
        break;
    case 15:
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        D_800A6038 = 1;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

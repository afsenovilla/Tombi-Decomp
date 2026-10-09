// FUNC 801337b4 388 X003
// MATCHING 801337b4 388
#include "TOBJ.H"
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);

void func_801337B4(TObj *o)
{
    switch (o->state) {
    case 0:
        D_800A603C = 5;
        D_800A603D = 100;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        FUN_8005a8a8(0xaa, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 1:
        if (--o->w08 <= 0) o->state = 0xf;
        break;
    case 2:
        D_800A603C = 5;
        D_800A603D = 100;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        FUN_8005a9a4(0xaa, 0);
        o->w08 = 200;
        o->state = 1;
        break;
    case 0xf:
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

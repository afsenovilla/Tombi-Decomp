// FUNC 8011767c 308 X016
// MATCHING 8011767c 308
#include "TOBJ.H"

extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern unsigned char D_8009D0E2;
extern short D_800A60EA;
extern void FUN_8005a9a4(int, int);
extern void removeItemFromInventory(int, int);

void func_8011767C(TObj *o)
{
    switch (o->state) {
    case 0:
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        D_8009C93E = 1;
        FUN_8005a9a4(0x61, 0);
        removeItemFromInventory(0x3e, D_8009D0E2);
        o->w08 = 200;
        o->state++;
        break;
    case 2:
        if (--o->w08 <= 0) {
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_8009C93E = 0;
            D_800A60EA = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}

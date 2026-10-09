// FUNC 8012dfbc 584 X004
// MATCHING 8012dfbc 584
#include "TOBJ.H"
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009C93E, D_8009C93F, D_8009C942;
extern unsigned char D_8009CDDB, D_8009CF29;
extern short D_800A6058;
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);

void func_8012DFBC(TObj *o)
{
    switch (o->state) {
    case 0:
        D_800A603C = 5;
        D_800A603D = 100;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        FUN_8005a8a8(0x37, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 1:
        if (--o->w08 <= 0) o->state = 15;
        break;
    case 2:
        D_800A603C = 5;
        D_800A603D = 7;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 3:
        D_8009C93E = 1;
        if (D_8009CDDB == 0xff) {
            D_800A603D = 0x20;
            D_800A6058 = 1;
            D_800A603C = 1;
            D_800A603E = 0;
            o->state = 5;
        } else {
            FUN_8005a9a4(0x37, 0);
            o->w08 = 250;
            o->state++;
        }
        break;
    case 4:
        if (--o->w08 <= 0) {
            D_800A603C = 1;
            D_800A603D = 0x20;
            D_800A603E = 0;
            D_800A6058 = 1;
            o->state++;
        }
        break;
    case 5:
        {
            short *p = &D_800A6058;
            D_8009CF29 = 1;
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_8009C93E = 0;
            (*p)--;
        }
        o->step = 0;
        o->state = 0;
        break;
    case 15:
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

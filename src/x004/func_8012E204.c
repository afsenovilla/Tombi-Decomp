// FUNC 8012e204 584 X004
// MATCHING 8012e204 584
/* Real size 584 B: includes the csv piece func_8012E438 (20 B), the epilogue of this function. */
#include "TOBJ.H"

extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E, D_8009CDD7, D_8009CF29;
extern unsigned short D_800A6058;
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);

void func_8012E204(TObj *o)
{
    unsigned short *q;

    switch (o->state) {
    case 0:
        D_800A603C = 5;
        D_800A603D = 100;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        FUN_8005a8a8(0x33, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 1:
        if (--o->w08 > 0)
            break;
        o->state = 15;
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
        if (D_8009CDD7 == 0xff) {
            D_800A603D = 0x20;
            D_800A6058 = 1;
            D_800A603C = 1;
            D_800A603E = 0;
            o->state = 5;
            break;
        }
        FUN_8005a9a4(0x33, 0);
        o->w08 = 250;
        o->state++;
        break;
    case 4:
        if (--o->w08 > 0)
            break;
        D_800A603C = 1;
        D_800A603D = 0x20;
        D_800A603E = 0;
        D_800A6058 = 1;
        o->state++;
        break;
    case 5:
        q = &D_800A6058;
        D_8009CF29 = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        *q = *q - 1;
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

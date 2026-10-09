// FUNC 80119788 384 X002
// MATCHING 80119788 384
#include "TOBJ.H"

extern unsigned char D_8009C93A, D_8009CDCB, D_8009D2B0;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned int D_8009C96C, D_8009C98C;
extern short D_800A60EA[], D_1F8001C6;
extern unsigned char D_8009C93FA[], D_8009C942A[];
extern void FUN_8005a9a4(int, int);

void func_80119788(TObj *o)
{
    char pad[16];
    switch (o->state) {
    case 0:
        if (D_8009C93A == 0)
            break;
        if (D_8009CDCB == 0 || D_8009CDCB == 0xff)
            break;
        if (D_8009C96C < D_8009C98C)
            break;
        D_8009C93FA[0] = 1;
        D_8009C942A[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        o->state++;
        break;
    case 1:
        o->w08 = 200;
        FUN_8005a9a4(0x27, 0);
        o->state++;
        break;
    case 2:
        if (--o->w08 > 0)
            break;
        D_8009C93FA[0] = 0;
        D_8009C942A[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        o->b0f = ((unsigned short *)o)[0x12];
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

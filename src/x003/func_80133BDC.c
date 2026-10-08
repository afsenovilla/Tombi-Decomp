// FUNC 80133bdc 216 X003
// MATCHING 80133bdc 216
#include "TOBJ.H"
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009C93E;
extern void FUN_8005a9a4(int, int);

void func_80133BDC(TObj *o)
{
    switch (o->state) {
    case 0:
        break;
    case 1:
        if (--o->w08 <= 0) o->state = 0xf;
        break;
    case 2:
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        o->w08 = 300;
        FUN_8005a9a4(0xab, 0);
        o->state = 1;
        break;
    case 0xf:
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

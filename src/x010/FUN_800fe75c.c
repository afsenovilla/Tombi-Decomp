// FUNC 800fe75c 304 X010
// MATCHING 800fe75c 304
#include "TOBJ.H"
extern unsigned char DAT_8009d2b1, DAT_8009cf06, DAT_8009c938, DAT_8009c942;
extern unsigned short DAT_8009c960, DAT_8009c962;
extern unsigned char *DAT_8009f0ec;
extern void FUN_8001f110(int);
extern void FUN_8001f2ec(int);

void FUN_800fe75c(TObj *o)
{
    unsigned char *p;
    switch (o->state) {
    case 0:
        if (o->b9e != 0 && (o->b9e == 4 || o->b9e == 7)) *DAT_8009f0ec = 1;
        p = &DAT_8009c938;
        if (*p != 1) {
            DAT_8009c942 = 1;
            *p = 1;
            FUN_8001f110(0);
            FUN_8001f2ec(3);
        }
        o->timer = 0xb4;
        *(signed char *)&o->b0f = -8;
        o->visible = 1;
        o->state++;
        break;
    case 1:
        if (--o->timer == 0) {
            DAT_8009d2b1 = 0;
            DAT_8009cf06 = 0;
            if (DAT_8009c960 != 10 || DAT_8009c962 != 0)
                DAT_8009c938 = 2;
            o->state++;
        }
        break;
    case 99:
        break;
    }
}

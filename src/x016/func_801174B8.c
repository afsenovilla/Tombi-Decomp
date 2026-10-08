// FUNC 801174b8 452 X016
// MATCHING 801174b8 452
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
extern TObj DAT_800a6038;
extern TObj *DAT_800a60c8;
extern unsigned char DAT_8009c93e, DAT_8009c93f, DAT_8009c942;
extern unsigned char DAT_8009ce06;
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void FUN_8005a8a8(int, int, int);

void func_801174B8(TObj *o)
{
    B12 v;

    switch (o->state) {
    case 0:
        DAT_800a6038.b04 = 5;
        DAT_800a6038.step = 0;
        DAT_800a6038.state = 0;
        DAT_8009c93f = 1;
        DAT_8009c942 = 1;
        o->state++;
        break;
    case 1:
        DAT_8009c93e = 1;
        o->state++;
        break;
    case 2:
        v = *(B12 *)&DAT_800a6038.a;
        DAT_800a60c8 = FUN_8002dcc8(4, 0, &v);
        o->state++;
        break;
    case 3:
        if (DAT_800a60c8->b04 != 2)
            break;
        DAT_800a60c8->b04 = 3;
        if (DAT_8009ce06 == 0) {
            FUN_8005a8a8(0x62, 0, 0);
            o->w08 = 200;
            o->state++;
        } else {
            o->state = 0xf;
        }
        break;
    case 4:
        if (--o->w08 > 0)
            break;
        o->state = 0xf;
        break;
    case 15:
        DAT_8009c93f = 0;
        DAT_8009c942 = 0;
        DAT_8009c93e = 0;
        DAT_800a6038.wb2 = 0;
        DAT_800a6038.b04 = 1;
        DAT_800a6038.step = 0;
        DAT_800a6038.state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

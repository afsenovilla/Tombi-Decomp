// FUNC 80121e50 400 X000
// MATCHING 80121e50 400
#include "TOBJ.H"
extern unsigned DAT_8009c96c;
extern unsigned char DAT_8009cdb4;
extern unsigned char DAT_800a60da;
extern unsigned char DAT_8009c93e;
extern unsigned char DAT_8009c93f;
extern unsigned char DAT_8009c942;
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);

void FUN_80121e50(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->b69 == 1 && DAT_800a60da == 1)
            o->step = o->step + 1;
        break;
    case 1:
        if (DAT_8009c96c < 100000) {
            if (DAT_8009cdb4 == 0) {
                FUN_8005a8a8(0x10, 0, 0);
                goto L;
            }
            o->step = o->step - 1;
            break;
        }
        if (DAT_8009cdb4 != 0xff) {
            FUN_8005a9a4(0x10, 0);
        L:
            o->step = o->step + 1;
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009c93e = 1;
            break;
        }
        o->b04 = 2;
        o->step = 0;
        break;
    case 2:
        o->timer = 300;
        o->step = o->step + 1;
        break;
    case 3:
        o->timer = o->timer - 1;
        if (o->timer == -1)
            o->step = o->step + 1;
        break;
    case 4:
        DAT_8009c93f = 0;
        DAT_8009c942 = 0;
        DAT_8009c93e = 0;
        o->step = 1;
        break;
    }
}

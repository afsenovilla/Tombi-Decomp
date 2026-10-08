// FUNC 8011b6e0 244 X000
#include "TOBJ.H"
extern unsigned char DAT_8009d005;
extern unsigned char DAT_8009c93f;
extern unsigned char DAT_8009c93e;
extern short DAT_1f80016a, DAT_1f800176, DAT_1f80016e, DAT_1f80017a;
extern TObj *FUN_8002dc50(int a, int b, int c, int d);
extern void FUN_800187e4(TObj *o);
extern void FUN_8011b5dc(TObj *o);

void FUN_8011b6e0(TObj *o)
{
    short v;
    if (DAT_8009d005 == 1) {
        switch (o->step) {
        case 0:
            o->step++;
            v = 0x50;
            if (DAT_1f80016a <= DAT_1f800176 + 0xa0) v = 0xf0;
            o->a.p.whole = v;
            v = 0x78;
            if (DAT_1f80016e <= DAT_1f80017a - 0x78) v = 0xd8;
            o->y.p.whole = v;
            o->d90 = (int)FUN_8002dc50(9, 0, o->a.p.whole, o->y.p.whole);
            break;
        case 1:
            if (((TObj *)o->d90)->b04 == 2) {
                ((TObj *)o->d90)->b04 = 3;
                DAT_8009c93f = 0;
                DAT_8009c93e = 0;
                FUN_800187e4(o);
            }
        }
    } else {
        FUN_8011b5dc(o);
    }
}

// FUNC 8011b5dc 260 X000
#include "TOBJ.H"
extern short DAT_1f80016a, DAT_1f800176, DAT_1f80016e, DAT_1f80017a;
extern short DAT_801386a8[];
extern short DAT_8009f3dc;
extern char DAT_8009c93f, DAT_8009c93e;
extern int FUN_8002dc50(int, int, int, int);
extern void FUN_800187e4(TObj *);

void FUN_8011b5dc(TObj *o)
{
    short v;
    if (o->step == 0) {
        o->step = 1;
        v = 0x50;
        if (DAT_1f80016a <= DAT_1f800176 + 0xa0)
            v = 0xf0;
        o->a.p.whole = v;
        v = 0x78;
        if (DAT_1f80016e <= DAT_1f80017a - 0x78)
            v = 0xd8;
        o->y.p.whole = v;
        o->d90 = FUN_8002dc50(9, DAT_801386a8[DAT_8009f3dc], o->a.p.whole, o->y.p.whole);
    } else if (o->step == 1) {
        if (((char *)o->d90)[4] == 2) {
            ((char *)o->d90)[4] = 3;
            DAT_8009c93f = 0;
            DAT_8009c93e = 0;
            FUN_800187e4(o);
        }
    }
}

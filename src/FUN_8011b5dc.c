// FUNC 8011b5dc 260 X000
// MATCHING 8011b5dc 260
#include "TOBJ.H"
extern short DAT_1f80016a, DAT_1f800176, DAT_1f80016e, DAT_1f80017a;
extern short DAT_801386a8[];
extern short DAT_8009f3dc;
extern char DAT_8009c93f, DAT_8009c93e;
extern int FUN_8002dc50(int, int, int, int);
extern void FUN_800187e4(TObj *);

void FUN_8011b5dc(TObj *o)
{
    unsigned char t = o->step;
    switch (t) {
    case 0:
        o->step = t + 1;
        if (DAT_1f800176 + 0xa0 >= DAT_1f80016a)
            o->a.p.whole = 0xf0;
        else
            o->a.p.whole = 0x50;
        if (DAT_1f80017a - 0x78 >= DAT_1f80016e)
            o->y.p.whole = 0xd8;
        else
            o->y.p.whole = 0x78;
        o->d90 = FUN_8002dc50(9, DAT_801386a8[DAT_8009f3dc], o->a.p.whole, o->y.p.whole);
        break;
    case 1:
        if (((char *)o->d90)[4] == 2) {
            ((char *)o->d90)[4] = 3;
            DAT_8009c93f = 0;
            DAT_8009c93e = 0;
            FUN_800187e4(o);
        }
        break;
    }
}

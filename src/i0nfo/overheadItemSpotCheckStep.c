// FUNC 800ee0f0 380 I0NFO
// MATCHING 800ee0f0 380
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void func_80026C50(int a, int b, int c);
extern void func_8004D620(int a, int b);
extern unsigned char D_8009D064[];
extern unsigned char D_8009D0AA, D_8009D0C8, D_8009D0D0, D_8009D0D1;
extern signed char D_800A603D[], D_800A603E[], D_800A60E4[];
extern short D_800EE994[];
extern void *D_800F53C0;

void overheadItemSpotCheckStep(TObj *o)
{
    int got;
    switch (o->state) {
    case 0:
        o->anim = D_800F53C0;
        func_8001FE6C(o);
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68 != 0) {
            switch (D_800EE994[o->b0c]) {
            case 0:
                got = D_8009D0AA != 0;
                break;
            case 1:
                got = D_8009D0C8 != 0;
                break;
            case 2:
                got = D_8009D0D0 != 0;
                break;
            case 3:
                got = D_8009D0D1 != 0;
                break;
            default:
                func_8004D620(0, 2);
                got = 0;
                break;
            }
            if (got) {
                D_8009D064[o->b0c] = 2;
                func_80026C50(7, 1, 1);
                o->step = 2;
                o->state = 0;
            }
            D_800A60E4[0] = 0;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
        }
        o->b68 = 0;
        break;
    }
}

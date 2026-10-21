// FUNC 800ee4bc 408 I0NFO2
// MATCHING 800ee4bc 408
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void func_80026C50(int item, int a, int b);
extern void func_8004D620(int a, int b);
extern unsigned char D_8009D064[];
extern unsigned char D_8009D0AA;
extern unsigned char D_8009D0C8;
extern unsigned char D_8009D0D0;
extern unsigned char D_8009D0D1;
extern short D_800FECF4[][4];
extern short D_800FED44[];
extern void *D_801073E4[];

void topdownItemSpotCheckStep(TObj *o)
{
    int ok;
    switch (o->state) {
    case 0:
        o->anim = D_801073E4[D_800FECF4[o->subtype][0]];
        func_8001FE6C(o);
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68 != 0) {
            switch (D_800FED44[o->b0c]) {
            case 0:
                ok = D_8009D0AA != 0;
                break;
            case 1:
                ok = D_8009D0C8 != 0;
                break;
            case 2:
                ok = D_8009D0D0 != 0;
                break;
            case 3:
                ok = D_8009D0D1 != 0;
                break;
            default:
                func_8004D620(0, 2);
                ok = 0;
                break;
            }
            if (ok) {
                D_8009D064[o->b0c] = 2;
                func_80026C50(D_800FECF4[o->subtype][1], 1, 1);
                o->step = 2;
                o->state = 0;
            }
        }
        o->b68 = 0;
        break;
    }
}

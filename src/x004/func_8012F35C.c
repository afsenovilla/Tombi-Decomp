// FUNC 8012f35c 404 X004
// MATCHING 8012f35c 404
#include "TOBJ.H"

extern unsigned char D_8009D2C3, D_8009CDDC;
extern unsigned char D_8009CF24[];
extern void func_8012ECA0(TObj *);
extern void func_8012EE14(TObj *);
extern void func_8012F10C(TObj *);
extern void FUN_80018980(TObj *);

void func_8012F35C(TObj *o)
{
    short i;
    switch (o->b04) {
    case 0:
        if (D_8009D2C3 & 8) {
            o->b04 = 3;
        } else if (D_8009CDDC == 0xff) {
            o->b04 = 3;
        } else {
            func_8012ECA0(o);
            o->step = 0;
            o->state = 0;
            o->b04++;
        }
        break;
    case 1:
        switch (o->step) {
        case 0:
            func_8012EE14(o);
            break;
        case 1:
            if (o->state == 0) {
                o->step = 0;
                o->state = 0;
                if (D_8009CDDC != 0xff) {
                    for (i = 0; i < 5; i++) {
                        if (D_8009CF24[i] == 0) return;
                    }
                    o->step = 2;
                }
            }
            break;
        case 2:
            func_8012F10C(o);
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}

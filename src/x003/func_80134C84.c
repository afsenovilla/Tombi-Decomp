// FUNC 80134c84 116 X003
// MATCHING 80134c84 116
#include "TOBJ.H"
extern unsigned char D_8009CDAB;
extern unsigned char D_8009CFCD;
extern void func_801346C8(TObj *);

void func_80134C84(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CDAB == 1 && D_8009CFCD == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_801346C8(o);
        break;
    }
}

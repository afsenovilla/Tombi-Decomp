// FUNC 8012de2c 116 X010
// MATCHING 8012de2c 116
#include "TOBJ.H"
extern unsigned char D_8009CDAB;
extern unsigned char D_8009CFCC;
extern void func_8012D8E4(TObj *);

void func_8012DE2C(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CDAB == 1 && D_8009CFCC == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_8012D8E4(o);
        break;
    }
}

// FUNC 8012ecd8 96 X010
// MATCHING 8012ecd8 96
#include "TOBJ.H"
extern unsigned char D_8009CE1F;
extern void func_8012EBA0(TObj *);

void func_8012ECD8(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CE1F == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_8012EBA0(o);
        break;
    }
}

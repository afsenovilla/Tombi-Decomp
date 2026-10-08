// FUNC 8013013c 116 X004
// MATCHING 8013013c 116
#include "TOBJ.H"
extern unsigned char D_8009CDAB;
extern unsigned char D_8009CFCB;
extern void func_8012FBF4(TObj *);

void func_8013013C(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CDAB == 1 && D_8009CFCB == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_8012FBF4(o);
        break;
    }
}

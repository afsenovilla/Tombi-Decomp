// FUNC 80118824 116 X016
// MATCHING 80118824 116
#include "gvars.h"
#include "TOBJ.H"
extern unsigned char D_8009D12F;

void func_80118824(TObj *o)
{
    if (GVARS.b40 != 0) {
        if (GVARS.b41 == 3 && D_8009D12F == 0) {
            o->step = 2;
            o->state = 0;
        }
        GVARS.b40 = 0;
    } else if (o->b68 != 0) {
        o->step = 1;
        o->state = 0;
    }
}

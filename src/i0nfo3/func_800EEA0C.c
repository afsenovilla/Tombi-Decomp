// FUNC 800eea0c 100 I0NFO3
// MATCHING 800eea0c 100
#include "TOBJ.H"
extern short func_800EE8CC(TObj *o);
extern unsigned char D_8009CDF3, D_8009CE21;

void func_800EEA0C(TObj *o)
{
    if (func_800EE8CC(o)) {
        o->step = 3;
        o->state = 0;
    } else if (D_8009CDF3 != 0xFF) {
        o->step = 2;
        o->state = 0;
    } else if (D_8009CE21 == 1) {
        o->step = 1;
        o->state = 0;
    }
}

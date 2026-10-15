// FUNC 800faf68 56 I0NFO2
// MATCHING 800faf68 56
#include "TOBJ.H"
extern void func_800FA84C(TObj *o);

void func_800FAF68(TObj *o)
{
    switch (o->step) {
    case 0:
        break;
    case 1:
        func_800FA84C(o);
        break;
    }
}

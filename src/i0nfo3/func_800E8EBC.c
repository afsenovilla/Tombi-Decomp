// FUNC 800e8ebc 180 I0NFO3
// MATCHING 800e8ebc 180
#include "TOBJ.H"
extern void func_80018838(TObj *o);
extern void func_800202B4(TObj *o);
extern unsigned char D_8009CE34, D_800A60A3;

void func_800E8EBC(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009CE34 != 0) o->b04 = 3;
        else o->b04++;
        break;
    case 1:
        if (D_800A60A3 == 0) func_800202B4(o);
        break;
    case 2:
        break;
    case 3:
        func_80018838(o);
        break;
    }
}

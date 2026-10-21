// FUNC 800e8f70 156 I0NFO3
// MATCHING 800e8f70 156
#include "TOBJ.H"
extern void func_80018838(TObj *o);
extern void func_800202B4(TObj *o);
extern unsigned char D_800A60A3;

void tunnelStaticObjHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->active = 2;
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

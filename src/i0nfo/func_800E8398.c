// FUNC 800e8398 136 I0NFO
// MATCHING 800e8398 136
#include "TOBJ.H"
extern void func_80020078(TObj *o, int a);
extern void func_800202B4(TObj *o);
extern void func_8001FEC0(TObj *o);

void func_800E8398(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->b0c == 0x23) {
            func_80020078(o, 0x6C);
        } else {
    case 1: case 2: case 3: case 4: case 5:
            func_800202B4(o);
        }
        if (o->visible != 0) {
            func_8001FEC0(o);
        }
        break;
    case 6:
        break;
    }
}

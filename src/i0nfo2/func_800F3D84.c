// FUNC 800f3d84 168 I0NFO2
// MATCHING 800f3d84 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800F3A6C(TObj *o);
extern void func_800F3C28(TObj *o);

void func_800F3D84(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800F3C28(o);
        break;
    case 1:
        func_800202B4(o);
        func_800F3A6C(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

// FUNC 800f05b0 168 I0NFO2
// MATCHING 800f05b0 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800F0270(TObj *o);
extern void func_800F0448(TObj *o);

void func_800F05B0(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800F0448(o);
        break;
    case 1:
        func_800202B4(o);
        func_800F0270(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

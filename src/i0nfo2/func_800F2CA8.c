// FUNC 800f2ca8 168 I0NFO2
// MATCHING 800f2ca8 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800F2994(TObj *o);
extern void func_800F2B50(TObj *o);

void func_800F2CA8(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800F2B50(o);
        break;
    case 1:
        func_800202B4(o);
        func_800F2994(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

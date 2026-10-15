// FUNC 800ede78 176 I0NFO3
// MATCHING 800ede78 176
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800EDB20(TObj *o);
extern void func_800EDCE4(TObj *o);

void func_800EDE78(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800EDCE4(o);
        break;
    case 1:
        *(signed char *)&o->b0f = -4;
        func_800202B4(o);
        func_800EDB20(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

// FUNC 800feb28 168 I0NFO2
// MATCHING 800feb28 168
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void func_800FDF24(TObj *o);
extern void func_800FE848(TObj *o);

void func_800FEB28(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800FDF24(o);
        o->b04++;
        break;
    case 1:
        func_800FE848(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}

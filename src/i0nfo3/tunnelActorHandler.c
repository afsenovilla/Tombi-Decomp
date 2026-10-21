// FUNC 800ede78 176 I0NFO3
// MATCHING 800ede78 176
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void tunnelActorStep(TObj *o);
extern void tunnelActorInit(TObj *o);

void tunnelActorHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        tunnelActorInit(o);
        break;
    case 1:
        *(signed char *)&o->b0f = -4;
        func_800202B4(o);
        tunnelActorStep(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

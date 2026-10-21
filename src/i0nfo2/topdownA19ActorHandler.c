// FUNC 800f6b4c 168 I0NFO2
// MATCHING 800f6b4c 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void topdownA19ActorStep(TObj *o);
extern void topdownA19ActorInit(TObj *o);

void topdownA19ActorHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        topdownA19ActorInit(o);
        break;
    case 1:
        func_800202B4(o);
        topdownA19ActorStep(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

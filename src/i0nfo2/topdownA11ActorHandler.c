// FUNC 800f2ca8 168 I0NFO2
// MATCHING 800f2ca8 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void topdownA11ActorStep(TObj *o);
extern void topdownA11ActorInit(TObj *o);

void topdownA11ActorHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        topdownA11ActorInit(o);
        break;
    case 1:
        func_800202B4(o);
        topdownA11ActorStep(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

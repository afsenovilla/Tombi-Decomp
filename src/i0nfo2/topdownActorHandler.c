// FUNC 800f05b0 168 I0NFO2
// MATCHING 800f05b0 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void topdownActorStep(TObj *o);
extern void topdownActorInit(TObj *o);

void topdownActorHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        topdownActorInit(o);
        break;
    case 1:
        func_800202B4(o);
        topdownActorStep(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

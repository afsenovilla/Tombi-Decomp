// FUNC 800f18c0 168 I0NFO2
// MATCHING 800f18c0 168
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void topdownA08ActorInit(TObj *o);

void topdownA08ActorHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        topdownA08ActorInit(o);
        break;
    case 1:
        func_800202B4(o);
        func_8001FEC0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

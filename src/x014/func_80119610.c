// FUNC 80119610 148 X014
// MATCHING 80119610 148
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

void func_80119610(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->active = 2;
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

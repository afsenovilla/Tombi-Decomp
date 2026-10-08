// FUNC 80118a78 200 X010
// MATCHING 80118a78 200
#include "TOBJ.H"

extern void ObjCullRegister(TObj *o);
extern void ObjFreeDup(TObj *o);
extern void func_801186E0(TObj *o);

void func_80118A78(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box3 = 0x20;
        o->box2 = 0x10;
        o->b69 = 0;
        o->b6a = 0;
        o->d30 = o->h->raw;
        o->d34 = o->y.raw;
        break;
    case 1:
        ObjCullRegister(o);
        func_801186E0(o);
        o->b69 = 0;
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

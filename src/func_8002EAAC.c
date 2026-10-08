// FUNC 8002eaac 136 MAIN0
// MATCHING 8002eaac 136
#include "TOBJ.H"
extern TObj *ObjAlloc();

void func_8002EAAC(int a, int b, int c)
{
    TObj *o = ObjAlloc();
    if (o != 0) {
        o->active = 1;
        o->type = 0x31;
        o->subtype = 0;
        o->b0c = 0;
        o->h->raw = a << 16;
        o->y.raw = b << 16;
        o->d->raw = c << 16;
        o->timer = 0x20;
    }
}

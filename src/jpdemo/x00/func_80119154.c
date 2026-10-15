// JPDEMO x00 80119154 136
// Demo version of PAL x014 80119610 (func_80119610.c):  Keeps its rotation (+0x84..+0x8c).
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

void func_80119610(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
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

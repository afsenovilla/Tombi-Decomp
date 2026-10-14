// JPDEMO slpm80165 8002EB24 164
// Demo version of PAL main0 8002FE78 (func_8002FE78.c): a stub object (clears its hit box, only registers for culling).
#include "TOBJ.H"
extern unsigned char PLAYER;
extern void FUN_8002ef20(TObj *);
extern void FUN_8002fc98(TObj *);
extern void ObjFree(TObj *);
extern void ObjCullRegister(TObj *);
void func_8002FE78(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->box0 = 0;
        o->box1 = 0;
        o->box2 = 0;
        o->box3 = 0;
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

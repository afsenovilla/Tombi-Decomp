// FUNC 8002fe78 168 MAIN0
// MATCHING 8002fe78 168
#include "TOBJ.H"
extern unsigned char PLAYER;
extern void FUN_8002ef20(TObj *);
extern void FUN_8002fc98(TObj *);
extern void ObjFree(TObj *);
void func_8002FE78(TObj *o)
{
    switch (o->b04) {
    case 0:
        FUN_8002ef20(o);
        break;
    case 1:
        if (PLAYER != 5)
            FUN_8002fc98(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

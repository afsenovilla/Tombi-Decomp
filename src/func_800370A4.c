// FUNC 800370a4 268 MAIN0
// MATCHING 800370a4 268
#include "plx.h"
#include "TOBJ.H"
typedef struct { char p[0xc]; unsigned char b; } G338;
extern G338 *D_8009C338;
extern unsigned char *D_8009C330;
void FUN_80035198(TObj *o);
void ObjCullRegister(TObj *o);
void FUN_80036f30(TObj *o);
void freeObjectLayer1(TObj *o);
void func_800370A4(TObj *o)
{
    if (PLX.b60ff) o->b04 = 2;
    switch (o->b04) {
    case 0:
        FUN_80035198(o);
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        FUN_80036f30(o);
        break;
    case 2:
        D_8009C338->b = 0;
        *D_8009C330 = 0;
        if ((signed char)--PLX.b611b < 0) PLX.b611b = 0;
        o->b04 = 3;
        break;
    case 3:
        freeObjectLayer1(o);
        break;
    }
}

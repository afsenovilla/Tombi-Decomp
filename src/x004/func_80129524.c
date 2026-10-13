// FUNC 80129524 168 X004
// MATCHING 80129524 168
#include "TOBJ.H"
extern void func_801293A0(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_80129250(TObj *);
extern void freeObjectLayer2(TObj *);

void func_80129524(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_801293A0(o);
        break;
    case 1:
        FUN_800202b4(o);
        func_80129250(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}

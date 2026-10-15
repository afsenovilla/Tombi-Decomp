// FUNC 801171a4 168 X005
// MATCHING 801171a4 168
#include "TOBJ.H"
extern void initBacoTownNpc(TObj *);
extern int FUN_800202b4(TObj *);
extern void updateBacoTownNpc(TObj *);
extern void freeObjectLayer2(TObj *);

void updateBacoTownNpc(TObj *o)
{
    switch (o->b04) {
    case 0:
        initBacoTownNpc(o);
        break;
    case 1:
        FUN_800202b4(o);
        updateBacoTownNpc(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}

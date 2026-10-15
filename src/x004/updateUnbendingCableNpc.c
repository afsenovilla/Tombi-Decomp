// FUNC 8012fb4c 168 X004
// MATCHING 8012fb4c 168
#include "TOBJ.H"
extern void initUnbendingCableNpc(TObj *);
extern int FUN_800202b4(TObj *);
extern void unbendingCableTalk(TObj *);
extern void freeObjectLayer2(TObj *);

void updateUnbendingCableNpc(TObj *o)
{
    switch (o->b04) {
    case 0:
        initUnbendingCableNpc(o);
        break;
    case 1:
        FUN_800202b4(o);
        unbendingCableTalk(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}

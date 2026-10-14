// FUNC 801170f4 184 X018
// MATCHING 801170f4 184
#include "TOBJ.H"
extern void func_80116F7C(TObj *);
extern void juiceManJoinFightTalk(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void updateJuiceManJoinFight(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80116F7C(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->step == 0) juiceManJoinFightTalk(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}

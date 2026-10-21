// FUNC 80138008 176 X000
// MATCHING 80138008 176
#include "TOBJ.H"
extern void spawnA00DarkCloakedFigure(TObj *);
extern short a00DarkCloakedFigureTalkCheck(TObj *);
extern void a00DarkCloakedFigureScene(TObj *);
extern void FUN_80018980(TObj *);

void updateDarkCloakedFigureEvent(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnA00DarkCloakedFigure(o);
        break;
    case 1:
        if (a00DarkCloakedFigureTalkCheck(o) != 0)
            a00DarkCloakedFigureScene(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}

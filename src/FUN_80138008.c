// FUNC 80138008 176 X000
// MATCHING 80138008 176
#include "TOBJ.H"
extern void FUN_80136a98(TObj *);
extern short FUN_80136bc0(TObj *);
extern void FUN_80136ce0(TObj *);
extern void FUN_80018980(TObj *);

void FUN_80138008(TObj *o)
{
    switch (o->b04) {
    case 0:
        FUN_80136a98(o);
        break;
    case 1:
        if (FUN_80136bc0(o) != 0)
            FUN_80136ce0(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}

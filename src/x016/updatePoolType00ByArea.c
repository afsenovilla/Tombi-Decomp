// FUNC 800e9e98 220 X016
// MATCHING 800e9e98 220
#include "TOBJ.H"
extern unsigned short DAT_8009c960;
extern void func_80117CAC(TObj *);
extern void FUN_80117f98(TObj *);
extern void FUN_80118568(TObj *);
extern void FUN_8011a0b4(TObj *);
extern void FUN_801169bc(TObj *);
extern void FUN_80115ec4(TObj *);
extern void FUN_801180ac(TObj *);
extern void FUN_80117928(TObj *);
extern void FUN_80115f24(TObj *);
extern void FUN_8011658c(TObj *);

void updatePoolType00ByArea(TObj *o)
{
    switch (DAT_8009c960) {
    case 0: func_80117CAC(o); break;
    case 1: FUN_80117f98(o); break;
    case 3: FUN_80118568(o); break;
    case 4: FUN_8011a0b4(o); break;
    case 6: FUN_801169bc(o); break;
    case 7: FUN_80115ec4(o); break;
    case 9: FUN_801180ac(o); break;
    case 10: FUN_80117928(o); break;
    case 12: FUN_80115f24(o); break;
    case 17: FUN_8011658c(o); break;
    }
}

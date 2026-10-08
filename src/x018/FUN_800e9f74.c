// FUNC 800e9f74 500 X018
// MATCHING 800e9f74 500
#include "TOBJ.H"
extern unsigned short DAT_8009c960;
extern void FUN_80118040(TObj *, short, short, short, int);
extern void FUN_8011832c(TObj *, short, short, short, int);
extern void FUN_801188fc(TObj *, short, short, short, int);
extern void FUN_8011a448(TObj *, short, short, short, int);
extern void FUN_80116d50(TObj *, short, short, short, int);
extern void FUN_80116258(TObj *, short, short, short, int);
extern void FUN_80118440(TObj *, short, short, short, int);
extern void FUN_80117cbc(TObj *, short, short, short, int);
extern void FUN_801162b8(TObj *, short, short, short, int);
extern void FUN_80116920(TObj *, short, short, short, int);

void FUN_800e9f74(TObj *o, short x, short y, short z)
{
    switch (DAT_8009c960) {
    case 0: FUN_80118040(o, x, y, z, 1); break;
    case 1: FUN_8011832c(o, x, y, z, 1); break;
    case 3: FUN_801188fc(o, x, y, z, 1); break;
    case 4: FUN_8011a448(o, x, y, z, 1); break;
    case 6: FUN_80116d50(o, x, y, z, 1); break;
    case 7: FUN_80116258(o, x, y, z, 1); break;
    case 9: FUN_80118440(o, x, y, z, 1); break;
    case 10: FUN_80117cbc(o, x, y, z, 1); break;
    case 12: FUN_801162b8(o, x, y, z, 1); break;
    case 17: FUN_80116920(o, x, y, z, 1); break;
    }
}

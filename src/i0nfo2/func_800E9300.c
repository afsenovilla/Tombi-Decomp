// FUNC 800e9300 88 I0NFO2
// MATCHING 800e9300 88
#include "TOBJ.H"
extern void func_8001FE94(TObj *o, int n);
extern void **D_800FEBD8[][3];

void func_800E9300(TObj *o, short sub, short i, short n)
{
    o->anim = D_800FEBD8[sub][0][i];
    func_8001FE94(o, n);
}

// FUNC 800ecbbc 80 I0NFO
// MATCHING 800ecbbc 80
#include "TOBJ.H"
extern void func_8001FE94(TObj *o, int n);
extern void **D_800EE914[][3];

void func_800ECBBC(TObj *o)
{
    o->anim = D_800EE914[o->subtype][0][o->w74];
    func_8001FE94(o, o->w76);
}

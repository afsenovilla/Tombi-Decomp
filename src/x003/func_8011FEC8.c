// FUNC 8011fec8 156 X003
// MATCHING 8011fec8 156
#include "TOBJ.H"
extern short D_1F80019E;
extern short func_8004461C(TObj *, TObj *);
extern int func_800428C0(TObj *, TObj *);

void func_8011FEC8(TObj *o, TObj *e)
{
    if (e->d94 == 0 && func_8004461C(o, e) >= 0) {
        func_800428C0(o, e);
        if (o->type != 1) {
            if (o->animFrame < 4) e->animFrame = o->animFrame & 1;
            else e->animFrame = 3;
        } else {
            e->animFrame = o->animFrame & 1;
        }
        D_1F80019E = 0;
    }
}

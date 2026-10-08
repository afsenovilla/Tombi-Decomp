// FUNC 8011fa6c 104 X003
// MATCHING 8011fa6c 104
#include "TOBJ.H"

extern unsigned char D_8009CDC3;
extern short FUN_8004461c(TObj *a, TObj *b);

void func_8011FA6C(TObj *o, TObj *p)
{
    if (FUN_8004461c(o, p) != -1) {
        if (D_8009CDC3 != 0) {
            p->b04 = 3;
        } else {
            p->state = 2;
        }
        p->active = 2;
    }
}

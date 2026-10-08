// FUNC 80045600 132 MAIN0
// MATCHING 80045600 132
#include "TOBJ.H"
typedef struct { TObj o; char p[0xe4 - 0xc0]; TObj *e4; } TObjX;
extern short func_8004306C(TObjX *o, TObj *p);
extern short D_1F80019E;

void func_80045600(TObjX *o, TObj *p)
{
    short r = func_8004306C(o, p);
    if (r != 0) {
        if (*(unsigned char *)&o->o.wac == 1 && r == 1) {
            p->active = 2;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            p->b69 = 0;
            o->e4 = p;
            *(unsigned char *)&o->o.wac = 2;
        }
        D_1F80019E = 0;
    }
}

// FUNC 8012d66c 212 X003
// MATCHING 8012d66c 212
#include "TOBJ.H"

extern short func_8004065C(TObj *, short, short, short);

int func_8012D66C(TObj *o)
{
    short d;

    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1))
        return 1;
    if (o->animFrame & 1)
        d = -0xe;
    else
        d = 0xe;
    if (func_8004065C(o, o->h->p.whole + d, o->y.p.whole, o->animFrame) == 0)
        return func_8004065C(o, o->h->p.whole + d, o->y.p.whole - 4, o->animFrame) != 0;
    return 1;
}

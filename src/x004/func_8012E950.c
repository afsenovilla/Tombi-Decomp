// FUNC 8012e950 204 X004
// MATCHING 8012e950 204
#include "gcd.h"
#include "TOBJ.H"
typedef struct { unsigned short anim, sub, b0c, x, y, z; } Spawn;
extern Spawn D_8013144C;
extern TObj *allocObjectLayer2(void);

void func_8012E950(TObj *o)
{
    TObj *p;

    if (GCD.bcf2d == 0) {
        p = allocObjectLayer2();
        if (p != 0) {
            p->active = 3;
            p->type = 0x32;
            p->animFrame = D_8013144C.anim;
            p->subtype = D_8013144C.sub;
            p->b0c = D_8013144C.b0c;
            p->a.p.whole = D_8013144C.x;
            p->y.p.whole = D_8013144C.y;
            p->b.p.whole = D_8013144C.z;
            p->b6b = 0;
            ((unsigned char*)&GCD.bcf29)[0] = 0;
        }
    }
    o->b04++;
    o->step = 0;
    o->state = 0;
}

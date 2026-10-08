// FUNC 8012e950 204 X004
// MATCHING 8012e950 204
#include "TOBJ.H"
typedef struct { unsigned short anim, sub, b0c, x, y, z; } Spawn;
extern unsigned char D_8009CF2D;
extern unsigned char D_8009CF29[];
extern Spawn D_8013144C;
extern TObj *FUN_800183b8(void);

void func_8012E950(TObj *o)
{
    TObj *p;

    if (D_8009CF2D == 0) {
        p = FUN_800183b8();
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
            D_8009CF29[0] = 0;
        }
    }
    o->b04++;
    o->step = 0;
    o->state = 0;
}

// FUNC 800edfb8 196 I0NFO
// MATCHING 800edfb8 196
#include "TOBJ.H"
extern TObj *func_800183B8(void);

void overheadSpawnPlayerShadow(TObj *o, char sub)
{
    TObj *n = func_800183B8();
    if (n != 0) {
        n->active = 2;
        n->type = 0x19;
        n->box0 = 1;
        n->box1 = 2;
        n->box2 = 1;
        n->box3 = 2;

        n->a.raw = o->a.raw;
        n->y.raw = o->y.raw;
        n->b.raw = o->b.raw;
        n->w1e = 0xF;
        n->subtype = sub;
        n->b0d = 0x80;
        n->b0a = 0;
        n->b0f = o->b0f + 1;
        n->_pad0e[0] = 1;
        n->d90 = (int)o;
        o->d94 = (int)n;

        n->b04 = 0;
        n->step = 0;
        n->state = 0;
    }
}

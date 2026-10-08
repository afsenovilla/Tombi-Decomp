// FUNC 8002e6c4 376 MAIN0
// MATCHING 8002e6c4 376
#include "TOBJ.H"
extern void *D_80012150[];
extern void AnimLoadDuration(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

void func_8002E6C4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b0a = 7;
        *(signed char *)&o->b0f = -20;
        o->w1e = 0x14;
        o->b0d = 0;
        o->d3c = ((struct { char pad[0x2d8]; int v; } *)0x1F800000)->v;
        o->anim = D_80012150[o->b0c];
        AnimLoadDuration(o);
        o->step = 0;
        o->timer = o->wb4;
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (--o->timer == 0) {
                o->timer = o->wb6;
                o->step++;
            }
            break;
        case 1:
            if (--o->timer == 0) {
                o->b04++;
            }
            o->visible = 1;
            ObjListPush_1F800220(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

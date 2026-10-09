// FUNC 80130114 388 X001
// MATCHING 80130114 388
#include "TOBJ.H"

extern void *D_8013E74C[];
extern void AnimLoadDuration(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);

void func_80130114(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->anim = D_8013E74C[0];
        AnimLoadDuration(o);
        o->velX = -0x200;
        o->velY = -0x300;
        o->active = 2;
        o->state++;
    case 1:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x30;
        if (o->velY > 0) o->state++;
        break;
    case 2:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x30;
        if (o->b69 || TileCollideAt(o, o->h->p.whole, o->y.p.whole + o->box3 - o->box2)) {
            o->velY = -0x300;
            o->state = 1;
        }
        break;
    }
    if (!o->visible) o->b04 = 3;
}

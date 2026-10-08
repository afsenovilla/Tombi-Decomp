// FUNC 80128b4c 248 X009
// MATCHING 80128b4c 248
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012B2E4[];
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void func_80128B4C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->anim = D_8012B2E4[o->subtype].anims[0x18];
        AnimLoadDuration(o);
        o->velX = -0x100;
        o->velY = 0;
        o->state++;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        if (o->visible == 0) o->state = 2;
        break;
    case 2:
        o->b04 = 2;
        break;
    }
}

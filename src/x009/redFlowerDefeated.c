// FUNC 80123ce8 316 X009
// MATCHING 80123ce8 316
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern char MOVE_TABLE_2[];
extern void *D_8012E9F8[];

void redFlowerDefeated(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b0b = 1;
        o->active = 2;
        o->b0f = 4;
        o->b0a = 2;
        o->velV = -0x400;
        o->movetab = MOVE_TABLE_2;
        o->d8c = 0;
        o->d88 = 0;
        o->d84 = 0;
        o->wac = 0;
        o->animFrame &= 1;
        o->state++;
        o->anim = D_8012E9F8[0];
        AnimLoadDuration(o);
        break;
    case 1:
        applyAnimVelocityX(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1) o->d8c = (o->d8c + 0x14) & 0xff;
    else o->d8c = (o->d8c - 0x14) & 0xff;
}

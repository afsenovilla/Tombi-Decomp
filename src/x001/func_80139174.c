// FUNC 80139174 760 X001
// MATCHING 80139174 760
#include "TOBJ.H"

extern void *D_8013FAC4[], *D_8013FAB0[], *D_8013FAC8[], *D_8013FAB4[], *D_8013FAC0[];
extern int AnimAdvance(TObj *o);
extern void AnimLoadDuration(TObj *o);

void func_80139174(TObj *o)
{
    switch (o->state) {
    case 0:
        *(signed char *)&o->b0f = -6;
        o->wac = 0xd;
        o->state++;
        o->animFrame = 1 - o->animFrame;
        o->anim = D_8013FAC4[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (AnimAdvance(o)) o->state++;
        break;
    case 2:
        *(signed char *)&o->b0f = -6;
        o->wac = 8;
        o->state++;
        o->animFrame = 1 - o->animFrame;
        o->anim = D_8013FAB0[0];
        AnimLoadDuration(o);
        break;
    case 3:
        if (AnimAdvance(o)) o->state++;
        if (o->animFrame == 0) o->h->raw += 0x4000;
        else o->h->raw -= 0x4000;
        break;
    case 4:
        o->wac = 0xe;
        o->state++;
        o->anim = D_8013FAC8[0];
        AnimLoadDuration(o);
        break;
    case 5:
        if (AnimAdvance(o)) o->state++;
        if (o->animFrame == 0) o->h->raw += 0x8000;
        else o->h->raw += -0x8000;
        break;
    case 6:
        o->wac = 9;
        o->state++;
        o->anim = D_8013FAB4[0];
        AnimLoadDuration(o);
        break;
    case 7:
        if (AnimAdvance(o)) {
            o->state++;
        } else if (o->animFrame == 0) {
            o->h->raw += 0x10000;
        } else {
            o->h->raw += -0x10000;
        }
        break;
    case 8:
        o->wac = 0xc;
        o->state++;
        o->anim = D_8013FAC0[0];
        AnimLoadDuration(o);
        break;
    case 9:
        AnimAdvance(o);
        if (!o->visible) {
            o->state++;
        } else {
            if (o->animFrame == 0) o->h->raw += 0x10000;
            else o->h->raw += -0x10000;
            o->y.raw += -0x28000;
        }
        break;
    case 10:
        o->state = 0;
        o->step++;
        break;
    }
}

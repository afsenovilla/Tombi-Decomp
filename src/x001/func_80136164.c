// FUNC 80136164 824 X001
// MATCHING 80136164 824
#include "TOBJ.H"
extern unsigned short D_8009C960, D_8009C962;
extern Fix16 *D_800A6078;
extern unsigned char D_8009C942[], D_8009C93F[], D_8009C93E[];
extern void *D_8013DE2C, *D_8013DDC8, *D_8013DE0C, *D_8013DDB0;
extern void func_80135B48(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);

void func_80136164(TObj *o)
{
    switch (o->state) {
    case 0:
        switch (o->subtype) {
        case 2:
        case 3:
            ((TObj *)o->d94)->b04 = 3;
            break;
        }
        func_80135B48(o);
        o->active = 4;
        if (D_8009C960 == 1 && D_8009C962 < 2) o->anim = D_8013DE2C;
        else o->anim = D_8013DDC8;
        AnimLoadDuration(o);
        o->animFrame = o->h->p.whole > D_800A6078->p.whole;
        o->timer = 0x3c;
        o->state = 1;
        break;
    case 1:
        if (--o->timer > 0) break;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009C93E[0] = 1;
        o->state++;
        break;
    case 2:
        if (AnimAdvance(o)) o->state++;
        break;
    case 3:
        o->active = 4;
        switch (D_8009C962) {
        case 0:
        case 1:
        case 4:
            o->animFrame = 0;
            break;
        case 2:
        case 3:
        case 5:
            o->animFrame = 1;
            break;
        }
        if (D_8009C960 == 1 && D_8009C962 < 2) o->anim = D_8013DE0C;
        else o->anim = D_8013DDB0;
        AnimLoadDuration(o);
        o->velX = (o->animFrame & 1) ? -0x200 : 0x200;
        o->velY = -0x300;
        o->state++;
    case 4:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0) o->state++;
        break;
    case 5:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + (o->box3 - o->box2))) o->state = 3;
        break;
    }
    if (!o->visible) {
        D_8009C942[0] = 0;
        D_8009C93F[0] = 0;
        D_8009C93E[0] = 0;
        o->b04 = 3;
        o->step = 0;
        o->state = 0;
    }
}

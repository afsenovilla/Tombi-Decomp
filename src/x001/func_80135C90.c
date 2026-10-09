// FUNC 80135c90 1236 X001
// MATCHING 80135c90 1236
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C93E, D_8009C942;
extern unsigned short D_8009C960[], D_8009C962[];
extern Fix16 *D_800A6078;
extern char D_80077CDC[];
extern void *D_8013DE0C, *D_8013DDB0, *D_8013DE2C, *D_8013DDC8;
extern void AnimLoadDuration(TObj *);
extern void func_80135B48(TObj *);
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);

void func_80135C90(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C93F = 1;
        D_8009C93E = 1;
        D_8009C942 = 1;
        o->animFrame = o->h->p.whole > D_800A6078->p.whole;
        switch (o->subtype) {
        case 2:
        case 3:
            ((TObj *)o->d94)->b04 = 3;
        }
        func_80135B48(o);
        o->active = 4;
        o->movetab = D_80077CDC;
        if (D_8009C960[0] == 1 && D_8009C962[0] < 2) o->anim = D_8013DE0C;
        else o->anim = D_8013DDB0;
        AnimLoadDuration(o);
        {
            short v = 0x120;
            o->b69 = 0;
            if (o->animFrame & 1) v = -0x120;
            o->velY = -0x100;
            o->timer = 0x3c;
            o->velX = v;
        }
        o->state = 1;
        break;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0) o->state++;
        break;
    case 2:
        if (--o->timer <= 0) o->state++;
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0x500) o->velY = 0x500;
        if (o->b69 | TileCollideAt(o, o->h->p.whole, o->y.p.whole + (o->box3 - o->box2))) {
            if (D_8009C960[0] == 1 && D_8009C962[0] < 2) o->anim = D_8013DE2C;
            else o->anim = D_8013DDC8;
            AnimLoadDuration(o);
            { int t = D_800A6078->p.whole < o->h->p.whole;
            o->state++;
            o->animFrame = t; }
        }
        break;
    case 4:
        if (AnimAdvance(o)) o->state++;
        break;
    case 5:
        o->active = 4;
        if (D_8009C960[0] == 1 && D_8009C962[0] < 2) o->anim = D_8013DE0C;
        else o->anim = D_8013DDB0;
        AnimLoadDuration(o);
        switch (D_8009C962[0]) {
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
        {
            short v = 0x200;
            if (o->animFrame & 1) v = -0x200;
            o->velX = v;
            o->velY = -0x300;
            o->state++;
        }
    case 6:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0) o->state++;
        break;
    case 7:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + (o->box3 - o->box2))) o->state = 5;
        break;
    }
    if (!o->visible) {
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_8009C93F = 0;
        o->b04 = 3;
        o->step = 0;
        o->state = 0;
    }
}

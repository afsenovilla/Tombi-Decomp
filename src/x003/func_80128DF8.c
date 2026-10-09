// FUNC 80128df8 424 X003
// MATCHING 80128df8 424
#include "TOBJ.H"

typedef struct { void **p; int a; int b; } T12;

extern T12 D_80135D84[];
extern int AnimAdvance(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern short TileCollideAt(TObj *, short, short);

void func_80128DF8(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->anim = D_80135D84[o->subtype].p[1];
        AnimLoadDuration(o);
        o->velX = -0x300;
        o->velY = 0x300;
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        if (!TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->velX = -0x80;
            o->velY = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->velX = -0x300;
            o->velY = 0x400;
            o->state = 1;
        }
        break;
    }
}

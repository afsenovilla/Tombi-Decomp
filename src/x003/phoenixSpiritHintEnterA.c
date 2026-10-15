// FUNC 80128a1c 492 X003
// MATCHING 80128a1c 492
#include "gcd.h"
#include "TOBJ.H"

typedef struct { int *p; int pad[2]; } E12;
extern E12 D_80135D84[];
void AnimLoadDuration(TObj *o);
void AnimAdvance(TObj *o);
short TileCollideAt(TObj *o, int x, int y);

void phoenixSpiritHintEnterA(TObj *o)
{
    unsigned char *c;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->anim = (void *)D_80135D84[o->subtype].p[1];
        AnimLoadDuration(o);
        o->velX = -0x200;
        o->velY = 0x800;
        o->state++;
    case 1:
        if (GCD.bceef) {
            o->anim = (void *)D_80135D84[o->subtype].p[1];
            AnimLoadDuration(o);
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        if (o->h->p.whole < 0x1f4) {
            o->h->p.whole = 0x1f4;
        }
        o->y.raw += o->velY << 8;
        if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10)) && o->h->p.whole < 0x1f5) {
            o->animFrame = 0;
            c = &GCD.bceef;
            *c = *c + 1;
            o->state++;
        }
        break;
    case 3:
        if (GCD.bceef == 4) {
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}

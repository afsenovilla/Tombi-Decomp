// FUNC 80129240 496 X003
// MATCHING 80129240 496
#include "TOBJ.H"

extern short D_80135D8E[];
extern int D_1F8002C8[];
extern unsigned char D_8009CEF0;
void AnimAdvance(TObj *o);
short MulNegSinScaled(int a, int b);

void func_80129240(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 0;
        o->d30 = o->h->p.whole;
        o->velX = -0x180;
        o->velY = 0x20;
        o->w1e = 1;
        o->d34 = o->y.p.whole;
        o->d84 = 0;
        o->subtype = 1;
        o->d3c = D_1F8002C8[D_80135D8E[0]];
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->d84 = (o->d84 + 4) & 0xfff;
        o->y.p.whole = MulNegSinScaled(*(short *)&o->d8c, 0x40) + o->d34;
        if (o->h->p.whole < 0x12fc) {
            o->h->p.whole = 0x12fc;
            o->velY = 0x40;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->y.raw += o->velY << 8;
        o->velY += 8;
        if (o->velY > 0x180) o->velY = 0x180;
        if (o->y.p.whole >= -0x961) {
            o->y.p.whole = -0x962;
            o->velY = 0;
            o->timer = 0x1e;
            o->state++;
        }
        break;
    case 3:
        if (--o->timer <= 0) {
            o->timer = 0x3c;
            o->state++;
        }
        break;
    case 4:
        if (--o->timer <= 0) {
            o->step = 0;
            o->state = 0;
            D_8009CEF0 = 1;
        }
        break;
    }
}

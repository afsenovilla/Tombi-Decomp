// FUNC 80129430 384 X003
// MATCHING 80129430 384
#include "TOBJ.H"

typedef struct { int *p; int pad[2]; } E12;
extern E12 D_80135D84[];
extern unsigned char D_8009C939[];
extern unsigned char D_8009CDC7;
extern unsigned short *D_800A6078;
extern short D_800A604E;
void AnimLoadDuration(TObj *o);
void AnimAdvance(TObj *o);
short MulNegSinScaled(int a, int b);

void func_80129430(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 0;
        o->velX = 0x80;
        o->velY = -0x10;
        o->d34 = o->y.raw;
        D_8009C939[0] = 1;
        o->anim = (void *)D_80135D84[o->subtype].p[5];
        AnimLoadDuration(o);
        o->state++;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->d84 = (o->d84 + 4) & 0xfff;
        o->y.raw = o->d34 + (MulNegSinScaled(*(short *)&o->d8c, 0x40) << 16);
        o->d34 += o->velY << 8;
        o->velY -= 0x10;
        if (o->velY < -0x180) {
            o->velY = -0x180;
        }
        D_800A6078[1] = o->h->p.whole;
        D_800A604E = o->y.p.whole - 0x10;
        if (o->visible == 0) {
            D_8009CDC7 = 2;
            o->b04 = 3;
        }
        break;
    }
}

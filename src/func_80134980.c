// FUNC 80134980 672 X000
// MATCHING 80134980 672
#include "TOBJ.H"
extern void *D_8013AC14, *D_8013AC44, *D_8013AC2C;
extern Fix16 *D_800A607C[], *D_800A6078;
extern unsigned char D_8009C93F[], D_8009C942[], D_8009C93E[], D_8009D00E[];
void AnimLoadDuration(TObj *o);
int AnimAdvance(TObj *o);
void func_80134980(TObj *o)
{
    switch (o->state) {
    case 0:
        o->anim = D_8013AC14;
        AnimLoadDuration(o);
        o->animFrame = 1;
        if (D_800A607C[0]->p.whole != 0) break;
        if ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x50) < 0xa0) {
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_8009C93E[0] = 1;
            o->state++;
        }
        break;
    case 1:
        o->anim = D_8013AC44;
        AnimLoadDuration(o);
        o->velX = 0xc0;
        o->animFrame = 0;
        o->velY = -0x480;
        o->state++;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0) {
            o->anim = D_8013AC2C;
            AnimLoadDuration(o);
            o->animFrame = o->h->p.whole >= 0x139;
            o->velH = (0x138 - o->h->p.whole) >> 6;
            o->velV = (-0x11c - o->y.p.whole) >> 6;
            o->velY = -0x200;
            o->timer = 0x40;
            o->state++;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->h->p.whole += o->velH;
        o->y.p.whole += o->velV;
        o->y.raw += o->velY << 8;
        o->velY += 0x10;
        if (--o->timer > 0) break;
        o->anim = D_8013AC14;
        AnimLoadDuration(o);
        o->animFrame = 0;
        o->subtype = 0;
        o->h->p.whole = 0x138;
        o->y.p.whole = -0x11c;
        D_8009D00E[0] = 1;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        break;
    }
}

// FUNC 80134980 672 X000
// MATCHING 80134980 672
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern void *NPC_CHARLES_ANIMS, *D_8013AC44, *D_8013AC2C;
extern Fix16 *D_800A607C[];
void AnimLoadDuration(TObj *o);
int AnimAdvance(TObj *o);
void npcCharlesHopToSpot(TObj *o)
{
    switch (o->state) {
    case 0:
        o->anim = NPC_CHARLES_ANIMS;
        AnimLoadDuration(o);
        o->animFrame = 1;
        if (D_800A607C[0]->p.whole != 0) break;
        if ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole + 0x50) < 0xa0) {
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            ((unsigned char*)&GVARS.b3e)[0] = 1;
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
        o->anim = NPC_CHARLES_ANIMS;
        AnimLoadDuration(o);
        o->animFrame = 0;
        o->subtype = 0;
        o->h->p.whole = 0x138;
        o->y.p.whole = -0x11c;
        ((unsigned char*)&GCD.bd00e)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        break;
    }
}

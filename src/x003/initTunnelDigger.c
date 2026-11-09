// FUNC 8012c68c 460 X003
// MATCHING 8012c68c 460
// VERSION SLPS_01144 x03 8012B860 472
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012C68C;

extern T8012C68C TUNNEL_DIGGER_DEFS[];
extern int SPR_DATA[];
extern void AnimJump(TObj *, int);
extern void AnimLoadDuration(TObj *);

void func_8012B860(TObj *o)
{
    o->box0 = TUNNEL_DIGGER_DEFS[o->subtype].b0;
    o->box1 = TUNNEL_DIGGER_DEFS[o->subtype].b1;
    o->box2 = TUNNEL_DIGGER_DEFS[o->subtype].b2;
    o->box3 = TUNNEL_DIGGER_DEFS[o->subtype].b3;
    o->w1e = TUNNEL_DIGGER_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[TUNNEL_DIGGER_DEFS[o->subtype].w6];
    if (((unsigned char*)&GCD.bcdc5)[0] != 0xff) {
        o->anim = TUNNEL_DIGGER_DEFS[o->subtype].anim[5];
        AnimJump(o, 0);
    } else {
        o->anim = TUNNEL_DIGGER_DEFS[o->subtype].anim[4];
        AnimJump(o, 0);
        o->h->p.whole -= 32;
    }
    o->animFrame = 1;
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = PLAYER.obj.b0f + 1;
    o->b04++;
}
#else
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012C68C;

extern T8012C68C TUNNEL_DIGGER_DEFS[];
extern int SPR_DATA[];
extern void AnimJump(TObj *, int);
extern void AnimLoadDuration(TObj *);

void initTunnelDigger(TObj *o)
{
    o->box0 = TUNNEL_DIGGER_DEFS[o->subtype].b0;
    o->box1 = TUNNEL_DIGGER_DEFS[o->subtype].b1;
    o->box2 = TUNNEL_DIGGER_DEFS[o->subtype].b2;
    o->box3 = TUNNEL_DIGGER_DEFS[o->subtype].b3;
    o->w1e = TUNNEL_DIGGER_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[TUNNEL_DIGGER_DEFS[o->subtype].w6];
    if (((unsigned char*)&GCD.bcdc5)[0] != 0xff) {
        o->anim = TUNNEL_DIGGER_DEFS[o->subtype].anim[5];
        AnimJump(o, 0);
    } else {
        o->anim = TUNNEL_DIGGER_DEFS[o->subtype].anim[4];
        AnimJump(o, 0);
        o->h->p.whole = 0xc4d;
    }
    o->animFrame = 1;
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = PLAYER.obj.b0f + 1;
    o->b04++;
}
#endif

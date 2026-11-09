// FUNC 80118388 372 X017
// MATCHING 80118388 372
#include "gcd.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80118388;

extern T80118388 RAFT_RETURN_NPC_DEFS[];
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);

void initRaftReturnNpc(TObj *o)
{
    o->box0 = RAFT_RETURN_NPC_DEFS[o->subtype].b0;
    o->box1 = RAFT_RETURN_NPC_DEFS[o->subtype].b1;
    o->box2 = RAFT_RETURN_NPC_DEFS[o->subtype].b2;
    o->box3 = RAFT_RETURN_NPC_DEFS[o->subtype].b3;
    o->w1e = RAFT_RETURN_NPC_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[RAFT_RETURN_NPC_DEFS[o->subtype].w6];
    o->anim = RAFT_RETURN_NPC_DEFS[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0f = 0;
    o->b0a = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
    if (((unsigned char*)&GCD.bce1a)[0]) o->step = 1;
}

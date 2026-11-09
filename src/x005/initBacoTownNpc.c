// FUNC 80117050 340 X005
// MATCHING 80117050 340
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80117050;

extern T80117050 BACO_TOWN_NPC_DEFS[];
extern int SPR_DATA[];

extern void AnimLoadDuration(TObj *);

void initBacoTownNpc(TObj *o)
{
    o->box0 = BACO_TOWN_NPC_DEFS[o->subtype].b0;
    o->box1 = BACO_TOWN_NPC_DEFS[o->subtype].b1;
    o->box2 = BACO_TOWN_NPC_DEFS[o->subtype].b2;
    o->box3 = BACO_TOWN_NPC_DEFS[o->subtype].b3;
    o->w1e = BACO_TOWN_NPC_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[BACO_TOWN_NPC_DEFS[o->subtype].w6];
    o->anim = BACO_TOWN_NPC_DEFS[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0f = 0;
    o->category |= 0x80;
    o->b04++;
}

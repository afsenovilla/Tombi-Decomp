// FUNC 8012f9f4 344 X004
// MATCHING 8012f9f4 344
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012F9F4;

extern T8012F9F4 UNBENDING_CABLE_NPC_DEFS[];
extern int SPR_DATA[];

extern void AnimLoadDuration(TObj *);

void initUnbendingCableNpc(TObj *o)
{
    o->box0 = UNBENDING_CABLE_NPC_DEFS[o->subtype].b0;
    o->box1 = UNBENDING_CABLE_NPC_DEFS[o->subtype].b1;
    o->box2 = UNBENDING_CABLE_NPC_DEFS[o->subtype].b2;
    o->box3 = UNBENDING_CABLE_NPC_DEFS[o->subtype].b3;
    o->w1e = UNBENDING_CABLE_NPC_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[UNBENDING_CABLE_NPC_DEFS[o->subtype].w6];
    o->anim = UNBENDING_CABLE_NPC_DEFS[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    *(signed char *)&o->b0f = -6;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
}

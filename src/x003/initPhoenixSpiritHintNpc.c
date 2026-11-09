// FUNC 80129a2c 440 X003
// MATCHING 80129a2c 440
#include "TOBJ.H"
#include "gamestate.h"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80129A2C;

extern T80129A2C PHOENIX_SPIRIT_HINT_NPC_DEFS[];
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);

void initPhoenixSpiritHintNpc(TObj *o)
{
    o->box0 = PHOENIX_SPIRIT_HINT_NPC_DEFS[o->subtype].b0;
    o->box1 = PHOENIX_SPIRIT_HINT_NPC_DEFS[o->subtype].b1;
    o->box2 = PHOENIX_SPIRIT_HINT_NPC_DEFS[o->subtype].b2;
    o->box3 = PHOENIX_SPIRIT_HINT_NPC_DEFS[o->subtype].b3;
    o->w1e = PHOENIX_SPIRIT_HINT_NPC_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[PHOENIX_SPIRIT_HINT_NPC_DEFS[o->subtype].w6];
    o->anim = PHOENIX_SPIRIT_HINT_NPC_DEFS[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0a = 0;
    if (((unsigned short*)&GAME.selectedSection)[0] == 2) {
        o->anim = PHOENIX_SPIRIT_HINT_NPC_DEFS[o->subtype].anim[0];
        AnimLoadDuration(o);
        o->active = 10;
        o->b0d = 1;
        o->w08 = 0x784e;
    }
    o->b68 = 0;
    o->b0f = 0;
    o->category |= 0x80;
    o->b04++;
}

// FUNC 8012a6d8 440 X003
// MATCHING 8012a6d8 440
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012A6D8;

extern T8012A6D8 MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[];
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);

void initMasterOfTheSkiesEventObj(TObj *o)
{
    o->box0 = MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[o->subtype].b0;
    o->box1 = MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[o->subtype].b1;
    o->box2 = MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[o->subtype].b2;
    o->box3 = MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[o->subtype].b3;
    o->w1e = MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[o->subtype].w6];
    if (((unsigned char*)&GCD.bcdc7)[0] != 0xff) o->anim = MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[o->subtype].anim[1];
    else o->anim = MASTER_OF_THE_SKIES_EVENT_OBJ_DEFS[o->subtype].anim[2];
    AnimLoadDuration(o);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = PLAYER.obj.b0f + 1;
    o->b04++;
}

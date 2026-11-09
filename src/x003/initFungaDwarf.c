// FUNC 8012b2e4 368 X003
// MATCHING 8012b2e4 368
#include "player.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012B2E4;

extern T8012B2E4 FUNGA_DWARF_DEFS[];
extern int SPR_DATA[];
extern void AnimJump(TObj *, int);
extern void AnimLoadDuration(TObj *);

void initFungaDwarf(TObj *o)
{
    o->box0 = FUNGA_DWARF_DEFS[o->subtype].b0;
    o->box1 = FUNGA_DWARF_DEFS[o->subtype].b1;
    o->box2 = FUNGA_DWARF_DEFS[o->subtype].b2;
    o->box3 = FUNGA_DWARF_DEFS[o->subtype].b3;
    o->w1e = FUNGA_DWARF_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[FUNGA_DWARF_DEFS[o->subtype].w6];
    AnimLoadDuration(o);
    o->anim = FUNGA_DWARF_DEFS[o->subtype].anim[4];
    AnimJump(o, 0);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = PLAYER.obj.b0f + 1;
    o->b04++;
}

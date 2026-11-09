// FUNC 8011977c 452 X011
// MATCHING 8011977c 452
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8011977C;

extern T8011977C FACTORY_NPC_DEFS[];
extern int SPR_DATA[];
extern short FUN_8005e420(int, int);
extern void AnimLoadDuration(TObj *);

void initFactoryNpc(TObj *o)
{
    o->box0 = FACTORY_NPC_DEFS[o->subtype].b0;
    o->box1 = FACTORY_NPC_DEFS[o->subtype].b1;
    o->box2 = FACTORY_NPC_DEFS[o->subtype].b2;
    o->box3 = FACTORY_NPC_DEFS[o->subtype].b3;
    o->w1e = FACTORY_NPC_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[FACTORY_NPC_DEFS[o->subtype].w6];
    o->anim = FACTORY_NPC_DEFS[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->b0d = 1;
    o->d8c = 0;
    switch (o->subtype) {
    case 1:
        o->w08 = FUN_8005e420(0xc0, 0x1e2);
        break;
    case 0:
    case 3:
        o->w08 = FUN_8005e420(0xc0, 0x1e0);
        break;
    case 2:
        o->w08 = FUN_8005e420(0xc0, 0x1e0);
        break;
    }
    *(signed char *)&o->b0f = -10;
    o->b0a = 0;
    o->category |= 0x80;
    o->b04++;
}

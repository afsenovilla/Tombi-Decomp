// FUNC 8011145c 308 X016
// MATCHING 8011145c 308
#include "TOBJ.H"
typedef struct { unsigned short a; short p0; short b; short p1; int *c; } E8;
extern E8 PLAYER_POWER_AURA_DEFS[];
extern int SPR_DATA[];
extern void FUN_8001fe6c(TObj *o);

void playerPowerAuraInit(TObj *o)
{
    o->box0 = 0x20;
    o->box1 = 0x40;
    o->box2 = 0x1e;
    o->box3 = 0x3c;
    o->w1e = PLAYER_POWER_AURA_DEFS[o->subtype].a;
    o->d3c = SPR_DATA[PLAYER_POWER_AURA_DEFS[o->subtype].b];
    o->anim = (void *)PLAYER_POWER_AURA_DEFS[o->subtype].c[o->b0c];
    FUN_8001fe6c(o);
    o->b0a = 13;
    o->b0d = 0x80;
    o->category |= 0x80;
    o->d8c = 0;
    o->b6b = 0;
    *(signed char *)&o->b0f = -7;
    if (o->subtype == 2) {
        o->ba5 = 0;
        o->active = 2;
    } else {
        o->ba5 = 1;
        o->w98 = 2;
    }
    o->b04 = o->b04 + 1;
}

// FUNC 801193d0 808 X003
// MATCHING 801193d0 808
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"

typedef struct { short vx, vy, vz, pad; } SV;
typedef struct { char p[0xb4]; SV v[4]; } OX;

extern short TRANSLUCENT_SPRITE_DRIFT_QUAD[];
extern short SCREEN_ORIGIN_X[];
extern unsigned int FUN_8001f9e0(void);

#define V(o) (((OX *)(o))->v)

#define INITV(o) { V(o)[0].vx = TRANSLUCENT_SPRITE_DRIFT_QUAD[0]; V(o)[0].vy = TRANSLUCENT_SPRITE_DRIFT_QUAD[1]; V(o)[0].vz = 0; V(o)[1].vx = TRANSLUCENT_SPRITE_DRIFT_QUAD[2]; V(o)[1].vy = TRANSLUCENT_SPRITE_DRIFT_QUAD[3]; V(o)[1].vz = 0; V(o)[2].vx = TRANSLUCENT_SPRITE_DRIFT_QUAD[4]; V(o)[2].vy = TRANSLUCENT_SPRITE_DRIFT_QUAD[5]; V(o)[2].vz = 0; V(o)[3].vx = TRANSLUCENT_SPRITE_DRIFT_QUAD[6]; V(o)[3].vy = TRANSLUCENT_SPRITE_DRIFT_QUAD[7]; V(o)[3].vz = 0; }

void translucentSpriteDriftB(TObj *o)
{
    int k;

    switch (o->step) {
    case 0:
        o->step = 2;
        o->velX = 0x100;
        o->timer = 0x168;
        INITV(o);
        o->a.raw = (SCREEN_ORIGIN_X[0] + o->subtype * 52) << 16;
        k = 0x60;
        if (((unsigned short*)&GAME.selectedSection)[0] == 3)
            o->y.raw = (-0x272 - (FUN_8001f9e0() & 0xf) * 42) << 16;
        else
            o->y.raw = ((*(short*)&SPAD.s186) - ((FUN_8001f9e0() & 7) * 32 + k)) << 16;
        o->velH = o->subtype * 24;
        break;
    case 1:
        o->step++;
        o->velX = 0x100;
        o->timer = 0x168;
        INITV(o);
        o->a.raw = (SCREEN_ORIGIN_X[0] - 0xa0) << 16;
        k = 0x18;
        if (((unsigned short*)&GAME.selectedSection)[0] == 3)
            o->y.raw = (-0x272 - (FUN_8001f9e0() & 0xf) * 42) << 16;
        else
            o->y.raw = ((*(short*)&SPAD.s186) - ((FUN_8001f9e0() & 7) * 32 + k)) << 16;
        o->velH = o->subtype * 24;
        break;
    case 2:
        o->h->raw += 0x10000 + (o->velH << 8);
        o->y.raw -= 0x1000;
        if (o->timer != 0) {
            V(o)[0].vx += o->velX >> 8;
            V(o)[1].vx += o->velX >> 8;
            o->timer--;
        }
        if (o->h->p.whole + V(o)[2].vx > SCREEN_ORIGIN_X[0] + 0x26c)
            o->step = 1;
        break;
    }
}

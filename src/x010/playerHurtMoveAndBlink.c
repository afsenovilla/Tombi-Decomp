// FUNC 800fe8f8 148 X010
// MATCHING 800fe8f8 148
#include "gvars.h"
#include "TOBJ.H"
typedef struct { char pad[8]; unsigned char b8; } P;
extern P *PLAYER_EXT;

void playerHurtMoveAndBlink(TObj *o)
{
    o->h->raw += GVARS.s44 << 8;
    o->y.raw += ((short*)&GVARS.s46)[0] << 8;
    if (PLAYER_EXT->b8 == 0) {
        o->visible = *(unsigned char *)0x1F8001F8 & 1;
    }
    o->h->raw += o->velX << 8;
    o->y.raw += o->velY << 8;
}

// VERSION SLPM_80165 x00 80106898 148
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of updatePlayerHealth (src/updatePlayerHealth.c): takes one point per call while health is
// above 0; at 0 one flag and sound command 0x2A instead of the jingle.
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char pad[8]; unsigned char b8; } P;
extern P *PLAYER_EXT;
extern void func_8001E3D8(int);

void updatePlayerHealth(TObj *o)
{
    if (*(unsigned char *)0x1F8001A4 == 0 && o->w98 > 0) {
        PLAYER_EXT->b8 = 1;
        o->w98--;
        ((unsigned char*)&GAME.playerHealth)[0] = o->w98;
        if (o->w98 <= 0) {
            GVARS.hurtState = 1;
            func_8001E3D8(0x2a);
        }
        o->w9a = o->w98;
    }
}
#endif

// FUNC 800fc104 136 X005
// MATCHING 800fc104 136
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char pad[8]; unsigned char b8; } P;
extern P *PLAYER_EXT;
extern void stopBgm(int);
extern void playJingle(int);

void updatePlayerHealth(TObj *o)
{
    char pad;
    if (*(unsigned char *)0x1F8001A4 == 0 && o->w9a != o->w98) {
        o->w9a = o->w98;
        PLAYER_EXT->b8 = 1;
        ((unsigned char*)&GAME.playerHealth)[0] = o->w98;
        if (o->w98 <= 0) {
            GVARS.b42 = 1;
            GVARS.b38 = 1;
            stopBgm(0);
            playJingle(3);
        }
    }
}

// FUNC 800fc104 136 X011
// MATCHING 800fc104 136
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char pad[8]; unsigned char b8; } P;
extern P *D_8009C330;
extern unsigned char D_8009C942;
extern unsigned char D_8009C938;
extern void stopBgm(int);
extern void playJingle(int);

void func_800FC104(TObj *o)
{
    char pad;
    if (*(unsigned char *)0x1F8001A4 == 0 && o->w9a != o->w98) {
        o->w9a = o->w98;
        D_8009C330->b8 = 1;
        ((unsigned char*)&GAME.playerHealth)[0] = o->w98;
        if (o->w98 <= 0) {
            D_8009C942 = 1;
            D_8009C938 = 1;
            stopBgm(0);
            playJingle(3);
        }
    }
}

// FUNC 800e9e68 424 I0NFO3
// MATCHING 800e9e68 424
#include "TOBJ.H"
#include "raw7.h"
extern int func_8001FEC0(TObj *o);
extern void tunnelPlayerSetMoveAnim(TObj *o);
extern void func_8001E4F0(int s);
extern void func_8001F110(int a);
extern TObj *PLAYER_EXT;
extern unsigned char GAME_FADE_SCREEN_CONTROL[];
extern unsigned char GAME_PLAYER_MODE;

void tunnelPlayerDropExitStep(TObj *o)
{
    void *p;
    switch (o->state) {
    case 0:
        PLAYER_EXT->timer = 0;
        PLAYER_EXT->animFrame = 0xFF;
        tunnelPlayerSetMoveAnim(o);
        o->timer = 0x1E;
        o->d8c = 0;
        S16(o, 0xE0) = 0;
        o->w22 = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        GAME_PLAYER_MODE = 3;
        o->d8c = 0;
        func_8001F110(1);
        o->state++;
        break;
    case 1:
        if (o->substep != 0 && (*(unsigned short *)0x1F8001F8 & 3) == 0) {
            func_8001E4F0(0x16);
        }
        func_8001FEC0(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        if (--o->timer <= 0) {
            o->timer = 0x3C;
            GAME_FADE_SCREEN_CONTROL[0] = 3;
            o->state++;
        }
        break;
    case 2:
        func_8001FEC0(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        if (o->timer != 0) {
            if (--o->timer <= 0) {
                p = *(void **)0x1F8001D4;
                S16(p, 0x4C) = 7;
                S16(p, 0x4E) = 0;
            }
        }
        break;
    }
}

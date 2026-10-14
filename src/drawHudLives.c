// FUNC 800587a8 388 MAIN0
// MATCHING 800587a8 388
#include "gamestate.h"
typedef struct { char p[0x24]; unsigned short **tbl; char q[0x30 - 0x28]; unsigned short *dot; } O;
void drawSpriteFrame(O *o, short x, short y, int k);
void AddDrawMode(int, int);
void drawHudLives(O *o, short x, short y)
{
    unsigned char *g = &GAME.playerLives;
    unsigned short *t;
    if (*g - 1 >= 10) {
        t = o->tbl[(*g - 1) / 10];
        drawSpriteFrame(o, x - 5, y, *t);
        drawSpriteFrame(o, x + 5, y, *o->tbl[(*g - 1) % 10]);
    } else {
        t = o->tbl[*g - 1];
        drawSpriteFrame(o, x, y, *t);
    }
    drawSpriteFrame(o, x, y - 4, *o->dot);
    AddDrawMode(0x15, 1);
}

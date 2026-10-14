// FUNC 8011cd78 376 X006
// MATCHING 8011cd78 376
#include "gamestate.h"
extern unsigned short *D_80012100[];
extern unsigned short D_800124F8;
extern void drawSpriteFrame(void *, int, int, int);
extern void AddDrawMode(int, int);

void func_8011CD78(void *o, short x, short y)
{
    int n;
    if (((unsigned char*)&GAME.playerLives)[0] - 1 >= 10) {
        n = *D_80012100[(((unsigned char*)&GAME.playerLives)[0] - 1) / 10];
        drawSpriteFrame(o, x - 5, y, n);
        n = *D_80012100[(((unsigned char*)&GAME.playerLives)[0] - 1) % 10];
        drawSpriteFrame(o, x + 5, y, n);
    } else {
        n = *D_80012100[((unsigned char*)&GAME.playerLives)[0] - 1];
        drawSpriteFrame(o, x, y, n);
    }

    drawSpriteFrame(o, x, y - 4, D_800124F8);
    AddDrawMode(0x15, 1);
}

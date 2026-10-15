// FUNC 801179b4 408 X013
// MATCHING 801179b4 408
#include "gamestate.h"
typedef struct { short a, b, c; } R6;
extern R6 D_80118210[];
extern R6 D_80118224[];
extern R6 D_8011825C[];
extern R6 D_801182A4[];
void drawStatsLabelGlyph(R6 *p);
void drawStatsNumber(int a, int b, int c);
void drawStatsPlayTime(int a, int b, int c, int d);
int GetGraphType(void);
void AddDrawMode(int a, int b);

void drawGameStats(int a, int b, int c, int d)
{
    R6 *p;

    for (p = D_80118210; p->a != -1;) {
        drawStatsLabelGlyph(p++);
    }
    for (p = D_80118224; p->a != -1;) {
        drawStatsLabelGlyph(p++);
    }
    for (p = D_8011825C; p->a != -1;) {
        drawStatsLabelGlyph(p++);
    }
    for (p = D_801182A4; p->a != -1;) {
        drawStatsLabelGlyph(p++);
    }
    drawStatsNumber((*(int*)&GAME.playerAP), 0xc0, 0x3c);
    drawStatsPlayTime(a, b, 0xc0, 0x5c);
    drawStatsNumber(c, 0xc0, 0x7c);
    drawStatsNumber(d, 0xc0, 0x9c);
    if (GetGraphType() == 1) {
        AddDrawMode(0xe, 5);
    } else {
        GetGraphType();
        AddDrawMode(0xe, 5);
    }
}

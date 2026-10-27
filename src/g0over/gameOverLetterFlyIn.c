// FUNC 800e82f8 268 G0OVER
// MATCHING 800e82f8 268
#include "TOBJ.H"
extern short GAMEOVER_LETTER_TARGET_X[];
extern short GAMEOVER_LETTER_TARGET_Y[];

void gameOverLetterFlyIn(TObj *o)
{
    switch (o->state) {
    case 200: break;
    case 0:
        o->d30 = ((GAMEOVER_LETTER_TARGET_X[o->type] - o->a.p.whole) << 16) / 24;
        o->d34 = ((GAMEOVER_LETTER_TARGET_Y[o->type] - o->y.p.whole) << 16) / 24;
        o->velY = 0x200;
        o->velV = -0x1600;
        o->state++;
        break;
    case 1:
        o->a.raw += o->d30;
        o->y.raw += o->d34;
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        break;
    }
}

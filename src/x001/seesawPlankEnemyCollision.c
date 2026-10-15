// FUNC 80127fdc 120 X001
// MATCHING 80127fdc 120
#include "TOBJ.H"
extern void collideEnemyWithTiltedBar(TObj *, TObj *, int, int);

void seesawPlankEnemyCollision(TObj *a, TObj *b)
{
    if (b->subtype == 0) {
        collideEnemyWithTiltedBar(a, b, 0, b->d8c);
    } else {
        collideEnemyWithTiltedBar(a, b, b->subtype, b->d8c);
        collideEnemyWithTiltedBar(a, b, 2, (b->d8c + 0x400) & 0xfff);
    }
}

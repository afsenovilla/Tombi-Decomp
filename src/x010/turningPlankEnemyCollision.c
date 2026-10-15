// FUNC 80121994 172 X010
// MATCHING 80121994 172
#include "TOBJ.H"
extern void collideEnemyWithTiltedBar(TObj *, TObj *, int, int);

void turningPlankEnemyCollision(TObj *o, TObj *p)
{
    switch (p->subtype) {
    case 0:
        collideEnemyWithTiltedBar(o, p, 0, p->d30);
        return;
    case 1:
    case 2:
        collideEnemyWithTiltedBar(o, p, p->subtype, p->d30);
        collideEnemyWithTiltedBar(o, p, 2, (p->d30 + 0x400) & 0xfff);
        return;
    }
    collideEnemyWithTiltedBar(o, p, 2, p->d30);
}

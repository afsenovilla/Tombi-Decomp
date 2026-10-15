// FUNC 8011fa74 116 X004
// MATCHING 8011fa74 116
#include "TOBJ.H"
extern short D_1F80019E;
extern short pushObjectOutOfObjectBoxB(TObj *, TObj *);

void shuttlePlatformEnemyCollision(TObj *o, TObj *e)
{
    if (pushObjectOutOfObjectBoxB(o, e)) {
        D_1F80019E = 0;
        o->h->raw += e->velH << 8;
        o->y.raw += e->velV << 8;
    }
}

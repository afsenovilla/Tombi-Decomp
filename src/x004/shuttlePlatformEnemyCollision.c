// FUNC 8011fa74 116 X004
// MATCHING 8011fa74 116
#include "TOBJ.H"
extern short LIST_COUNTER;
extern short pushObjectOutOfObjectBoxB(TObj *, TObj *);

void shuttlePlatformEnemyCollision(TObj *o, TObj *e)
{
    if (pushObjectOutOfObjectBoxB(o, e)) {
        LIST_COUNTER = 0;
        o->h->raw += e->velH << 8;
        o->y.raw += e->velV << 8;
    }
}

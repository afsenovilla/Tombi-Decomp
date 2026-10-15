// FUNC 80125074 48 X001
// MATCHING 80125074 48
#include "TOBJ.H"

extern int playerPushOutOfObjectBox(TObj *a, TObj *b);

void solidBlockPlayerCollision(TObj *a, TObj *b)
{
    if (b->subtype == 0)
        playerPushOutOfObjectBox(a, b);
}

// FUNC 8011dee8 52 X010
// MATCHING 8011dee8 52
#include "gvars.h"
extern short playerPushOutOfObjectBox();

void collapsingBridgePlayerCollision(void *o)
{
    if (playerPushOutOfObjectBox(o)) GVARS.b3f = 1;
}

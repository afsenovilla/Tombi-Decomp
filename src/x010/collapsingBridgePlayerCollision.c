// FUNC 8011dee8 52 X010
// MATCHING 8011dee8 52
#include "gvars.h"
extern short func_8004306C();

void collapsingBridgePlayerCollision(void *o)
{
    if (func_8004306C(o)) GVARS.b3f = 1;
}

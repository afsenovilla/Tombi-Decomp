// FUNC 8003ab9c 36 MAIN0
// MATCHING 8003ab9c 36
#include "player.h"
#include "TOBJ.H"
extern char *SCRIPT_CTX;
void scriptOpGetPlayerHitFlag(void)
{
    char *p = SCRIPT_CTX;
    *(int *)(p + 0x1190) = PLAYER.obj.b9e;
    *(short *)(p + 0x8a) += 1;
}

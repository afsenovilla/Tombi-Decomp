// FUNC 8003ab78 36 MAIN0
// MATCHING 8003ab78 36
#include "player.h"
extern unsigned char *SCRIPT_CTX;
void scriptOpGetPlayerTouchFlag(void)
{
    unsigned char *p = SCRIPT_CTX;
    *(int *)(p + 0x1190) = PLAYER.obj.b69;
    (*(short *)(p + 0x8a))++;
}

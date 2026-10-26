// FUNC 8001ca00 88 MAIN0
// MATCHING 8001ca00 88
// Ported from psx_tomba (gamestate.c, openMenuScreenEx); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void openMenuScreenEx(s16 arg0, s16 arg1, s16 arg2)
{
    Task* p = CURRENT_TASK;

    D_800A3952 = arg0;
    D_800A3954 = arg1;
    D_800A3956 = arg2;
    MENU_STATE.unk0 = 0;
    p->state2 = 3;
    p->step.value = 0;
    playSFXWithNote(10, 10);
}

// FUNC 8001ca00 88 MAIN0
// MATCHING 8001ca00 88
// Ported from psx_tomba (gamestate.c, openMenuScreenEx); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x10];
    s16 unk12;
    s16 unk14;
    s16 unk16;
} unk_800A3940;
#define MENU_STATE ((unk_800A3940*)D_800A3940)

void openMenuScreenEx(s16 arg0, s16 arg1, s16 arg2)
{
    Task* p = CURRENT_TASK;

    D_800A3952 = arg0;
    D_800A3954 = arg1;
    D_800A3956 = arg2;
    D_800A3940[0] = 0;
    p->state2 = 3;
    p->step.value = 0;
    playSFXWithNote(10, 10);
}

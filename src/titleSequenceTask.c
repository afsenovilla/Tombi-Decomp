// FUNC 8004dd8c 372 MAIN0
// MATCHING 8004dd8c 372
// Ported from psx_tomba (boot.c, titleSequenceTask); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
typedef struct {
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    s16 w;
    s16 h;
    s16 tpage;
    s16 clutX;
    s16 clutY;
} unk_80076E40;

extern unk_80076E40 D_80076E40[];

unk_80076E40 D_80076E40[4] = {
    { 0, 0xC0, 0, 0xC0, 0x100, 0x28, 0x80, 0x80, 0x1E0 },
    { 0x100, 0xC0, 0, 0xC0, 0x100, 0x40, 0x82, 0x80, 0x1E0 },
    { 0x200, 0xC0, 0, 0xC0, 0x80, 0x28, 0x84, 0x80, 0x1E0 },
    { -1, 0, 0, 0, 0, 0, 0, 0, 0 }
};

u_short D_80076E80[4] = { 0xE4, 0xAC, 0x74, 0 };

void titleSequenceTask(void)
{
    scratchpad* scratch = PSX_SCRATCH;
    Task* task = *(Task**)scratch->currentTask;

    u32 sp10[2];
    u16 state;

    *(s8* )&scratch->unk1D1 = 1;
    *(s8* )&scratch->unk1D0 = 0;
    sp10[0] = 0;
    scratch->currentTask->titleScreenSelectedOption = 0;
    while(true) {
        *(u16* )0x1F8001F8 = *(u16* )(D_1F8000F8+0x100) + 1;
        asm("");
        func_8001F850();
        task = CURRENT_TASK;
        state = task->state0;
        if ((state >= 3U) && (JOYPAD_STATE & (JOY_CROSS | JOY_START)) && (state != 4)) {
            task->state0 = 4U;
            task->state1 = 0;
            task->state2 = 0;
            sp10[0] = 1;
            stopBgm(0);
            if (MOVIE_PLAY_STATE != MOVIE_STATE_IDLE) {
                MOVIE_SKIP_REQUEST = 1;
            }
        }
        switch ((u16)(CURRENT_TASK)->state0) {
            case 0:
                func_8004DF00(sp10);
                break;
            case 1:
                bootLoadMovieResources();
                break;
            case 2:
                func_8004E914();
                break;
            case 3:
                bootPlayIntroMovie();
                break;
            case 4:
                loopTitleScreen(sp10);
                break;
        }
        sleepTask(1);
    };
}

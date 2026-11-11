// VERSION PCPX_96084 tomba 8001FDA4 156
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80019CEC (src/gameStateDispatcher.c): the task pointer is the RAM global D_800DE0B0.
#define TASK_C D_800DE0B0
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern void func_8001FE40(void);
extern void func_80020000_pcpx96084(void);
extern void func_80021238(void);
extern void func_80021300(void);

/* Task state1 (+0x4A): 0 intro, 1 gameplay, 2 game end screen, 3 game over (set by transitionToGameOver). */
void gameStateDispatcher(void)
{
    switch (TASK_C->state1) {
    case 0: func_8001FE40(); break;
    case 1: func_80020000_pcpx96084(); break;
    case 2: func_80021238(); break;
    case 3: func_80021300(); break;
    }
}
#endif

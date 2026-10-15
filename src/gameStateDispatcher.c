// FUNC 80019cec 156 MAIN0
// MATCHING 80019cec 156
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern void introSequenceHandler(void);
extern void gameplayStateDispatcher(void);
extern void gameEndScreenState(void);
extern void gameOverScreenState(void);

/* Task state1 (+0x4A): 0 intro, 1 gameplay, 2 game end screen, 3 game over (set by transitionToGameOver). */
void gameStateDispatcher(void)
{
    switch (TASK_C->state1) {
    case 0: introSequenceHandler(); break;
    case 1: gameplayStateDispatcher(); break;
    case 2: gameEndScreenState(); break;
    case 3: gameOverScreenState(); break;
    }
}

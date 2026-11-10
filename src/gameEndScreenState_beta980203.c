// VERSION BETA_980203 beta980203 8001CC84 392
#if defined(VERSION_BETA_980203)
// Beta version: PAL gameEndScreenState.c: 360-frame countdown (PAL 240), one-argument queueSystemLoadList, state0 = 0 at the end (PAL 1).
#define SKIP_ASM
#include "common.h"
#include "game.h"

extern void queueSystemLoadList(int);

void gameEndScreenState(void)
{
    Task* task = TASK_C;

    switch ((u_short)task->state2) {
    case 0:
        task->countdown = 360;
        task->step.value = 0;
        *(u8*)0x1F8001CE = 0;
        task->state2++;
        queueSystemLoadList(0x5D);
        break;
    case 1:
        if (*(u8*)0x1F8001CE == 0) break;
        task->state2++;
        break;
    case 2:
    {
        Task* t2;
        s16 val;
        func_800E8B08();
        t2 = (*(Task**)(&SCRATCHPAD + 0x1D4));
        val = --t2->countdown;
        if (val == -1) {
            t2->state2++;
        } else {
            if (val >= 0x3D) {
                if (*(u16*)0x1F8001FC & 0x6008) {
                    t2->countdown = 0x3C;
                }
            }
            if ((s16)CURRENT_TASK->countdown == 0x3C) {
                func_8001F110(1);
            }
        }
        func_8001DBDC();
        break;
    }
    case 3:
    {
        Task* t;
        func_8001F4BC();
        t = TASK_C;
        *(char*)0x1F8001D0 = 0;
        t->state0 = 0;
        t->state1 = 0;
        t->state2 = 0;
        t->step.value = 0;
        setTask(&titleSequenceTask);
        break;
    }
    }
}
#endif

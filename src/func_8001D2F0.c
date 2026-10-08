// FUNC 8001c688 400 MAIN0
// MATCHING 8001c688 400
// Portado de psx_tomba (gamestate.c, func_8001D2F0); licencia MIT del proyecto original.
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

void func_8001D2F0(void)
{
    unkstruct_1F8001D4* task = (unkstruct_1F8001D4*)D_1F8001D4;

    switch ((u_short)task->state2) {
    case 0:
        task->unk5C = 0xF0;
        task->unk4E.value = 0;
        *(u8*)0x1F8001CE = 0;
        task->state2++;
        func_8004FA80(0x5D, 1);
        break;
    case 1:
        if (*(u8*)0x1F8001CE == 0) break;
        task->state2++;
        break;
    case 2:
    {
        unkstruct_1F8001D4* t2;
        s16 val;
        func_800E8B08();
        t2 = (*(unkstruct_1F8001D4**)(&SCRATCHPAD + 0x1D4));
        val = --t2->unk5C;
        if (val == -1) {
            t2->state2++;
        } else {
            if (val >= 0x3D) {
                if (*(u16*)0x1F8001FC & 0x6008) {
                    t2->unk5C = 0x3C;
                }
            }
            if ((s16)CURRENT_TASK->unk5C == 0x3C) {
                func_8001F110(1);
            }
        }
        func_8001DBDC();
        break;
    }
    case 3:
    {
        unkstruct_1F8001D4* t;
        func_8001F4BC();
        t = TASK_C;
        *(char*)0x1F8001D0 = 0;
        t->state0 = 1;
        t->state1 = 0;
        t->state2 = 0;
        t->unk4E.value = 0;
        setTask(&titleSequenceTask);
        break;
    }
    }
}

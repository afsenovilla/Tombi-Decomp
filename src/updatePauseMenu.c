// FUNC 8001ca58 1892 MAIN0
// MATCHING 8001ca58 1892
// Portado de psx_tomba (gamestate.c, updatePauseMenu); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

void updatePauseMenu(void) {
    if (*(u8*)0x1F8003CE | *(u8*)0x1F8001CC) {
        return;
    }

    switch (*(s16*)0x1F8001C6) {
    case 0: {
        u16 pad;

        if (GAME_FADE_SCREEN_CONTROL != 0 && GAME_FADE_SCREEN_CONTROL != 0x10) {
            return;
        }

        pad = *(u16*)0x1F8001FC;
        if ((pad & 8) || PAD_BUFFERS != 0) {
            *(s16*)(&SCRATCHPAD + 0x1C6) = 1;
            GAME_INVENTORY_SCREEN = 0;
            *(&SCRATCHPAD + 0x3CD) = 1;
            keyOffSfxAll(0);
            playSFX(0x29);
            return;
        }

        if (GAME_DISABLE_SELECT_MENU != 0) {
            return;
        }
        if (D_8009BCA2 == 0) {
            return;
        }
        if (GAME_PLAYER_MODE != 1) {
            return;
        }
        if (D_8009BCA7 != 0) {
            return;
        }

        if (D_800A5462 != GAME_PLAYER_MODE) {
            if (PLAYER_STATE != GAME_PLAYER_MODE) {
                return;
            }
            if (PLAYER_SUBSTATE >= 0x20) {
                return;
            }
        }

        if (pad & 1) {
            *(s16*)(&SCRATCHPAD + 0x1C6) = 1;
            GAME_INVENTORY_SCREEN = 0;
            *(&SCRATCHPAD + 0x3CD) = 0;
            keyOffSfxAll(0);
            playSFX(0x29);
            return;
        }

        if (*(u8*)0x1F8001B8 != 0) {
            return;
        }
        if ((pad & *(u16*)0x1F8003CA) == 0) {
            return;
        }

        *(s16*)(&SCRATCHPAD + 0x1C6) = 1;
        GAME_INVENTORY_SCREEN = 4;
        *(&SCRATCHPAD + 0x3CD) = 0;
        keyOffSfxAll(0);

        {
            Task* p;
            u16 savedS2;
            u16 savedUn;
            p = CURRENT_TASK;
            savedS2 = p->state2;
            savedUn = p->step.value;

            MENU_STATE.unk12 = 7;
            MENU_STATE.unk16 = 4;
            MENU_STATE.unk14 = 0;
            MENU_STATE.unk0 = 0;
            p->state2 = 3;
            p->step.value = 0;
            *(u16*)0x1F8003B8 = savedS2;
            *(u16*)0x1F8003BA = savedUn;
            func_80020058(10, 10);
        }
        break;
    }

    case 1: {
        u8 cd;
        short buttons;

        cd = *(u8*)0x1F8003CD;
        switch (cd) {
        case 0:
            buttons = 0x2001;
            break;
        case 1:
            buttons = 0x2000;
            break;
        }

        if (*(u8*)0x1F8003CD < 2) {
            if (*(u16*)0x1F8001FC & buttons) {
                *(u16*)(&SCRATCHPAD + 0x1FC) = 0;
                *(s16*)(&SCRATCHPAD + 0x1C6) = 0;
                GAME_INVENTORY_SCREEN = 0;
                return;
            }
        }

        switch (*(u8*)0x1F8003CD) {
        case 0: {
            u16 pad = *(u16*)0x1F8001FC;

            if (pad & 8) {
                *(s16*)(&SCRATCHPAD + 0x1C6) = 1;
                GAME_INVENTORY_SCREEN = 0;
                *(&SCRATCHPAD + 0x3CD) = 1;
                playSFX(0x29);
                return;
            }

            if (pad & 0x4000) {
                Task* task = CURRENT_TASK;
                u16 s2 = task->state2;
                u16 un = task->step.value;
                int c10b;

                *(u16*)0x1F8003B8 = s2;
                *(u16*)0x1F8003BA = un;

                c10b = GAME_INVENTORY_SCREEN;
                switch (c10b) {
                case 0:
                    MENU_STATE.unk16 = 4;
                    MENU_STATE.unk12 = c10b + 1;
                    MENU_STATE.unk14 = 0;
                    MENU_STATE.unk0 = 0;
                    task->state2 = 3;
                    task->step.value = 0;
                    func_80020058(10, 10);
                    return;
                case 1:
                    MENU_STATE.unk12 = 2;
                    MENU_STATE.unk16 = 5;
                    MENU_STATE.unk14 = 0;
                    MENU_STATE.unk0 = 0;
                    task->state2 = 3;
                    task->step.value = 0;
                    func_80020058(10, 10);
                    return;
                case 2:
                    MENU_STATE.unk16 = 6;
                    MENU_STATE.unk12 = 0;
                    MENU_STATE.unk14 = 0;
                    MENU_STATE.unk0 = 0;
                    task->state2 = 3;
                    task->step.value = 0;
                    func_80020058(10, 10);
                    return;
                case 3:
                    MENU_STATE.unk16 = 7;
                    MENU_STATE.unk12 = c10b;
                    MENU_STATE.unk14 = 0;
                    MENU_STATE.unk0 = 0;
                    task->state2 = 3;
                    task->step.value = 0;
                    func_80020058(10, 10);
                    return;
                }
                return;
            }

            if (pad & 0x10) {
                u8 *p = &GAME_INVENTORY_SCREEN;
                u8 val = *p;
                if (val != 0) {
                    *p = val - 1;
                    playSFX(8);
                    return;
                }
            }

            if (*(u16*)0x1F8001FC & 0x40) {
                u8 *p = &GAME_INVENTORY_SCREEN;
                u8 val = *p;
                if (val < 3) {
                    *p = val + 1;
                    playSFX(8);
                    return;
                }
            }
            break;
        }

        case 1: {
            u16 pad = *(u16*)0x1F8001FC;

            if (pad & 0x4008) {
                u8* s0 = &GAME_INVENTORY_SCREEN;
                u8 c10b = *s0;

                switch (c10b) {
                case 0:
                    *(u16*)(&SCRATCHPAD + 0x1FC) = 0;
                    *(s16*)(&SCRATCHPAD + 0x1C6) = 0;
                    *s0 = 0;
                    return;
                case 1:
                    func_80020058(10, 10);
                    *(&SCRATCHPAD + 0x3CD) = 2;
                    *s0 = 1;
                    return;
                case 2:
                    func_80020058(10, 10);
                    *(&SCRATCHPAD + 0x3CD) = 3;
                    *s0 = 1;
                    return;
                default:
                    return;
                }
            }

            if (pad & 0x10) {
                u8 *p = &GAME_INVENTORY_SCREEN;
                u8 val = *p;
                if (val != 0) {
                    *p = val - 1;
                    playSFX(8);
                    return;
                }
            }

            if (*(u16*)0x1F8001FC & 0x40) {
                u8 *p = &GAME_INVENTORY_SCREEN;
                u8 val = *p;
                if (val < 2) {
                    *p = val + 1;
                    playSFX(8);
                    return;
                }
            }
            break;
        }

        case 2:
        case 3:
            if (*(u16*)0x1F8001FC & 0x4008) {
                u8 c10b = GAME_INVENTORY_SCREEN;

                if (c10b == 0) {
                    if (*(u8*)0x1F8003CD == 2) {
                        SetDispMask(0);
                        func_80020FAC();
                        {
                            Task* task = CURRENT_TASK;
                            task->state0 = 1;
                            task->state1 = 3;
                            task->state2 = 3;
                        }
                    } else {
                        Task* task;
                        u16 s2, un;

                        func_80020058(10, 10);
                        task = CURRENT_TASK;
                        s2 = task->state2;
                        un = task->step.value;

                        MENU_STATE.unk12 = 5;
                        MENU_STATE.unk0 = 0;
                        MENU_STATE.unk1 = 0;
                        MENU_STATE.unk16 = 0;
                        task->state2 = 3;
                        task->step.value = 0;
                        *(u16*)0x1F8003B8 = s2;
                        *(u16*)0x1F8003BA = un;
                    }
                    return;
                }
                if (c10b == 1) goto cd_check;
            }

            if (*(u16*)0x1F8001FC & 0x2000) {
            cd_check:
                if (*(u8*)0x1F8003CD == 2) {
                    GAME_INVENTORY_SCREEN = 1;
                } else {
                    GAME_INVENTORY_SCREEN = 2;
                }
                *(&SCRATCHPAD + 0x3CD) = 1;
                return;
            }

            if (*(u16*)0x1F8001FC & 0x20) {
                u8 *p = &GAME_INVENTORY_SCREEN;
                u8 val = *p;
                if (val == 0) {
                    *p = val + 1;
                    playSFX(8);
                    return;
                }
            }

            if (*(u16*)0x1F8001FC & 0x80) {
                u8 *p = &GAME_INVENTORY_SCREEN;
                u8 val = *p;
                if (val == 1) {
                    *p = val - 1;
                    playSFX(8);
                    return;
                }
            }
            break;
        }
        break;
    }
    case 2:
        return;
    }
}

// VERSION SLPS_01144 slps01144 8001C2D8 880
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct { char p0[0x4c]; unsigned short state2; unsigned short step; } Task;
extern short DRAW_FROZEN;
extern unsigned char D_1F8001B8;
extern Task *TASK_C;
extern unsigned short JOYPAD_STATE, D_1F8003B8, D_1F8003BA, BUTTON_MASK_B, BUTTON_MASK_C, BUTTON_MASK_D;
#define CURRENT_TASK TASK_C
extern unsigned char D_8009E94A, D_8009E4E2, D_8009E4E7, D_800A7C42, D_800A7B7C, D_800A7B7D;
extern signed char D_8009EE58;
extern unsigned char GAME_INVENTORY_SCREEN;
extern unsigned char MENU_STATE;
extern short D_800A6132, D_800A6134, D_800A6136;
extern void SfxStopAll(int);
extern void playSFX(int);
extern void playSFXWithNote(int, int);

void func_8001C2D8_slps01144(void)
{
    switch (DRAW_FROZEN) {
    case 0: {
        unsigned short pad;
        if (D_8009E94A != 0) return;
        if (D_8009E4E2 == 0) return;
        if (D_8009EE58 != 1) return;
        if (D_8009E4E7 != 0) return;
        if (D_800A7C42 != 1) {
            if (D_800A7B7C != 1) return;
            if (D_800A7B7D >= 32) return;
        }
        pad = JOYPAD_STATE;
        if (pad & 0x800) {
            DRAW_FROZEN = 1;
            GAME_INVENTORY_SCREEN = 0;
            playSFX(0x29);
            return;
        }
        if (D_1F8001B8 != 0) return;
        if ((pad & BUTTON_MASK_D) == 0) return;
        DRAW_FROZEN = 1;
        GAME_INVENTORY_SCREEN = 4;
        playSFX(0x29);
        break;
    }
    case 1: {
        unsigned short pad = JOYPAD_STATE;
        if ((pad & 0x800) || (pad & BUTTON_MASK_B)) {
            DRAW_FROZEN = 0;
            GAME_INVENTORY_SCREEN = 0;
            return;
        }
        if (pad & BUTTON_MASK_C) {
            unsigned short s2, st;
            int c;
            SfxStopAll(0);
            c = GAME_INVENTORY_SCREEN;
            s2 = TASK_C->state2;
            st = TASK_C->step;
            D_1F8003B8 = s2;
            D_1F8003BA = st;
            c = GAME_INVENTORY_SCREEN;
            switch (c) {
            case 0:
                D_800A6132 = 1;
                D_800A6136 = 4;
                D_800A6134 = 0;
                MENU_STATE = 0;
                TASK_C->state2 = 3;
                TASK_C->step = 0;
                playSFXWithNote(10, 10);
                return;
            case 1:
                D_800A6132 = 2;
                D_800A6136 = 5;
                D_800A6134 = 0;
                MENU_STATE = 0;
                TASK_C->state2 = 3;
                TASK_C->step = 0;
                playSFXWithNote(10, 10);
                return;
            case 2:
                D_800A6136 = 6;
                D_800A6132 = 0;
                D_800A6134 = 0;
                MENU_STATE = 0;
                TASK_C->state2 = 3;
                TASK_C->step = 0;
                playSFXWithNote(10, 10);
                return;
            case 3:
                D_800A6132 = 3;
                D_800A6134 = 0;
                D_800A6136 = 7;
                MENU_STATE = 0;
                TASK_C->state2 = 3;
                TASK_C->step = 0;
                playSFXWithNote(10, 10);
                return;
            case 4:
                D_800A6132 = 7;
                D_800A6136 = 4;
                D_800A6134 = 0;
                MENU_STATE = 0;
                TASK_C->state2 = 3;
                TASK_C->step = 0;
                playSFXWithNote(10, 10);
                return;
            }
            return;
        }
        {
            unsigned char *p = &GAME_INVENTORY_SCREEN;
            unsigned char v = *p;
            if (v >= 4) return;
            if (pad & 0x1000) {
                if (v != 0) {
                    *p = v - 1;
                    playSFX(8);
                }
            }
        }
        if (JOYPAD_STATE & 0x4000) {
            unsigned char *p = &GAME_INVENTORY_SCREEN;
            unsigned char v = *p;
            if (v < 3) {
                *p = v + 1;
                playSFX(8);
            }
        }
        break;
    }
    case 2:
        break;
    }
}
#endif

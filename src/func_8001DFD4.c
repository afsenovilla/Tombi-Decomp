// FUNC 8001d36c 324 MAIN0
// MATCHING 8001d36c 324
// Portado de psx_tomba (gamestate.c, func_8001DFD4); licencia MIT del proyecto original.
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

void func_8001DFD4(void)
{
    u8* s0 = (u8*)&D_800A5970;
    u8* s1 = s0 + 2;

    *(s32*)&D_1F800198 = 0;

    do {
        if (*s0 != 0) {
            switch (s1[0x1A] & 0x7F) {
            case 2:
                ((void (*)(u8*))D_800772BC[*s1])(s0);
                break;
            case 3:
                ((void (*)(u8*))D_8007C6B0[*s1])(s0);
                break;
            case 4:
                ((void (*)(u8*))D_8007D30C[*s1])(s0);
                break;
            case 5:
                ((void (*)(u8*))D_8007E8A8[*s1])(s0);
                break;
            }
        }
        s1 += 0xD4;
        s0 += 0xD4;
    } while (++(*(s32*)&D_1F800198) < 0xC8);
}

// FUNC 80030fcc 260 MAIN0
// MATCHING 80030fcc 260
// Ported from psx_tomba (effect.c, updateScrollAnchoredEffect); MIT licence of the original project.
#include "gcd.h"
#define SKIP_ASM
#include "common.h"
#include "game.h"
void updateScrollAnchoredEffect(u8* self);

extern u8 D_8009C1C0;

extern u8 D_8009BCA6;

void func_800346A8(u8* self);

void updateScrollAnchoredEffect(u8* self)
{
    u8 state;

    state = self[4];
    switch (state) {
    case 0:
        self[0xD] = 1;
        self[0xF] = 0;
        *(int*)(self + 0x30) = *(s16*)(self + 0x12);
        *(int*)(self + 0x34) = *(s16*)(self + 0x16);
        self[4]++;
        break;
    case 1:
        if (self[3] == 0x1B && GCD.bd088 == 0xFF) {
            break;
        }
        self[1] = 1;
        *(s16*)(self + 0x12) = *(int*)(self + 0x30) - *(u16*)0x1F800176;
        *(s16*)(self + 0x16) = *(int*)(self + 0x34) - *(u16*)0x1F800186;
        pushDrawListLayer7(self);
        break;
    case 2:
        self[4] = state + 1;
        break;
    case 3:
        freeObjectLayer7(self);
        break;
    }
}

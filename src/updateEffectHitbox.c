// FUNC 80030d58 480 MAIN0
// MATCHING 80030d58 480
// Ported from psx_tomba (effect.c, updateEffectHitbox); MIT licence of the original project.
#include "gcd.h"
#define SKIP_ASM
#include "common.h"
#include "game.h"
void updateScrollAnchoredEffect(u8* self);

extern u8 D_8009C3F0;
extern u8 D_8009BCA6;
void func_800346A8(u8* self);
/* The effect tables live in the retail .rodata (linked from splat by tools/build_full.py). */
#ifdef EFFECT_TABLES

extern u8 D_8009C3F0;
extern u8 D_8009BCA6;

void func_800346A8(u8* self);

#endif

void updateEffectHitbox(u8* self)
{
    u8 state;

    state = self[4];
    switch (state) {
    case 0:
        self[4] = state + 1;
        *(int*)(self + 0xA0) = 0;
        switch (self[0xC]) {
        case 0:
            *(s16*)(self + 0x6C) = 0x50;
            *(s16*)(self + 0x6E) = 0xA0;
            *(s16*)(self + 0x70) = 0x40;
            *(s16*)(self + 0x72) = 0x5C;
            break;
        case 1:
            *(s16*)(self + 0x6C) = 0x50;
            *(s16*)(self + 0x6E) = 0xA0;
            *(s16*)(self + 0x70) = 0x18;
            *(s16*)(self + 0x72) = 0x24;
            break;
        case 2:
            *(s16*)(self + 0x6C) = 0x50;
            *(s16*)(self + 0x6E) = 0xA0;
            *(s16*)(self + 0x70) = 0x50;
            *(s16*)(self + 0x72) = 0xA0;
            break;
        case 3:
            *(s16*)(self + 0x6C) = 0xA;
            *(s16*)(self + 0x6E) = 0x14;
            *(s16*)(self + 0x70) = 8;
            *(s16*)(self + 0x72) = 0x10;
            break;
        case 4:
            if (self[3] == 0) {
                *(s16*)(self + 0x6C) = 0x24;
                *(s16*)(self + 0x6E) = 0x36;
                *(s16*)(self + 0x70) = 8;
                *(s16*)(self + 0x72) = 0x2E;
            } else {
                *(s16*)(self + 0x6C) = 0x10;
                *(s16*)(self + 0x6E) = 0x28;
                *(s16*)(self + 0x70) = 6;
                *(s16*)(self + 0x72) = 0x26;
            }
            break;
        }
        break;
    case 1:
        switch (self[0xC]) {
        case 0:
            self[1] = 1;
            pushDrawListLayer4(self);
            break;
        case 2:
            if (GCD.bce58 != 0) {
                self[4] = 3;
                break;
            }
        case 1:
        case 3:
        case 4:
            func_800202B4(self);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer4(self);
        break;
    }
}

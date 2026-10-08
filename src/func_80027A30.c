// FUNC 80024ea0 3080 MAIN0
// MATCHING 80024ea0 3080
// Portado de psx_tomba (ui.c, func_80027A30); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_80027A30(u8* dst, u8* src, s32 scale)
{
    s32 n;

    n = *(s32*)src;
    src += 4;
    if (n != 0) {
        do {
            *(s16*)(dst + 0x8) -=
                (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0xA) -=
                (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0xC) -=
                (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x10) -=
                (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0x12) -=
                (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0x14) -=
                (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x18) -=
                (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x1A) -=
                (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x1C) -=
                (*(s16*)(src + 0x14) * scale) >> 12;
            dst += 0x20;
            src += 0x20;
        } while (--n != 0);
    }

    n = *(s32*)src;
    src += 4;
    if (n != 0) {
        do {
            *(s16*)(dst + 0x8) -=
                (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0xA) -=
                (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0xC) -=
                (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x10) -=
                (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0x12) -=
                (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0x14) -=
                (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x18) -=
                (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x1A) -=
                (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x1C) -=
                (*(s16*)(src + 0x14) * scale) >> 12;
            *(s16*)(dst + 0x20) -=
                (*(s16*)(src + 0x18) * scale) >> 12;
            *(s16*)(dst + 0x22) -=
                (*(s16*)(src + 0x1A) * scale) >> 12;
            *(s16*)(dst + 0x24) -=
                (*(s16*)(src + 0x1C) * scale) >> 12;
            dst += 0x28;
            src += 0x28;
        } while (--n != 0);
    }

    n = *(s32*)src;
    src += 4;
    if (n != 0) {
        do {
            *(s16*)(dst + 0x10) -=
                (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x12) -=
                (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x14) -=
                (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x18) -=
                (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0x1A) -=
                (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0x1C) -=
                (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x20) -=
                (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x22) -=
                (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x24) -=
                (*(s16*)(src + 0x14) * scale) >> 12;
            dst += 0x28;
            src += 0x30;
        } while (--n != 0);
    }

    n = *(s32*)src;
    src += 4;
    if (n != 0) {
        do {
            *(s16*)(dst + 0x14) -=
                (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x16) -=
                (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x18) -=
                (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x1C) -=
                (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0x1E) -=
                (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0x20) -=
                (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x24) -=
                (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x26) -=
                (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x28) -=
                (*(s16*)(src + 0x14) * scale) >> 12;
            *(s16*)(dst + 0x2C) -=
                (*(s16*)(src + 0x18) * scale) >> 12;
            *(s16*)(dst + 0x2E) -=
                (*(s16*)(src + 0x1A) * scale) >> 12;
            *(s16*)(dst + 0x30) -=
                (*(s16*)(src + 0x1C) * scale) >> 12;
            dst += 0x34;
            src += 0x40;
        } while (--n != 0);
    }

    n = *(s32*)src;
    src += 4;
    if (n != 0) {
        do {
            *(s16*)(dst + 0x4) -=
                (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x6) -=
                (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x8) -=
                (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0xC) -=
                (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xE) -=
                (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0x10) -=
                (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x14) -=
                (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x16) -=
                (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x18) -=
                (*(s16*)(src + 0x14) * scale) >> 12;
            dst += 0x28;
            src += 0x20;
        } while (--n != 0);
    }

    n = *(s32*)src;
    src += 4;
    if (n != 0) {
        do {
            *(s16*)(dst + 0x4) -=
                (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x6) -=
                (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x8) -=
                (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0xC) -=
                (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xE) -=
                (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0x10) -=
                (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x14) -=
                (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x16) -=
                (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x18) -=
                (*(s16*)(src + 0x14) * scale) >> 12;
            *(s16*)(dst + 0x1C) -=
                (*(s16*)(src + 0x18) * scale) >> 12;
            *(s16*)(dst + 0x1E) -=
                (*(s16*)(src + 0x1A) * scale) >> 12;
            *(s16*)(dst + 0x20) -=
                (*(s16*)(src + 0x1C) * scale) >> 12;
            dst += 0x30;
            src += 0x28;
        } while (--n != 0);
    }

    n = *(s32*)src;
    src += 4;
    if (n != 0) {
        do {
            *(s16*)(dst + 0xC) -=
                (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0xE) -=
                (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x10) -=
                (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x14) -=
                (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0x16) -=
                (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0x18) -=
                (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x1C) -=
                (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x1E) -=
                (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x20) -=
                (*(s16*)(src + 0x14) * scale) >> 12;
            dst += 0x40;
            src += 0x30;
        } while (--n != 0);
    }

    n = *(s32*)src;
    src += 4;
    if (n != 0) {
        do {
            *(s16*)(dst + 0x10) -=
                (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x12) -=
                (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x14) -=
                (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x18) -=
                (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0x1A) -=
                (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0x1C) -=
                (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x20) -=
                (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x22) -=
                (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x24) -=
                (*(s16*)(src + 0x14) * scale) >> 12;
            *(s16*)(dst + 0x28) -=
                (*(s16*)(src + 0x18) * scale) >> 12;
            *(s16*)(dst + 0x2A) -=
                (*(s16*)(src + 0x1A) * scale) >> 12;
            *(s16*)(dst + 0x2C) -=
                (*(s16*)(src + 0x1C) * scale) >> 12;
            dst += 0x54;
            src += 0x40;
        } while (--n != 0);
    }
}

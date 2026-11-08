// VERSION SLPS_01144 slps01144 80025F70 3084
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "common.h"
#include "game.h"

void func_80025F70_slps01144(u8* dst, u8* src, s32 scale)
{
    s32 n1;
    s32 n2;
    s32 n3;
    s32 n4;
    s32 n5;
    s32 n6;
    s32 n7;
    s32 n8;

    n1 = *(s32*)src;
    src += 4;
    if (n1 != 0) {
        do {
            *(s16*)(dst + 0x0) -= (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x2) -= (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x4) -= (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x8) -= (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xA) -= (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0xC) -= (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x10) -= (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x12) -= (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x14) -= (*(s16*)(src + 0x14) * scale) >> 12;
            dst += 32;
            src += 32;
        } while (--n1 != 0);
    }

    n2 = *(s32*)src;
    src += 4;
    if (n2 != 0) {
        do {
            *(s16*)(dst + 0x0) -= (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x2) -= (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x4) -= (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x8) -= (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xA) -= (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0xC) -= (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x10) -= (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x12) -= (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x14) -= (*(s16*)(src + 0x14) * scale) >> 12;
            *(s16*)(dst + 0x18) -= (*(s16*)(src + 0x18) * scale) >> 12;
            *(s16*)(dst + 0x1A) -= (*(s16*)(src + 0x1A) * scale) >> 12;
            *(s16*)(dst + 0x1C) -= (*(s16*)(src + 0x1C) * scale) >> 12;
            dst += 40;
            src += 40;
        } while (--n2 != 0);
    }

    n3 = *(s32*)src;
    src += 4;
    if (n3 != 0) {
        do {
            *(s16*)(dst + 0x0) -= (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x2) -= (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x4) -= (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x8) -= (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xA) -= (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0xC) -= (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x10) -= (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x12) -= (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x14) -= (*(s16*)(src + 0x14) * scale) >> 12;
            dst += 48;
            src += 48;
        } while (--n3 != 0);
    }

    n4 = *(s32*)src;
    src += 4;
    if (n4 != 0) {
        do {
            *(s16*)(dst + 0x0) -= (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x2) -= (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x4) -= (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x8) -= (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xA) -= (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0xC) -= (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x10) -= (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x12) -= (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x14) -= (*(s16*)(src + 0x14) * scale) >> 12;
            *(s16*)(dst + 0x18) -= (*(s16*)(src + 0x18) * scale) >> 12;
            *(s16*)(dst + 0x1A) -= (*(s16*)(src + 0x1A) * scale) >> 12;
            *(s16*)(dst + 0x1C) -= (*(s16*)(src + 0x1C) * scale) >> 12;
            dst += 64;
            src += 64;
        } while (--n4 != 0);
    }

    n5 = *(s32*)src;
    src += 4;
    if (n5 != 0) {
        do {
            *(s16*)(dst + 0x0) -= (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x2) -= (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x4) -= (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x8) -= (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xA) -= (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0xC) -= (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x10) -= (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x12) -= (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x14) -= (*(s16*)(src + 0x14) * scale) >> 12;
            dst += 32;
            src += 32;
        } while (--n5 != 0);
    }

    n6 = *(s32*)src;
    src += 4;
    if (n6 != 0) {
        do {
            *(s16*)(dst + 0x0) -= (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x2) -= (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x4) -= (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x8) -= (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xA) -= (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0xC) -= (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x10) -= (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x12) -= (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x14) -= (*(s16*)(src + 0x14) * scale) >> 12;
            *(s16*)(dst + 0x18) -= (*(s16*)(src + 0x18) * scale) >> 12;
            *(s16*)(dst + 0x1A) -= (*(s16*)(src + 0x1A) * scale) >> 12;
            *(s16*)(dst + 0x1C) -= (*(s16*)(src + 0x1C) * scale) >> 12;
            dst += 40;
            src += 40;
        } while (--n6 != 0);
    }

    n7 = *(s32*)src;
    src += 4;
    if (n7 != 0) {
        do {
            *(s16*)(dst + 0x0) -= (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x2) -= (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x4) -= (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x8) -= (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xA) -= (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0xC) -= (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x10) -= (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x12) -= (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x14) -= (*(s16*)(src + 0x14) * scale) >> 12;
            dst += 48;
            src += 48;
        } while (--n7 != 0);
    }

    n8 = *(s32*)src;
    src += 4;
    if (n8 != 0) {
        do {
            *(s16*)(dst + 0x0) -= (*(s16*)(src + 0x0) * scale) >> 12;
            *(s16*)(dst + 0x2) -= (*(s16*)(src + 0x2) * scale) >> 12;
            *(s16*)(dst + 0x4) -= (*(s16*)(src + 0x4) * scale) >> 12;
            *(s16*)(dst + 0x8) -= (*(s16*)(src + 0x8) * scale) >> 12;
            *(s16*)(dst + 0xA) -= (*(s16*)(src + 0xA) * scale) >> 12;
            *(s16*)(dst + 0xC) -= (*(s16*)(src + 0xC) * scale) >> 12;
            *(s16*)(dst + 0x10) -= (*(s16*)(src + 0x10) * scale) >> 12;
            *(s16*)(dst + 0x12) -= (*(s16*)(src + 0x12) * scale) >> 12;
            *(s16*)(dst + 0x14) -= (*(s16*)(src + 0x14) * scale) >> 12;
            *(s16*)(dst + 0x18) -= (*(s16*)(src + 0x18) * scale) >> 12;
            *(s16*)(dst + 0x1A) -= (*(s16*)(src + 0x1A) * scale) >> 12;
            *(s16*)(dst + 0x1C) -= (*(s16*)(src + 0x1C) * scale) >> 12;
            dst += 64;
            src += 64;
        } while (--n8 != 0);
    }

}
#endif

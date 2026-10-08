// FUNC 800189b8 628 MAIN0
// MATCHING 800189b8 628
// Ported from psx_tomba (gameinit.c, freeObjectByLayer); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/gameinit", allocObjectLayer3);














const char BUILD_DATE[] asm("D_80010000") = "98/3/22";

const char BUILD_TIME[] asm("D_80010008") = "21:11";

void freeObjectByLayer(s32* self)
{
    switch (((u8*)self)[0x1C] & 0x7F) {
    case OBJECT_LAYER_1:
        ((u8*)self)[0x1C] &= 0x7F;
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        D_1F800236++;
        *--D_1F800204 = (s32)self;
        break;
    case OBJECT_LAYER_2:
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        asm("");
        ((u8*)self)[0x1C] = 0;
        ((u8*)self)[0x9C] = 0;
        ((u8*)self)[0x9D] = 0;
        ((u8*)self)[0x9E] = 0;
        ((u8*)self)[0x9F] = 0;
        D_1F800238++;
        *--D_1F800208 = (s32)self;
        break;
    case OBJECT_LAYER_3:
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        asm("");
        ((u8*)self)[0x1C] = 0;
        ((u8*)self)[0x9C] = 0;
        ((u8*)self)[0x9D] = 0;
        ((u8*)self)[0x9E] = 0;
        ((u8*)self)[0x9F] = 0;
        D_1F800238++;
        *--D_1F800208 = (s32)self;
        break;
    case OBJECT_LAYER_4:
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        asm("");
        ((u8*)self)[0x1C] = 0;
        ((u8*)self)[0x9C] = 0;
        ((u8*)self)[0x9D] = 0;
        ((u8*)self)[0x9E] = 0;
        ((u8*)self)[0x9F] = 0;
        D_1F800238++;
        *--D_1F800208 = (s32)self;
        break;
    case OBJECT_LAYER_5:
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        asm("");
        ((u8*)self)[0x1C] = 0;
        ((u8*)self)[0x9C] = 0;
        ((u8*)self)[0x9D] = 0;
        ((u8*)self)[0x9E] = 0;
        ((u8*)self)[0x9F] = 0;
        D_1F800238++;
        *--D_1F800208 = (s32)self;
        break;
    case OBJECT_LAYER_7:
        ((u8*)self)[0x1C] &= 0x7F;
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        D_1F80023C++;
        *--D_1F800214 = (s32)self;
        break;
    case OBJECT_LAYER_8:
        ((u8*)self)[0x1C] &= 0x7F;
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        ((u8*)self)[0x9C] = 0;
        ((u8*)self)[0x9D] = 0;
        D_1F80023A++;
        *--D_1F80020C = (s32)self;
        break;
    }
}

// JPDEMO slpm80165 80017620 596
// Demo version of PAL main0 800189B8 (freeObjectByLayer.c): layers 2-6 do not clear +0x9e/+0x9f.
// Ported from psx_tomba (gameinit.c, freeObjectByLayer); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/gameinit", allocObjectLayer3);
















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

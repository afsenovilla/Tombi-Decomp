// FUNC 800189b8 628 MAIN0
// MATCHING 800189b8 628
// VERSION SLPM_80165 slpm80165 80017620 596
// Ported from psx_tomba (gameinit.c, freeObjectByLayer); MIT licence of the original project.
#if defined(VERSION_SLPM_80165)
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
        FREE_OBJECT_COUNT_LAYER1++;
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
        FREE_OBJECT_COUNT++;
        *--FREE_OBJECTS = (s32)self;
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
        FREE_OBJECT_COUNT++;
        *--FREE_OBJECTS = (s32)self;
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
        FREE_OBJECT_COUNT++;
        *--FREE_OBJECTS = (s32)self;
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
        FREE_OBJECT_COUNT++;
        *--FREE_OBJECTS = (s32)self;
        break;
    case OBJECT_LAYER_7:
        ((u8*)self)[0x1C] &= 0x7F;
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        FREE_OBJECT_COUNT_LAYER7++;
        *--FREE_OBJECTS_LAYER7 = (s32)self;
        break;
    case OBJECT_LAYER_8:
        ((u8*)self)[0x1C] &= 0x7F;
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        ((u8*)self)[0x9C] = 0;
        ((u8*)self)[0x9D] = 0;
        FREE_OBJECT_COUNT_LAYER8++;
        *--FREE_OBJECTS_LAYER8 = (s32)self;
        break;
    }
}
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/gameinit", allocObjectLayer3);














const char BUILD_DATE[] asm("D_80010000") = "98/8/26";

const char BUILD_TIME[] asm("D_80010008") = "14:10";

void freeObjectByLayer(s32* self)
{
    switch (((u8*)self)[0x1C] & 0x7F) {
    case OBJECT_LAYER_1:
        ((u8*)self)[0x1C] &= 0x7F;
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        FREE_OBJECT_COUNT_LAYER1++;
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
        FREE_OBJECT_COUNT++;
        *--FREE_OBJECTS = (s32)self;
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
        FREE_OBJECT_COUNT++;
        *--FREE_OBJECTS = (s32)self;
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
        FREE_OBJECT_COUNT++;
        *--FREE_OBJECTS = (s32)self;
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
        FREE_OBJECT_COUNT++;
        *--FREE_OBJECTS = (s32)self;
        break;
    case OBJECT_LAYER_7:
        ((u8*)self)[0x1C] &= 0x7F;
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        FREE_OBJECT_COUNT_LAYER7++;
        *--FREE_OBJECTS_LAYER7 = (s32)self;
        break;
    case OBJECT_LAYER_8:
        ((u8*)self)[0x1C] &= 0x7F;
        self[0] = 0;
        self[1] = 0;
        self[2] = 0;
        self[3] = 0;
        ((u8*)self)[0x9C] = 0;
        ((u8*)self)[0x9D] = 0;
        FREE_OBJECT_COUNT_LAYER8++;
        *--FREE_OBJECTS_LAYER8 = (s32)self;
        break;
    }
}
#endif

// JP slps01144 800220F0 316
// Japanese version of PAL main0 8002171C (loadSpawnPosition.c): 244 -> 228, 238 -> 222, 240 -> 224, 236 -> 220.
// Portado de psx_tomba (camera.c, loadSpawnPosition); licencia MIT del proyecto original.
#include "gv2.h"
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern int** D_80079474[];


typedef struct {
    int x;
    int y;
    int z;
} VEC3;

extern VEC3 D_8009C61C;






//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/render/camera", getBaseMatrix);

void func_80024BD4(u8* self);

void loadSpawnPosition(u8* self)
{
    s16* row = (s16*)((u8*)D_80079474[GAME.selectedArea][GAME.selectedSection] + (u16)D_8009BCEA * 8);

    *(int*)(self + 0xDC) = *row++ << 16;
    *(int*)(self + 0xE0) = *row << 16;
    *(int*)(self + 0xE4) = row[1] << 16;
    switch (GAME.selectedArea) {
    case 0:
        if (GV2.baf == 0 && GAME.selectedSection == 0) {
            *(s16*)(self + 0xDE) = 0x40;
        } else if (GAME.selectedSection == 3) {
            *(s16*)(self + 0xDE) = 0xD2;
        }
        break;
    case 2:
        if (GAME.selectedSection == 4) {
            *(s16*)(self + 0xDE) = 0x90;
        }
        break;
    case 4:
        if (GAME.selectedSection == 0xF) {
            *(s16*)(self + 0xDE) = 0x90;
        }
        break;
    case 10:
        if (GAME.selectedSection == 8) {
            *(s16*)(self + 0xDE) = 0x90;
        }
        break;
    }
}

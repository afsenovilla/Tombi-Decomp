// VERSION BETA_980203 beta980203 80023B70 316
#if defined(VERSION_BETA_980203)
// Beta version of src/loadSpawnPosition.c (loadSpawnPosition, PAL body): PLAYER spawn fields 0xec..0xf4 are at 0xe0..0xe8.
#include "gv2.h"
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern int** SPAWN_TABLES[];


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
    s16* row = (s16*)((u8*)SPAWN_TABLES[GAME.selectedArea][GAME.selectedSection] + (u16)GAME_SELECTED_SPAWN_POINT * 8);

    *(int*)(self + 0xE0) = *row++ << 16;
    *(int*)(self + 0xE4) = *row << 16;
    *(int*)(self + 0xE8) = row[1] << 16;
    switch (GAME.selectedArea) {
    case 0:
        if (GV2.baf == 0 && GAME.selectedSection == 0) {
            *(s16*)(self + 0xE2) = 0x40;
        } else if (GAME.selectedSection == 3) {
            *(s16*)(self + 0xE2) = 0xD2;
        }
        break;
    case 2:
        if (GAME.selectedSection == 4) {
            *(s16*)(self + 0xE2) = 0x90;
        }
        break;
    case 4:
        if (GAME.selectedSection == 0xF) {
            *(s16*)(self + 0xE2) = 0x90;
        }
        break;
    case 10:
        if (GAME.selectedSection == 8) {
            *(s16*)(self + 0xE2) = 0x90;
        }
        break;
    }
}
#endif

// FUNC 8002171c 316 MAIN0
/* demoted: bytes match but some relocated addresses (globals/callees) differ from the game; run tools/ncheck.py to see which ("address of X differs"). Fix the extern names/offsets. */
// Portado de psx_tomba (camera.c, func_800242AC); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern int** D_8007BF78[];
extern u8 D_8009C617;


typedef struct {
    int x;
    int y;
    int z;
} VEC3;

extern VEC3 D_8009C61C;






//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/render/camera", getBaseMatrix);

void func_80024BD4(u8* self);

void func_800242AC(u8* self)
{
    s16* row = (s16*)((u8*)D_8007BF78[GAME.selectedArea][GAME.selectedSection] + (u16)D_8009BCEA * 8);

    *(int*)(self + 0xEC) = *row++ << 16;
    *(int*)(self + 0xF0) = *row << 16;
    *(int*)(self + 0xF4) = row[1] << 16;
    switch (GAME.selectedArea) {
    case 0:
        if (D_8009C617 == 0 && GAME.selectedSection == 0) {
            *(s16*)(self + 0xEE) = 0x40;
        } else if (GAME.selectedSection == 3) {
            *(s16*)(self + 0xEE) = 0xD2;
        }
        break;
    case 2:
        if (GAME.selectedSection == 4) {
            *(s16*)(self + 0xEE) = 0x90;
        }
        break;
    case 4:
        if (GAME.selectedSection == 0xF) {
            *(s16*)(self + 0xEE) = 0x90;
        }
        break;
    case 10:
        if (GAME.selectedSection == 8) {
            *(s16*)(self + 0xEE) = 0x90;
        }
        break;
    }
}

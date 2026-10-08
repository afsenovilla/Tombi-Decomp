// FUNC 80021a94 88 MAIN0
// MATCHING 80021a94 88
// Ported from psx_tomba (camera.c, loadSectionBounds); MIT licence of the original project.
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

void applyLighting(u8* self);

void loadSectionBounds(u8* self)
{
    u16* row = (u16*)(D_8007B680[GAME.selectedArea] + D_8009BCCA * 8);

    *(u16*)(self + 0x2C) = *row++;
    *(u16*)(self + 0x2E) = *row++;
    *(u16*)(self + 0x30) = *row;
    *(u16*)(self + 0x32) = row[1];
}

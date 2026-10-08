// FUNC 80021858 572 MAIN0
// MATCHING 80021858 572
// Portado de psx_tomba (camera.c, func_800243E8); licencia MIT del proyecto original.
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

void func_800243E8(void)
{
    u8* p = D_800A5398;
    s16* row;
    u_int flags;
    u_int plane;

    p[0] = 3;
    if ((CURRENT_TASK)->loadGameSelected != 0) {
        D_8009C618 = 4;
        if (*(u_long*)&GAME.selectedArea == 0x20000) {
            D_8009BCEA = 2;
        }
    }
    row = (s16*)((u8*)D_8007BF78[GAME.selectedArea][GAME.selectedSection] + (u16)D_8009BCEA * 8);
    *(int*)(p + 0x10) = *row++ << 16;
    *(int*)(p + 0x14) = *row++ << 16;
    *(int*)(p + 0x18) = *row << 16;
    flags = (u16)row[1];
    plane = flags >> 8;
    flags &= 1;
    *(u16*)0x1F8001C8 = flags;
    GAME.area00_fogControl = plane;
    if (flags == 0) {
        *(u8**)(p + 0x40) = p + 0x10;
        *(u8**)(p + 0x44) = p + 0x18;
    } else {
        *(u8**)(p + 0x44) = p + 0x10;
        *(u8**)(p + 0x40) = p + 0x18;
    }
    GAME.selectedPlane = (s16)(*(u16**)(p + 0x44))[1] / 90;
    {
        u8*  r = D_8007C110[GAME.selectedArea] + GAME.selectedSection * 2;
        u16* dst = (u16*)((u8*)&GAME + 0x964 + r[0] * 2);

        *dst |= 1 << r[1];
    }
    if (D_8009C618 == 3 || D_8009C617 == 0) {
        func_800242AC(p);
        *(s16*)(p + 0x12) = *(u16*)(p + 0xEE);
        *(s16*)(p + 0x16) -= 0x104;
    } else if ((CURRENT_TASK)->loadGameSelected != 0) {
        (CURRENT_TASK)->loadGameSelected = 0;
        *(VEC3*)&D_800A5398[0x10] = D_8009C61C;
    }
}

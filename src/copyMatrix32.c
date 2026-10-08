// FUNC 80021f18 68 MAIN0
// MATCHING 80021f18 68
// Ported from psx_tomba (camera.c, copyMatrix32); MIT licence of the original project.
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

void copyMatrix32(s32* src, s32* dst)
{
    s32 a, b, c, d;

    a = src[0];
    b = src[1];
    c = src[2];
    d = src[3];
    dst[0] = a;
    dst[1] = b;
    dst[2] = c;
    dst[3] = d;
    a = src[4];
    b = src[5];
    c = src[6];
    d = src[7];
    dst[4] = a;
    dst[5] = b;
    dst[6] = c;
    dst[7] = d;
}

// FUNC 80057610 424 MAIN0
// MATCHING 80057610 424
// VERSION SLPS_01144 slps01144 8004AA4C 424
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", func_8004AA4C_slps01144);
#else
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct {
    char p0[0xf]; signed char f; char p1[2]; unsigned short x; char p2[2]; unsigned short y; char p3[2]; unsigned short z;
    char p4[0x84 - 0x1c]; int a84, a88, a8c; char p5[0xa0 - 0x90]; int aa0; unsigned char ba4; char p6[3]; int aa8;
} O;
extern MATRIX D_1f800000, D_1f800020, VIEW_CAMERA;
extern SVECTOR D_1f800060, D_1f800068;
extern unsigned short SPAWN_AXIS_SWAP;
extern int *CURRENT_OT;
extern void getBaseMatrix(MATRIX *);
extern void FUN_8006424c(SVECTOR *, MATRIX *);
extern void FUN_8006395c(MATRIX *, MATRIX *, MATRIX *);
extern void FUN_80063bcc(SVECTOR *, int *);
extern void FUN_80063ddc(MATRIX *);
extern void FUN_80063e6c(MATRIX *);
extern void drawModelVtx(int, int *, O *, int, int);

void drawObjectModelRotatedVtx(O *o)
{
    getBaseMatrix(&D_1f800020);
    switch (SPAWN_AXIS_SWAP & 1) {
    case 0:
        D_1f800060.vx = o->a84;
        D_1f800060.vy = o->a88;
        D_1f800060.vz = o->a8c;
        break;
    case 1:
        D_1f800060.vx = o->a8c;
        D_1f800060.vy = o->a88;
        D_1f800060.vz = o->a84;
        break;
    }
    FUN_8006424c(&D_1f800060, &D_1f800020);
    D_1f800068.vx = o->x;
    D_1f800068.vy = o->y;
    D_1f800068.vz = o->z;
    FUN_8006395c(&VIEW_CAMERA, &D_1f800020, &D_1f800000);
    FUN_80063bcc(&D_1f800068, D_1f800000.t);
    D_1f800000.t[0] += VIEW_CAMERA.t[0];
    D_1f800000.t[1] += VIEW_CAMERA.t[1];
    D_1f800000.t[2] += VIEW_CAMERA.t[2];
    FUN_80063ddc(&D_1f800000);
    FUN_80063e6c(&D_1f800000);
    drawModelVtx(o->aa0, &CURRENT_OT[o->f + 4], o, o->aa8, o->ba4);
}
#endif

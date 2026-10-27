// FUNC 80057368 264 MAIN0
// MATCHING 80057368 264
// VERSION SLPM_80165 slpm80165 80046370 276
// VERSION SLPS_01144 slps01144 8004A7A4 264
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80057368 (FUN_80057368.c): also calls func_8003F160(o, 0) at the end.
#include "raw7.h"
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { int vx, vy, vz, pad; } VECTOR;
extern MATRIX M;
extern MATRIX VIEW_CAMERA;
extern short SV[3];
extern void f0(void *), SetRotMatrix(void *), ApplyRotMatrix(void *, void *), SetTransMatrix(void *), f1(int, int, void *, int);
extern char *CURRENT_OT;
extern void func_8003F160(char *, int);
void FUN_80057368(char *o)
{
    f0(&M);
    SV[0] = U16(o, 0x12);
    SV[1] = U16(o, 0x16);
    SV[2] = U16(o, 0x1a);
    SetRotMatrix(&VIEW_CAMERA);
    ApplyRotMatrix(SV, &M.t[0]);
    M.t[0] += VIEW_CAMERA.t[0];
    M.t[1] += VIEW_CAMERA.t[1];
    M.t[2] += VIEW_CAMERA.t[2];
    SetTransMatrix(&M);
    f1(S32(o, 0xa0), (int)(CURRENT_OT + ((S8(o, 0xf) << 2) + 0x10)), o, U8(o, 0xa4));
    func_8003F160(o, 0);
}
#elif defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", func_8004A7A4_slps01144);
#else
#include "raw7.h"
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { int vx, vy, vz, pad; } VECTOR;
extern MATRIX M;
extern MATRIX VIEW_CAMERA;
extern short SV[3];
extern void f0(void *), SetRotMatrix(void *), ApplyRotMatrix(void *, void *), SetTransMatrix(void *), f1(int, int, void *, int);
extern char *CURRENT_OT;
void drawObjectModelNoRot(char *o)
{
    f0(&M);
    SV[0] = U16(o, 0x12);
    SV[1] = U16(o, 0x16);
    SV[2] = U16(o, 0x1a);
    SetRotMatrix(&VIEW_CAMERA);
    ApplyRotMatrix(SV, &M.t[0]);
    M.t[0] += VIEW_CAMERA.t[0];
    M.t[1] += VIEW_CAMERA.t[1];
    M.t[2] += VIEW_CAMERA.t[2];
    SetTransMatrix(&M);
    f1(S32(o, 0xa0), (int)(CURRENT_OT + ((S8(o, 0xf) << 2) + 0x10)), o, U8(o, 0xa4));
}
#endif

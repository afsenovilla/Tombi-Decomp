// FUNC 80057470 416 MAIN0
// MATCHING 80057470 416
// VERSION SLPM_80165 slpm80165 80046484 428
// VERSION SLPS_01144 slps01144 8004A8AC 416
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80057470 (FUN_80057470.c): also calls func_8003F160(o, 0) at the end.
#include "TOBJ.H"
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
extern MATRIX DAT_1f800000, DAT_1f800020, DAT_1f8000c0;
extern SVECTOR DAT_1f800060, DAT_1f800068;
extern unsigned short SPAWN_AXIS_SWAP;
extern int CURRENT_OT;
extern void getBaseMatrix(MATRIX *);
extern MATRIX *RotMatrix(SVECTOR *, MATRIX *);
extern MATRIX *MulMatrix0(MATRIX *, MATRIX *, MATRIX *);
extern VECTOR *ApplyRotMatrix(SVECTOR *, long *);
extern void SetRotMatrix(MATRIX *);
extern void SetTransMatrix(MATRIX *);
extern void FUN_80023b04(int, int, TObj *, int);
extern void func_8003F160(TObj *, int);

void FUN_80057470(TObj *o)
{
    getBaseMatrix(&DAT_1f800020);
    switch (SPAWN_AXIS_SWAP & 1) {
    case 0:
        DAT_1f800060.vx = o->d84;
        DAT_1f800060.vy = o->d88;
        DAT_1f800060.vz = o->d8c;
        break;
    case 1:
        DAT_1f800060.vx = o->d8c;
        DAT_1f800060.vy = o->d88;
        DAT_1f800060.vz = o->d84;
        break;
    }
    RotMatrix(&DAT_1f800060, &DAT_1f800020);
    DAT_1f800068.vx = o->a.p.whole;
    DAT_1f800068.vy = o->y.p.whole;
    DAT_1f800068.vz = o->b.p.whole;
    MulMatrix0(&DAT_1f8000c0, &DAT_1f800020, &DAT_1f800000);
    ApplyRotMatrix(&DAT_1f800068, DAT_1f800000.t);
    DAT_1f800000.t[0] += DAT_1f8000c0.t[0];
    DAT_1f800000.t[1] += DAT_1f8000c0.t[1];
    DAT_1f800000.t[2] += DAT_1f8000c0.t[2];
    SetRotMatrix(&DAT_1f800000);
    SetTransMatrix(&DAT_1f800000);
    FUN_80023b04(o->da0, (int)&((struct { char pad[0x10]; int a[1]; } *)CURRENT_OT)->a[(signed char)o->b0f], o, o->ba4);
    func_8003F160(o, 0);
}
#elif defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", func_8004A8AC_slps01144);
#else
#include "TOBJ.H"
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
extern MATRIX DAT_1f800000, DAT_1f800020, DAT_1f8000c0;
extern SVECTOR DAT_1f800060, DAT_1f800068;
extern unsigned short SPAWN_AXIS_SWAP;
extern int CURRENT_OT;
extern void getBaseMatrix(MATRIX *);
extern MATRIX *RotMatrix(SVECTOR *, MATRIX *);
extern MATRIX *MulMatrix0(MATRIX *, MATRIX *, MATRIX *);
extern VECTOR *ApplyRotMatrix(SVECTOR *, long *);
extern void SetRotMatrix(MATRIX *);
extern void SetTransMatrix(MATRIX *);
extern void drawModel(int, int, TObj *, int);

void drawObjectModelRotated(TObj *o)
{
    getBaseMatrix(&DAT_1f800020);
    switch (SPAWN_AXIS_SWAP & 1) {
    case 0:
        DAT_1f800060.vx = o->d84;
        DAT_1f800060.vy = o->d88;
        DAT_1f800060.vz = o->d8c;
        break;
    case 1:
        DAT_1f800060.vx = o->d8c;
        DAT_1f800060.vy = o->d88;
        DAT_1f800060.vz = o->d84;
        break;
    }
    RotMatrix(&DAT_1f800060, &DAT_1f800020);
    DAT_1f800068.vx = o->a.p.whole;
    DAT_1f800068.vy = o->y.p.whole;
    DAT_1f800068.vz = o->b.p.whole;
    MulMatrix0(&DAT_1f8000c0, &DAT_1f800020, &DAT_1f800000);
    ApplyRotMatrix(&DAT_1f800068, DAT_1f800000.t);
    DAT_1f800000.t[0] += DAT_1f8000c0.t[0];
    DAT_1f800000.t[1] += DAT_1f8000c0.t[1];
    DAT_1f800000.t[2] += DAT_1f8000c0.t[2];
    SetRotMatrix(&DAT_1f800000);
    SetTransMatrix(&DAT_1f800000);
    drawModel(o->da0, (int)&((struct { char pad[0x10]; int a[1]; } *)CURRENT_OT)->a[(signed char)o->b0f], o, o->ba4);
}
#endif

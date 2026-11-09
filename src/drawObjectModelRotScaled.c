// FUNC 800577b8 496 MAIN0
// MATCHING 800577b8 496
#include "TOBJ.H"
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
extern MATRIX DAT_1f800000;
extern MATRIX OBJECT_MATRIX;
extern VECTOR OBJECT_SCALE;
extern SVECTOR WORK_VEC_A;
extern SVECTOR WORK_VEC_B;
extern MATRIX VIEW_CAMERA;
extern unsigned short SPAWN_AXIS_SWAP;
extern char *CURRENT_OT;
extern void getBaseMatrix(MATRIX *);
extern void FUN_8006481c(long, MATRIX *);
extern void FUN_800644dc(long, MATRIX *);
extern void FUN_8006467c(long, MATRIX *);
extern void FUN_8006395c(MATRIX *, MATRIX *, MATRIX *);
extern void FUN_80063bcc(SVECTOR *, long *);
extern void FUN_80063cac(MATRIX *, VECTOR *);
extern void FUN_80063ddc(MATRIX *);
extern void FUN_80063e6c(MATRIX *);
extern void drawModel(int, char *, TObj *, int);

void drawObjectModelRotScaled(TObj *o)
{
    getBaseMatrix(&OBJECT_MATRIX);
    switch (SPAWN_AXIS_SWAP & 1) {
    case 0:
        WORK_VEC_A.vx = o->d84;
        WORK_VEC_A.vy = o->d88;
        WORK_VEC_A.vz = o->d8c;
        break;
    case 1:
        WORK_VEC_A.vx = o->d8c;
        WORK_VEC_A.vy = o->d88;
        WORK_VEC_A.vz = o->d84;
        break;
    }
    OBJECT_SCALE.vx = o->w74;
    OBJECT_SCALE.vy = o->w76;
    OBJECT_SCALE.vz = o->w78;
    FUN_8006481c(o->d8c, &OBJECT_MATRIX);
    FUN_800644dc(o->d84, &OBJECT_MATRIX);
    FUN_8006467c(o->d88, &OBJECT_MATRIX);
    WORK_VEC_B.vx = o->a.p.whole;
    WORK_VEC_B.vy = o->y.p.whole;
    WORK_VEC_B.vz = o->b.p.whole;
    FUN_8006395c(&VIEW_CAMERA, &OBJECT_MATRIX, &DAT_1f800000);
    FUN_80063bcc(&WORK_VEC_B, DAT_1f800000.t);
    DAT_1f800000.t[0] += VIEW_CAMERA.t[0];
    DAT_1f800000.t[1] += VIEW_CAMERA.t[1];
    DAT_1f800000.t[2] += VIEW_CAMERA.t[2];
    FUN_80063cac(&DAT_1f800000, &OBJECT_SCALE);
    FUN_80063ddc(&DAT_1f800000);
    FUN_80063e6c(&DAT_1f800000);
    drawModel(o->da0, CURRENT_OT + ((signed char)o->b0f * 4 + 0x10), o, o->ba4);
}

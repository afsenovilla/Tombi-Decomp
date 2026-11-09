// FUNC 80057ae4 268 MAIN0
// MATCHING 80057ae4 268
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { short m[3][3]; int t[3]; } MATR;
extern MATR DAT_1f800000;
extern MATR VIEW_CAMERA;
extern SVEC WORK_VEC_A;
extern int *CURRENT_OT;
extern void MulMatrix0(MATR *, MATR *, MATR *);
extern void ApplyRotMatrix(SVEC *, int *);
extern void SetRotMatrix(MATR *);
extern void SetTransMatrix(MATR *);
extern void drawModel(int, int *, TObj *, int);

void drawObjectModelOwnMatrix(TObj *o)
{
    WORK_VEC_A.vx = o->a.p.whole;
    WORK_VEC_A.vy = o->y.p.whole;
    WORK_VEC_A.vz = o->b.p.whole;
    MulMatrix0(&VIEW_CAMERA, (MATR *)&o->w48, &DAT_1f800000);
    ApplyRotMatrix(&WORK_VEC_A, DAT_1f800000.t);
    DAT_1f800000.t[0] += VIEW_CAMERA.t[0];
    DAT_1f800000.t[1] += VIEW_CAMERA.t[1];
    DAT_1f800000.t[2] += VIEW_CAMERA.t[2];
    SetRotMatrix(&DAT_1f800000);
    SetTransMatrix(&DAT_1f800000);
    drawModel(o->da0, &CURRENT_OT[*(signed char *)&o->b0f + 4], o, o->ba4);
}

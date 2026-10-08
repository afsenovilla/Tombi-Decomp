// FUNC 80057ae4 268 MAIN0
// MATCHING 80057ae4 268
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { short m[3][3]; int t[3]; } MATR;
extern MATR DAT_1f800000;
extern MATR DAT_1f8000c0;
extern SVEC DAT_1f800060;
extern int *DAT_1f8001e0;
extern void MulMatrix0(MATR *, MATR *, MATR *);
extern void ApplyRotMatrix(SVEC *, int *);
extern void SetRotMatrix(MATR *);
extern void SetTransMatrix(MATR *);
extern void FUN_80023b04(int, int *, TObj *, int);

void FUN_80057ae4(TObj *o)
{
    DAT_1f800060.vx = o->a.p.whole;
    DAT_1f800060.vy = o->y.p.whole;
    DAT_1f800060.vz = o->b.p.whole;
    MulMatrix0(&DAT_1f8000c0, (MATR *)&o->w48, &DAT_1f800000);
    ApplyRotMatrix(&DAT_1f800060, DAT_1f800000.t);
    DAT_1f800000.t[0] += DAT_1f8000c0.t[0];
    DAT_1f800000.t[1] += DAT_1f8000c0.t[1];
    DAT_1f800000.t[2] += DAT_1f8000c0.t[2];
    SetRotMatrix(&DAT_1f800000);
    SetTransMatrix(&DAT_1f800000);
    FUN_80023b04(o->da0, &DAT_1f8001e0[*(signed char *)&o->b0f + 4], o, o->ba4);
}

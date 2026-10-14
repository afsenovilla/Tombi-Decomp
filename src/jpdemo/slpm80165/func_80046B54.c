// JPDEMO slpm80165 80046B54 280
// Demo version of PAL main0 80057368 (FUN_80057368.c): rotation from the object's own matrix at +0x48 (MulMatrix0),
// then func_8003F160.
#include "raw7.h"
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { int vx, vy, vz, pad; } VECTOR;
extern MATRIX M;
extern MATRIX R;
extern short SV[3];
extern void f0(void *), SetRotMatrix(void *), ApplyRotMatrix(void *, void *), SetTransMatrix(void *), f1(int, int, void *, int);
extern char *frame;
extern void MulMatrix0(void *, void *, void *);
extern void func_8003F160(char *, int);
void FUN_80057368(char *o)
{
    SV[0] = U16(o, 0x12);
    SV[1] = U16(o, 0x16);
    SV[2] = U16(o, 0x1a);
    MulMatrix0(&R, o + 0x48, &M);
    ApplyRotMatrix(SV, &M.t[0]);
    M.t[0] += R.t[0];
    M.t[1] += R.t[1];
    M.t[2] += R.t[2];
    SetRotMatrix(&M);
    SetTransMatrix(&M);
    f1(S32(o, 0xa0), (int)(frame + ((S8(o, 0xf) << 2) + 0x10)), o, U8(o, 0xa4));
    func_8003F160(o, 0);
}

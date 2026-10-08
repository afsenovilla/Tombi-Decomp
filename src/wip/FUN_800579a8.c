// FUNC 800579a8 316 MAIN0
#include "raw7.h"
typedef struct { short m[3][3]; int t[3]; } MATRIX;
extern MATRIX M;
extern MATRIX A;
extern MATRIX B;
extern short SV[3];
extern short SV2[3];
extern void f0(void *), RotMatrix(void *, void *), MulMatrix0(void *, void *, void *), SetRotMatrix(void *), ApplyRotMatrix(void *, void *), SetTransMatrix(void *), f1(int, int);
extern char *frame;
void FUN_800579a8(char *o)
{
    f0(&A);
    f0(&B);
    SV[0] = S32(o, 0x84);
    SV[1] = S32(o, 0x88);
    SV[2] = S32(o, 0x8c);
    RotMatrix(SV, &A);
    SV2[0] = U16(o, 0x12);
    SV2[1] = U16(o, 0x16);
    SV2[2] = U16(o, 0x1a);
    MulMatrix0(&B, &A, &M);
    ApplyRotMatrix(SV2, &M.t[0]);
    M.t[0] += -0xa0;
    M.t[1] += 0x80;
    M.t[2] += 0x220;
    SetRotMatrix(&M);
    SetTransMatrix(&M);
    f1(S32(o, 0xa0), (int)(frame + ((S8(o, 0xf) << 2) + 0x10)));
}

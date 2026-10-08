// FUNC 80057bf0 276 MAIN0
// MATCHING 80057bf0 276
#include "raw7.h"
typedef struct { short m[3][3]; int t[3]; } MATRIX;
extern MATRIX M;
extern MATRIX R;
extern short SV[3];
extern void MulMatrix0(void *, void *, void *), SetRotMatrix(void *), ApplyRotMatrix(void *, void *), SetTransMatrix(void *), f1(int, int, void *, int, int);
extern char *frame;
void FUN_80057bf0(char *o)
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
    f1(S32(o, 0xa0), (int)(frame + ((S8(o, 0xf) << 2) + 0x10)), o, S32(o, 0xa8), U8(o, 0xa4));
}

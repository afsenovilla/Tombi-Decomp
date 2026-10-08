// FUNC 80057ae4 268 MAIN0
#include "TOBJ.H"
extern short DAT_1f800060, DAT_1f800062, DAT_1f800064;
extern int DAT_1f800014, DAT_1f800018, DAT_1f80001c;
extern int DAT_1f8000d4, DAT_1f8000d8, DAT_1f8000dc;
extern int DAT_1f8001e0;
extern char DAT_1f800000[], DAT_1f8000c0[];
extern void MulMatrix0();
extern void ApplyRotMatrix();
extern void SetRotMatrix();
extern void SetTransMatrix();
extern void FUN_80023b04();

void FUN_80057ae4(TObj *o)
{
    DAT_1f800060 = o->a.p.whole;
    DAT_1f800062 = o->y.p.whole;
    DAT_1f800064 = o->b.p.whole;
    MulMatrix0(DAT_1f8000c0, &o->w48, DAT_1f800000);
    ApplyRotMatrix(&DAT_1f800060, &DAT_1f800014);
    DAT_1f800014 = DAT_1f800014 + DAT_1f8000d4;
    DAT_1f800018 = DAT_1f800018 + DAT_1f8000d8;
    DAT_1f80001c = DAT_1f80001c + DAT_1f8000dc;
    SetRotMatrix(DAT_1f800000);
    SetTransMatrix(DAT_1f800000);
    FUN_80023b04(o->da0, DAT_1f8001e0 + (char)o->b0f * 4 + 0x10, o, o->ba4);
}

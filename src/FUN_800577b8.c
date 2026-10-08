// FUNC 800577b8 496 MAIN0
// MATCHING 800577b8 496
#include "TOBJ.H"
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
extern MATRIX DAT_1f800000;
extern MATRIX DAT_1f800020;
extern VECTOR DAT_1f800040;
extern SVECTOR DAT_1f800060;
extern SVECTOR DAT_1f800068;
extern MATRIX DAT_1f8000c0;
extern unsigned short DAT_1f8001c8;
extern char *DAT_1f8001e0;
extern void FUN_80021f5c(MATRIX *);
extern void FUN_8006481c(long, MATRIX *);
extern void FUN_800644dc(long, MATRIX *);
extern void FUN_8006467c(long, MATRIX *);
extern void FUN_8006395c(MATRIX *, MATRIX *, MATRIX *);
extern void FUN_80063bcc(SVECTOR *, long *);
extern void FUN_80063cac(MATRIX *, VECTOR *);
extern void FUN_80063ddc(MATRIX *);
extern void FUN_80063e6c(MATRIX *);
extern void FUN_80023b04(int, char *, TObj *, int);

void FUN_800577b8(TObj *o)
{
    FUN_80021f5c(&DAT_1f800020);
    switch (DAT_1f8001c8 & 1) {
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
    DAT_1f800040.vx = o->w74;
    DAT_1f800040.vy = o->w76;
    DAT_1f800040.vz = o->w78;
    FUN_8006481c(o->d8c, &DAT_1f800020);
    FUN_800644dc(o->d84, &DAT_1f800020);
    FUN_8006467c(o->d88, &DAT_1f800020);
    DAT_1f800068.vx = o->a.p.whole;
    DAT_1f800068.vy = o->y.p.whole;
    DAT_1f800068.vz = o->b.p.whole;
    FUN_8006395c(&DAT_1f8000c0, &DAT_1f800020, &DAT_1f800000);
    FUN_80063bcc(&DAT_1f800068, DAT_1f800000.t);
    DAT_1f800000.t[0] += DAT_1f8000c0.t[0];
    DAT_1f800000.t[1] += DAT_1f8000c0.t[1];
    DAT_1f800000.t[2] += DAT_1f8000c0.t[2];
    FUN_80063cac(&DAT_1f800000, &DAT_1f800040);
    FUN_80063ddc(&DAT_1f800000);
    FUN_80063e6c(&DAT_1f800000);
    FUN_80023b04(o->da0, DAT_1f8001e0 + ((signed char)o->b0f * 4 + 0x10), o, o->ba4);
}

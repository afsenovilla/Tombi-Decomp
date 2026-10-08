// FUNC 80116254 356 X006
// MATCHING 80116254 356
typedef struct { short m[3][3]; short pad; int t[3]; } MAT;
typedef struct { unsigned short frac; short whole; } FP;
typedef union { int raw; FP p; } FX;
typedef struct { MAT m; FX pos[6]; } CAM;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct O { char p0[8]; int w08; int w0c; int w10; char p1[0x20-0x14]; int w20; int w24;
  char p2[0x34-0x28]; int *d34; int *d38; char p3[0x78-0x3c]; int w78; int w7c; } O;
extern CAM DAT_1f8000c0;
#define CAMP (&DAT_1f8000c0)
extern SVEC DAT_1f800060;
extern SVEC DAT_1f800068;
extern void FUN_80021f5c(MAT *);
extern void RotMatrixY(long, MAT *);
extern void RotMatrixX(long, MAT *);
extern void ApplyMatrixSV(MAT *, SVEC *, SVEC *);

void func_80116254(O *o)
{
    o->w08 = *o->d34 - 0xa00000;
    o->w0c = CAMP->pos[4].raw + 0x800000;
    o->w10 = *o->d38;
    FUN_80021f5c(&CAMP->m);
    RotMatrixY(o->w24 + (o->w7c << 4) / 360, &CAMP->m);
    RotMatrixX(o->w20, &CAMP->m);
    CAMP->m.t[0] = -CAMP->pos[0].p.whole;
    CAMP->m.t[1] = -CAMP->pos[1].p.whole;
    CAMP->m.t[2] = -CAMP->pos[2].p.whole;
    DAT_1f800060.vx = -CAMP->pos[3].p.whole;
    DAT_1f800060.vy = -CAMP->pos[4].p.whole;
    DAT_1f800060.vz = -CAMP->pos[5].p.whole;
    ApplyMatrixSV(&CAMP->m, &DAT_1f800060, &DAT_1f800068);
    CAMP->m.t[0] += DAT_1f800068.vx;
    CAMP->m.t[1] += DAT_1f800068.vy;
    CAMP->m.t[2] += DAT_1f800068.vz;
}

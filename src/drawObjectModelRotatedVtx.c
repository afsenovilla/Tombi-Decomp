// FUNC 80057610 424 MAIN0
// MATCHING 80057610 424
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct {
    char p0[0xf]; signed char f; char p1[2]; unsigned short x; char p2[2]; unsigned short y; char p3[2]; unsigned short z;
    char p4[0x84 - 0x1c]; int a84, a88, a8c; char p5[0xa0 - 0x90]; int aa0; unsigned char ba4; char p6[3]; int aa8;
} O;
extern MATRIX D_1f800000, OBJECT_MATRIX, VIEW_CAMERA;
extern SVECTOR WORK_VEC_A, WORK_VEC_B;
extern unsigned short SPAWN_AXIS_SWAP;
extern int *CURRENT_OT;
extern void getBaseMatrix(MATRIX *);
extern void FUN_8006424c(SVECTOR *, MATRIX *);
extern void FUN_8006395c(MATRIX *, MATRIX *, MATRIX *);
extern void FUN_80063bcc(SVECTOR *, int *);
extern void FUN_80063ddc(MATRIX *);
extern void FUN_80063e6c(MATRIX *);
extern void drawModelVtx(int, int *, O *, int, int);

void drawObjectModelRotatedVtx(O *o)
{
    getBaseMatrix(&OBJECT_MATRIX);
    switch (SPAWN_AXIS_SWAP & 1) {
    case 0:
        WORK_VEC_A.vx = o->a84;
        WORK_VEC_A.vy = o->a88;
        WORK_VEC_A.vz = o->a8c;
        break;
    case 1:
        WORK_VEC_A.vx = o->a8c;
        WORK_VEC_A.vy = o->a88;
        WORK_VEC_A.vz = o->a84;
        break;
    }
    FUN_8006424c(&WORK_VEC_A, &OBJECT_MATRIX);
    WORK_VEC_B.vx = o->x;
    WORK_VEC_B.vy = o->y;
    WORK_VEC_B.vz = o->z;
    FUN_8006395c(&VIEW_CAMERA, &OBJECT_MATRIX, &D_1f800000);
    FUN_80063bcc(&WORK_VEC_B, D_1f800000.t);
    D_1f800000.t[0] += VIEW_CAMERA.t[0];
    D_1f800000.t[1] += VIEW_CAMERA.t[1];
    D_1f800000.t[2] += VIEW_CAMERA.t[2];
    FUN_80063ddc(&D_1f800000);
    FUN_80063e6c(&D_1f800000);
    drawModelVtx(o->aa0, &CURRENT_OT[o->f + 4], o, o->aa8, o->ba4);
}

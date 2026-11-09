// FUNC 800e8044 336 G0OVER
// MATCHING 800e8044 336
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { MATRIX m; short xl, x, yl, y, zl, z, rxl, rx, ryl, ry, rzl, rz; } CAMERA;
extern void getBaseMatrix(MATRIX *);
extern void ApplyMatrixSV(MATRIX *, SVECTOR *, SVECTOR *);
extern SVECTOR WORK_VEC_A, WORK_VEC_B;
extern CAMERA VIEW_CAMERA;

void gameOverSetupCamera(void)
{
    VIEW_CAMERA.x = 0;
    VIEW_CAMERA.z = -0x220;
    VIEW_CAMERA.rx = 0xA0;
    VIEW_CAMERA.y = 0;
    VIEW_CAMERA.ry = -0x78;
    VIEW_CAMERA.rz = 0;
    getBaseMatrix(&VIEW_CAMERA.m);
    VIEW_CAMERA.m.t[0] = -VIEW_CAMERA.x;
    VIEW_CAMERA.m.t[1] = -VIEW_CAMERA.y;
    VIEW_CAMERA.m.t[2] = -VIEW_CAMERA.z;
    WORK_VEC_A.vx = -VIEW_CAMERA.rx;
    WORK_VEC_A.vy = -VIEW_CAMERA.ry;
    WORK_VEC_A.vz = -VIEW_CAMERA.rz;
    ApplyMatrixSV(&VIEW_CAMERA.m, &WORK_VEC_A, &WORK_VEC_B);
    VIEW_CAMERA.m.t[0] += WORK_VEC_B.vx;
    VIEW_CAMERA.m.t[1] += WORK_VEC_B.vy;
    VIEW_CAMERA.m.t[2] += WORK_VEC_B.vz;
}

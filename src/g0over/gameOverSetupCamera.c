// FUNC 800e8044 336 G0OVER
// MATCHING 800e8044 336
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { MATRIX m; short xl, x, yl, y, zl, z, rxl, rx, ryl, ry, rzl, rz; } CAMERA;
extern void getBaseMatrix(MATRIX *);
extern void ApplyMatrixSV(MATRIX *, SVECTOR *, SVECTOR *);
extern SVECTOR D_1F800060, D_1F800068;
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
    D_1F800060.vx = -VIEW_CAMERA.rx;
    D_1F800060.vy = -VIEW_CAMERA.ry;
    D_1F800060.vz = -VIEW_CAMERA.rz;
    ApplyMatrixSV(&VIEW_CAMERA.m, &D_1F800060, &D_1F800068);
    VIEW_CAMERA.m.t[0] += D_1F800068.vx;
    VIEW_CAMERA.m.t[1] += D_1F800068.vy;
    VIEW_CAMERA.m.t[2] += D_1F800068.vz;
}

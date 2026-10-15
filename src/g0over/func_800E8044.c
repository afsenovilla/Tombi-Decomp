// FUNC 800e8044 336 G0OVER
// MATCHING 800e8044 336
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { MATRIX m; short xl, x, yl, y, zl, z, rxl, rx, ryl, ry, rzl, rz; } CAMERA;
extern void getBaseMatrix(MATRIX *);
extern void ApplyMatrixSV(MATRIX *, SVECTOR *, SVECTOR *);
extern SVECTOR D_1F800060, D_1F800068;
extern CAMERA D_1F8000C0;

void func_800E8044(void)
{
    D_1F8000C0.x = 0;
    D_1F8000C0.z = -0x220;
    D_1F8000C0.rx = 0xA0;
    D_1F8000C0.y = 0;
    D_1F8000C0.ry = -0x78;
    D_1F8000C0.rz = 0;
    getBaseMatrix(&D_1F8000C0.m);
    D_1F8000C0.m.t[0] = -D_1F8000C0.x;
    D_1F8000C0.m.t[1] = -D_1F8000C0.y;
    D_1F8000C0.m.t[2] = -D_1F8000C0.z;
    D_1F800060.vx = -D_1F8000C0.rx;
    D_1F800060.vy = -D_1F8000C0.ry;
    D_1F800060.vz = -D_1F8000C0.rz;
    ApplyMatrixSV(&D_1F8000C0.m, &D_1F800060, &D_1F800068);
    D_1F8000C0.m.t[0] += D_1F800068.vx;
    D_1F8000C0.m.t[1] += D_1F800068.vy;
    D_1F8000C0.m.t[2] += D_1F800068.vz;
}

// FUNC 800e8194 356 G0OVER
// MATCHING 800e8194 356
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz; } VECTOR;
typedef struct {
    MATRIX m;       /* 0x00 */
    MATRIX cm;      /* 0x20 */
    unsigned char r, g, b, p43;
    short s44, s46, s48;
} OB;
extern OB D_1F800118;
extern void getBaseMatrix(MATRIX *);
extern void SetFarColor(int r, int g, int b);
extern void SetBackColor(int r, int g, int b);
extern void SetColorMatrix(MATRIX *m);
extern void RotMatrixX(int a, MATRIX *m);
extern void RotMatrixY(int a, MATRIX *m);
extern void ApplyMatrix(MATRIX *m, SVECTOR *v, VECTOR *r);

void func_800E8194(void)
{
    SVECTOR v;
    VECTOR r;
    OB *o = &D_1F800118;
    getBaseMatrix(&o->cm);
    o->cm.m[0][0] = 0x1000;
    o->cm.m[1][0] = 0x1000;
    o->cm.m[2][0] = 0x1000;
    o->r = 0x80;
    o->g = 0x80;
    o->b = 0x80;
    SetFarColor(0, 0, 0);
    SetBackColor(o->r, o->g, o->b);
    SetColorMatrix(&o->cm);
    o->s44 = 0x638;
    o->s46 = 0x800;
    o->s48 = 0;
    getBaseMatrix(&o->m);
    RotMatrixX(o->s44, &o->m);
    RotMatrixY(o->s46, &o->m);
    v.vx = 0;
    v.vy = 0x1000;
    v.vz = 0;
    ApplyMatrix(&o->m, &v, &r);
    o->m.m[0][0] = r.vx;
    o->m.m[0][1] = r.vy;
    o->m.m[0][2] = r.vz;
    o->m.m[1][0] = 0;
    o->m.m[1][1] = 0;
    o->m.m[1][2] = 0;
    o->m.m[2][0] = 0;
    o->m.m[2][1] = 0;
    o->m.m[2][2] = 0;
}

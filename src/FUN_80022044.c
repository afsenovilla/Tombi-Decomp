// FUNC 80022044 272 MAIN0
// MATCHING 80022044 272
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz; } VECTOR;
typedef struct {
    MATRIX m;       /* 0x00 */
    MATRIX cm;      /* 0x20 */
    unsigned char r, g, b, p43;
    short s44, s46;
} OB;
extern MATRIX DAT_1f8000f8;
extern void SetFarColor(int r, int g, int b);
extern void SetBackColor(int r, int g, int b);
extern void SetColorMatrix(MATRIX *m);
extern void FUN_800644dc(int a, OB *o);
extern void FUN_8006467c(int a, OB *o);
extern void FUN_80063bfc(OB *o, SVECTOR *v, VECTOR *r);
extern void FUN_800635e8(int a, int b);

void FUN_80022044(OB *o)
{
    SVECTOR v;
    VECTOR r;
    SetFarColor(0xff, 0xff, 0xff);
    SetBackColor(o->r, o->g, o->b);
    SetColorMatrix(&o->cm);
    o->m = DAT_1f8000f8;
    FUN_800644dc(o->s44, o);
    FUN_8006467c(o->s46, o);
    v.vx = 0;
    v.vy = 0x1000;
    v.vz = 0;
    FUN_80063bfc(o, &v, &r);
    o->m.m[0][0] = r.vx;
    o->m.m[0][1] = r.vy;
    o->m.m[0][2] = r.vz;
    o->m.m[1][0] = 0;
    o->m.m[1][1] = 0;
    o->m.m[1][2] = 0;
    o->m.m[2][0] = 0;
    o->m.m[2][1] = 0;
    o->m.m[2][2] = 0;
    FUN_800635e8(0x118, 0x220);
}

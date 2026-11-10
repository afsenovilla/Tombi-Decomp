// VERSION PCPX_96084 tomba 8001ADB4 248
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of applyLighting (src/applyLighting.c): no far colour and no fog call, base matrix at 0x1F800268.
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz; } VECTOR;
typedef struct {
    MATRIX m;       /* 0x00 */
    MATRIX cm;      /* 0x20 */
    unsigned char r, g, b, p43;
    short s44, s46;
} OB;
extern MATRIX D_1F800268;
extern void SetBackColor(int r, int g, int b);
extern void SetColorMatrix(MATRIX *m);
extern void RotMatrixX(int a, OB *o);
extern void RotMatrixY(int a, OB *o);
extern void ApplyMatrix(OB *o, SVECTOR *v, VECTOR *r);

void func_8001ADB4_pcpx96084(OB *o)
{
    SVECTOR v;
    VECTOR r;
    SetBackColor(o->r, o->g, o->b);
    SetColorMatrix(&o->cm);
    o->m = D_1F800268;
    RotMatrixX(o->s44, o);
    RotMatrixY(o->s46, o);
    v.vx = 0;
    v.vy = 0x1000;
    v.vz = 0;
    ApplyMatrix(o, &v, &r);
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
#endif

// FUNC 8003511c 124 MAIN0
// MATCHING 8003511c 124
typedef struct P { short x; short y; } P;
typedef struct S { char p[0x16]; short z; char q[0x28]; P *pos; } S;
extern P *D_800A6078;
extern short D_800A604E;
extern int csqrt(int);
extern int abs(int);
short func_8003511C(S *o)
{
    int dx = abs(o->pos->y - D_800A6078->y);
    int dz = abs(o->z - D_800A604E);
    return csqrt((dx * dx + dz * dz) << 12) >> 12;
}

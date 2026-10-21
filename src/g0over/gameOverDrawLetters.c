// FUNC 800e87a0 476 G0OVER
// MATCHING 800e87a0 476
#include "TOBJ.H"
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { TObj o; char pad[0x14]; } GObj; /* 0xD4 bytes */
extern GObj D_800A6610[];
extern MATRIX D_1F800000, D_1F800020, D_1F8000C0;
extern SVECTOR D_1F800060, D_1F800068;
extern char *DAT_1f8001e0;
extern void getBaseMatrix(MATRIX *);
extern MATRIX *RotMatrix(SVECTOR *, MATRIX *);
extern MATRIX *MulMatrix0(MATRIX *, MATRIX *, MATRIX *);
extern void ApplyRotMatrix(SVECTOR *, long *);
extern void SetRotMatrix(MATRIX *);
extern void SetTransMatrix(MATRIX *);
extern char *func_80022ABC(char *f, char *vtx, char *ot);
extern char *func_80022C80(char *f, char *vtx, char *ot);
extern char *func_80022EA8(char *f, char *vtx, char *ot);
extern char *func_80023084(char *f, char *vtx, char *ot);

void gameOverDrawLetters(void)
{
    short i;
    TObj *o;
    char *m;
    char *ot;

    i = 0;
    do {
        o = &D_800A6610[i].o;
        getBaseMatrix(&D_1F800020);
        D_1F800060.vx = o->d84;
        D_1F800060.vy = o->d88;
        D_1F800060.vz = o->d8c;
        RotMatrix(&D_1F800060, &D_1F800020);
        D_1F800068.vx = o->a.p.whole;
        D_1F800068.vy = o->y.p.whole;
        D_1F800068.vz = o->b.p.whole;
        MulMatrix0(&D_1F8000C0, &D_1F800020, &D_1F800000);
        ApplyRotMatrix(&D_1F800068, D_1F800000.t);
        D_1F800000.t[0] += D_1F8000C0.t[0];
        D_1F800000.t[1] += D_1F8000C0.t[1];
        D_1F800000.t[2] += D_1F8000C0.t[2];
        SetRotMatrix(&D_1F800000);
        SetTransMatrix(&D_1F800000);
        ot = DAT_1f8001e0 + 0x10;
        m = func_80022ABC((char *)o->da0 + 0x10, (char *)o->da0 + 0x14, ot);
        m = func_80022C80(m, m + 4, ot);
        m = func_80022EA8(m, m + 4, ot);
        func_80023084(m, m + 4, ot);
        i++;
    } while (i < 8);
}

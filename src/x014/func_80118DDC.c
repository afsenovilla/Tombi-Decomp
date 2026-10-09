// FUNC 80118ddc 756 X014
// MATCHING 80118ddc 756
#include "TOBJ.H"
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
extern MATRIX D_1f800000, D_1f800020, D_1f8000c0;
extern SVECTOR D_1f800068;
extern long D_1F800070;
extern char *D_1F800164;
extern int D_1F8001E0;
extern void FUN_80021f5c(MATRIX *);
extern void FUN_8006481c(int, MATRIX *);
extern void FUN_800644dc(int, MATRIX *);
extern void FUN_8006467c(int, MATRIX *);
extern void FUN_8006395c(MATRIX *, MATRIX *, MATRIX *);
extern void FUN_80063bcc(SVECTOR *, int *);
extern void FUN_80063ddc(MATRIX *);
extern void FUN_80063e6c(MATRIX *);
extern int FUN_8004fd6c(void *, int, int, int, unsigned);
extern int GetGraphType(void);
extern void SetDrawMode(void *, int, int, int, void *);
extern void AddPrim(void *, void *);

#define gte_ldv3(r0, r1, r2) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 0( %1 );lwc2 $3, 4( %1 );lwc2 $4, 0( %2 );lwc2 $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stsxy3_g4(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")

void func_80118DDC(TObj *o)
{
    int m;
    char *vb4;
    char *vbc;
    char *vc4;
    char *vcc;
    unsigned char *p;
    unsigned char *q;

    FUN_80021f5c(&D_1f800020);
    FUN_8006481c(o->d8c, &D_1f800020);
    FUN_800644dc(o->d84, &D_1f800020);
    FUN_8006467c(o->d88, &D_1f800020);
    D_1f800068.vx = o->a.p.whole;
    D_1f800068.vy = o->y.p.whole;
    D_1f800068.vz = o->b.p.whole;
    FUN_8006395c(&D_1f8000c0, &D_1f800020, &D_1f800000);
    FUN_80063bcc(&D_1f800068, D_1f800000.t);
    D_1f800000.t[0] += D_1f8000c0.t[0];
    D_1f800000.t[1] += D_1f8000c0.t[1];
    D_1f800000.t[2] += D_1f8000c0.t[2];
    FUN_80063ddc(&D_1f800000);
    FUN_80063e6c(&D_1f800000);
    p = (unsigned char *)D_1F800164;
    p[7] = 0x3b;
    {
        unsigned short c = o->w74;
        p[6] = c;
        p[4] = p[5] = (unsigned char)c / 3;
    }
    {
        unsigned short c = o->w76;
        p[0xe] = c;
        p[0xc] = p[0xd] = (unsigned char)c / 3;
    }
    {
        unsigned short c = o->w78;
        p[0x16] = c;
        p[0x14] = p[0x15] = (unsigned char)c / 3;
    }
    {
        unsigned short c = o->w7a;
        p[0x1e] = c;
        p[0x1c] = p[0x1d] = (unsigned char)c / 3;
    }
    vb4 = (char *)o + 0xb4;
    vbc = (char *)o + 0xbc;
    vcc = (char *)o + 0xcc;
    vc4 = (char *)o + 0xc4;
    gte_ldv3(vb4, vbc, vc4);
    gte_rtpt();
    gte_stsxy3_g4(p);
    gte_ldv0(vcc);
    gte_rtps();
    gte_avsz4();
    gte_stotz(&D_1F800070);
    gte_stsxy(p + 0x20);
    FUN_8004fd6c(p, D_1F8001E0 + 0x10, D_1F800070, (signed char)o->b0f, 0x8000000);
    D_1F800164 += 0x24;
    q = (unsigned char *)D_1F800164;
    if (GetGraphType() == 1) m = 0x80;
    else if (GetGraphType() == 2) m = 0x80;
    else m = 0x20;
    SetDrawMode(q, 0, 0, m, 0);
    AddPrim((int *)D_1F8001E0 + (D_1F800070 + 4) + (signed char)o->b0f, q);
    D_1F800164 += 0xc;
}

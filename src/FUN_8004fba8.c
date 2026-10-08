// FUNC 8004fba8 280 MAIN0
// MATCHING 8004fba8 280
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

extern SVECTOR DAT_1f800060;
extern int DAT_1f80008c;
extern MATRIX DAT_1f8000c0;
extern char *DAT_1f8001d4;
extern int DAT_8009c960;
extern void SetRotMatrix(MATRIX *);
extern void SetTransMatrix(MATRIX *);

int FUN_8004fba8(char *p, long *sxy, long *z)
{
    DAT_1f800060.vx = *(short *)(p + 0x12);
    DAT_1f800060.vy = *(short *)(p + 0x16);
    if (DAT_8009c960 == 0x10005 && *(unsigned short *)(DAT_1f8001d4 + 0x4c) != 3)
        DAT_1f800060.vz = *(short *)(p + 0x1a) >> 2;
    else
        DAT_1f800060.vz = *(short *)(p + 0x1a);
    SetRotMatrix(&DAT_1f8000c0);
    SetTransMatrix(&DAT_1f8000c0);
    gte_ldv0(&DAT_1f800060);
    gte_rtps();
    gte_stflg(&DAT_1f80008c);
    if (DAT_1f80008c < 0) return 1;
    gte_stsxy(sxy);
    gte_stszotz(z);
    return 0;
}

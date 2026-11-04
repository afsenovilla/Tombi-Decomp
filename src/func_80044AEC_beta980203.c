// VERSION BETA_980203 beta980203 80044AEC 208
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/projectActorPosition.c (func_80043278_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

extern SVECTOR DAT_1f800060;
extern int MODEL_VERTEX_PTR;
extern MATRIX VIEW_CAMERA;
extern char *TASK_C;
extern int GAME;
extern void SetRotMatrix(MATRIX *);
extern void SetTransMatrix(MATRIX *);

int func_80044AEC_beta980203(char *p, long *sxy, long *z)
{
    DAT_1f800060.vx = *(short *)(p + 0x12);
    DAT_1f800060.vy = *(short *)(p + 0x16);
    if (GAME == 0x10005)
        DAT_1f800060.vz = *(short *)(p + 0x1a) >> 2;
    else
        DAT_1f800060.vz = *(short *)(p + 0x1a);
    SetRotMatrix(&VIEW_CAMERA);
    SetTransMatrix(&VIEW_CAMERA);
    gte_ldv0(&DAT_1f800060);
    gte_rtps();
    gte_stsxy(sxy);
    gte_stszotz(z);
}
#endif

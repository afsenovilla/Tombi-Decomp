// VERSION SLPM_80165 slpm80165 8003F690 164
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_80043278_slps01144 (src/projectActorPosition.c): no area 5 depth scaling; the work
// vector and the camera matrix are in the scratchpad.
#define WORK_VEC_A D_1F800060
#define VIEW_CAMERA D_1F8000C0
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

extern SVECTOR WORK_VEC_A;
extern int MODEL_VERTEX_PTR;
extern MATRIX VIEW_CAMERA;
extern char *TASK_C;
extern void SetRotMatrix(MATRIX *);
extern void SetTransMatrix(MATRIX *);

int func_80043278_slps01144(char *p, long *sxy, long *z)
{
    WORK_VEC_A.vx = *(short *)(p + 0x12);
    WORK_VEC_A.vy = *(short *)(p + 0x16);
    WORK_VEC_A.vz = *(short *)(p + 0x1a);
    SetRotMatrix(&VIEW_CAMERA);
    SetTransMatrix(&VIEW_CAMERA);
    gte_ldv0(&WORK_VEC_A);
    gte_rtps();
    gte_stsxy(sxy);
    gte_stszotz(z);
}
#endif

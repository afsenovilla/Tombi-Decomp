// FUNC 80054c38 796 MAIN0
// MATCHING 80054c38 796
// VERSION SLPS_01144 slps01144 80048004 796
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 80054C38 (drawObjectSpriteQuad.c): 320 -> 321, 256 -> 241.
/* c loaded before an empty loop (its notes split the scheduling block) so the e constant lands after lw c, as in the game (debt). */
#include "TOBJ.H"
#include "camera.h"
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned short uv0, clut;
    unsigned short x1, y1;
    unsigned short uv1, tpage;
    unsigned short x2, y2;
    unsigned short uv2, pad2;
    unsigned short x3, y3;
    unsigned short uv3, pad3;
} PolyFT4;
typedef struct {
    unsigned long w0, w4;
    unsigned short uv2, p0a;
    unsigned short uv3;
} Face;
extern MATRIX D_1F800000;
extern SVECTOR D_1F800060;
extern long LZ_FILE_CTRL;
extern PolyFT4 *NEXT_PRIM;
extern char *CURRENT_OT;
extern void getBaseMatrix(MATRIX *);
extern void SetRotMatrix(MATRIX *);
extern void ApplyRotMatrix(SVECTOR *, long *);
extern void SetTransMatrix(MATRIX *);
extern void SetSemiTrans(PolyFT4 *, int);

#define gte_ldv3(r0, r1, r2) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 0( %1 );lwc2 $3, 4( %1 );lwc2 $4, 0( %2 );lwc2 $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stsxy3_ft4(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")

static __inline__ int onscreen(PolyFT4 *p)
{
    if (p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1 || p->y3 < 0xf1) {
        if (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141 || p->x3 < 0x141)
            return 1;
    }
    return 0;
}

static __inline__ int otadd(unsigned long *a, char *b, int c, int d, unsigned long e)
{
    d = ((signed char)d + c) << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - (int)CURRENT_OT) >= 0xca0) return 1;
    {
        unsigned long v = *(unsigned long *)d;
        *(unsigned long *)d = (unsigned long)a;
        *a = v | e;
    }
    return 0;
}

void drawObjectSpriteQuad(TObj *o)
{
    PolyFT4 *p;
    Face *f;
    char *v0, *v1, *v2, *v3;

    getBaseMatrix(&D_1F800000);
    D_1F800060.vx = o->a.p.whole;
    D_1F800060.vy = o->y.p.whole;
    D_1F800060.vz = o->b.p.whole;
    SetRotMatrix(&VIEW_CAMERA.m);
    ApplyRotMatrix(&D_1F800060, D_1F800000.t);
    D_1F800000.t[0] += VIEW_CAMERA.m.t[0];
    D_1F800000.t[1] += VIEW_CAMERA.m.t[1];
    D_1F800000.t[2] += VIEW_CAMERA.m.t[2];
    SetTransMatrix(&D_1F800000);
    v0 = (char *)o + 0xb4;
    v1 = (char *)o + 0xbc;
    v2 = (char *)o + 0xc4;
    v3 = (char *)o + 0xcc;
    p = NEXT_PRIM;
    gte_ldv3(v0, v1, v2);
    gte_rtpt();
    gte_stsxy3_ft4(p);
    gte_ldv0(v3);
    gte_rtps();
    gte_avsz4();
    gte_stotz(&LZ_FILE_CTRL);
    gte_stsxy(&p->x3);
    if (onscreen(p)) {
        f = (Face *)(*(volatile int *)&o->d3c + ((short *)(o->d3c + (*(unsigned short *)o->anim << 2)))[1]);
        p->code = 0x2d;
        SetSemiTrans(p, o->b0d >> 7);
        *(unsigned long *)&p->uv0 = f->w0;
        *(unsigned long *)&p->uv1 = f->w4;
        p->uv2 = f->uv2;
        p->uv3 = f->uv3;
        p->tpage += o->w1e;
        if (o->b0d & 1)
            p->clut = o->w08;
        {
            int c = LZ_FILE_CTRL;
            unsigned long e;
            do { } while (0); /* loop notes end the sched block after the c load (debt) */
            e = 0x9000000;
            if (!otadd((unsigned long *)p, CURRENT_OT + 0x10, c, o->b0f, e))
                NEXT_PRIM = (PolyFT4 *)((char *)NEXT_PRIM + 0x28);
        }
    }
}
#else
/* c loaded before an empty loop (its notes split the scheduling block) so the e constant lands after lw c, as in the game (debt). */
#include "TOBJ.H"
#include "camera.h"
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned short uv0, clut;
    unsigned short x1, y1;
    unsigned short uv1, tpage;
    unsigned short x2, y2;
    unsigned short uv2, pad2;
    unsigned short x3, y3;
    unsigned short uv3, pad3;
} PolyFT4;
typedef struct {
    unsigned long w0, w4;
    unsigned short uv2, p0a;
    unsigned short uv3;
} Face;
extern MATRIX D_1F800000;
extern SVECTOR D_1F800060;
extern long LZ_FILE_CTRL;
extern PolyFT4 *NEXT_PRIM;
extern char *CURRENT_OT;
extern void getBaseMatrix(MATRIX *);
extern void SetRotMatrix(MATRIX *);
extern void ApplyRotMatrix(SVECTOR *, long *);
extern void SetTransMatrix(MATRIX *);
extern void SetSemiTrans(PolyFT4 *, int);

#define gte_ldv3(r0, r1, r2) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 0( %1 );lwc2 $3, 4( %1 );lwc2 $4, 0( %2 );lwc2 $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stsxy3_ft4(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")

static __inline__ int onscreen(PolyFT4 *p)
{
    if (p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100 || p->y3 < 0x100) {
        if (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140 || p->x3 < 0x140)
            return 1;
    }
    return 0;
}

static __inline__ int otadd(unsigned long *a, char *b, int c, int d, unsigned long e)
{
    d = ((signed char)d + c) << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - (int)CURRENT_OT) >= 0xca0) return 1;
    {
        unsigned long v = *(unsigned long *)d;
        *(unsigned long *)d = (unsigned long)a;
        *a = v | e;
    }
    return 0;
}

void drawObjectSpriteQuad(TObj *o)
{
    PolyFT4 *p;
    Face *f;
    char *v0, *v1, *v2, *v3;

    getBaseMatrix(&D_1F800000);
    D_1F800060.vx = o->a.p.whole;
    D_1F800060.vy = o->y.p.whole;
    D_1F800060.vz = o->b.p.whole;
    SetRotMatrix(&VIEW_CAMERA.m);
    ApplyRotMatrix(&D_1F800060, D_1F800000.t);
    D_1F800000.t[0] += VIEW_CAMERA.m.t[0];
    D_1F800000.t[1] += VIEW_CAMERA.m.t[1];
    D_1F800000.t[2] += VIEW_CAMERA.m.t[2];
    SetTransMatrix(&D_1F800000);
    v0 = (char *)o + 0xb4;
    v1 = (char *)o + 0xbc;
    v2 = (char *)o + 0xc4;
    v3 = (char *)o + 0xcc;
    p = NEXT_PRIM;
    gte_ldv3(v0, v1, v2);
    gte_rtpt();
    gte_stsxy3_ft4(p);
    gte_ldv0(v3);
    gte_rtps();
    gte_avsz4();
    gte_stotz(&LZ_FILE_CTRL);
    gte_stsxy(&p->x3);
    if (onscreen(p)) {
        f = (Face *)(*(volatile int *)&o->d3c + ((short *)(o->d3c + (*(unsigned short *)o->anim << 2)))[1]);
        p->code = 0x2d;
        SetSemiTrans(p, o->b0d >> 7);
        *(unsigned long *)&p->uv0 = f->w0;
        *(unsigned long *)&p->uv1 = f->w4;
        p->uv2 = f->uv2;
        p->uv3 = f->uv3;
        p->tpage += o->w1e;
        if (o->b0d & 1)
            p->clut = o->w08;
        {
            int c = LZ_FILE_CTRL;
            unsigned long e;
            do { } while (0); /* loop notes end the sched block after the c load (debt) */
            e = 0x9000000;
            if (!otadd((unsigned long *)p, CURRENT_OT + 0x10, c, o->b0f, e))
                NEXT_PRIM = (PolyFT4 *)((char *)NEXT_PRIM + 0x28);
        }
    }
}
#endif

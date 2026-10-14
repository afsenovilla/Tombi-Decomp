// FUNC 80054868 976 MAIN0
// MATCHING 80054868 976
// FLAGS -O2 -G0 -fno-strength-reduce -fno-expensive-optimizations
#include "spad.h"
/* e built in two steps (base, then += index) keeps the game load order; the matrix address is reloaded for each call (-fno-expensive-optimizations). */
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short w, h;
} SP;
typedef struct { char pad[0x4c]; unsigned short w4c; } C4C;
extern char *DAT_1f800164;
extern char *DAT_1f8001e0;
extern C4C *TASK_C;
extern long LZ_FILE_CTRL;
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetDrawMode(void *, int, int, int, void *);
extern void AddPrim(void *, void *);
extern void SetSprt(void *);
extern void SetSemiTrans(void *, int);
extern void SetShadeTex(void *, int);

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

static __inline__ int proj(void)
{
    gte_ldv0(((short*)&SPAD.s60));
    gte_rtps();
    gte_stflg(&SPAD.i8c);
    if (SPAD.i8c < 0) return 1;
    gte_stsxy(&LZ_FILE_CTRL);
    gte_stszotz(&SPAD.i74);
    return 0;
}

void func_80054868(TObj *o)
{
    SP *s;
    short *e;
    char *q;
    unsigned char *p;
    int n;
    int tp;

    ((short*)&SPAD.s60)[0] = o->a.p.whole;
    SPAD.s62 = o->y.p.whole;
    if ((*(int*)&GAME.selectedArea) == 0x10005 && TASK_C->w4c != 3)
        SPAD.s64 = o->b.p.whole >> 2;
    else
        SPAD.s64 = o->b.p.whole;
    SetRotMatrix(((char*)&SPAD.bc0));
    SetTransMatrix(((char*)&SPAD.bc0));
    if (proj()) return;
    e = (short *)o->d3c;
    e += *(unsigned short *)o->anim * 2;
    s = (SP *)DAT_1f800164;
    n = e[0];
    q = (char *)(o->d3c + e[1]);
    p = (unsigned char *)q + 0xf;
    tp = *(unsigned short *)(q + 6);
    SetDrawMode(s, 0, 0, 0, 0);
    AddPrim(DAT_1f8001e0 + SPAD.i74 * 4 + ((signed char)o->b0f * 4 + 0x10), s);
    DAT_1f800164 += 0xc;
    do {
        s = (SP *)DAT_1f800164;
        s->w = p[-5];
        s->h = p[-4];
        s->x0 = (signed char)p[-1] + LZ_FILE_CTRL;
        s->y0 = (signed char)p[0] + (LZ_FILE_CTRL >> 16);
        if (((unsigned short)s->y0 < 0x101 || (unsigned short)(s->y0 + s->h) < 0x101)
            && ((unsigned short)s->x0 < 0x141 || (unsigned short)(s->x0 + s->w) < 0x141)) {
            SetSprt(s);
            SetSemiTrans(s, o->b0d >> 7);
            s->code |= 1;
            *(int *)&s->u0 = *(int *)q;
            if (o->b0d & 1)
                s->clut = o->w08;
            SetShadeTex(s, 0);
            s->r0 = o->b6b;
            s->g0 = o->b6b;
            s->b0 = o->b6b;
            AddPrim(DAT_1f8001e0 + SPAD.i74 * 4 + ((signed char)o->b0f * 4 + 0x10), s);
            DAT_1f800164 += 0x14;
        }
        p += 0x10;
        n--;
        q += 0x10;
    } while (n != 0);
    s = (SP *)DAT_1f800164;
    SetDrawMode(s, 0, 0, o->w1e + (short)tp, 0);
    AddPrim(DAT_1f8001e0 + SPAD.i74 * 4 + ((signed char)o->b0f * 4 + 0x10), s);
    DAT_1f800164 += 0xc;
}

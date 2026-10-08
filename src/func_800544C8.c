// FUNC 800544c8 928 MAIN0
// MATCHING 800544c8 928
// FLAGS -O2 -G0 -fno-strength-reduce
/* volatile first o->d3c read and volatile D_164V load pin the game load order (debt) */
#include "TOBJ.H"
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
extern char *volatile D_164V;
extern char *DAT_1f8001e0;
extern int D_8009C960;
extern C4C *D_1F8001D4;
extern short D_1F800060[];
extern short D_1F800062, D_1F800064;
extern long D_1F80008C;
extern long D_1F800070;
extern long D_1F800074;
extern char D_1F8000C0[], D_1F8000C0b[];
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetDrawMode(void *, int, int, int, void *);
extern void AddPrim(void *, void *);
extern void SetSprt(void *);
extern void SetSemiTrans(void *, int);

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

static __inline__ int proj(void)
{
    gte_ldv0(D_1F800060);
    gte_rtps();
    gte_stflg(&D_1F80008C);
    if (D_1F80008C < 0) return 1;
    gte_stsxy(&D_1F800070);
    gte_stszotz(&D_1F800074);
    return 0;
}

void func_800544C8(TObj *o)
{
    SP *s;
    short *e;
    char *q;
    unsigned char *p;
    int n;
    int tp;

    D_1F800060[0] = o->a.p.whole;
    D_1F800062 = o->y.p.whole;
    if (D_8009C960 == 0x10005 && D_1F8001D4->w4c != 3)
        D_1F800064 = o->b.p.whole >> 2;
    else
        D_1F800064 = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (proj()) return;
    e = (short *)(*(volatile int *)&o->d3c + *(unsigned short *)o->anim * 4);
    s = (SP *)D_164V;
    n = e[0];
    q = (char *)(o->d3c + e[1]);
    p = (unsigned char *)q + 0xf;
    tp = *(unsigned short *)(q + 6);
    SetDrawMode(s, 0, 0, 0, 0);
    AddPrim(DAT_1f8001e0 + D_1F800074 * 4 + ((signed char)o->b0f * 4 + 0x10), s);
    DAT_1f800164 += 0xc;
    do {
        s = (SP *)DAT_1f800164;
        s->w = p[-5];
        s->h = p[-4];
        s->x0 = (signed char)p[-1] + D_1F800070;
        s->y0 = (signed char)p[0] + (D_1F800070 >> 16);
        if (((unsigned short)s->y0 < 0x101 || (unsigned short)(s->y0 + s->h) < 0x101)
            && ((unsigned short)s->x0 < 0x141 || (unsigned short)(s->x0 + s->w) < 0x141)) {
            SetSprt(s);
            SetSemiTrans(s, o->b0d >> 7);
            s->code |= 1;
            *(int *)&s->u0 = *(int *)q;
            if (o->b0d & 1)
                s->clut = o->w08;
            AddPrim(DAT_1f8001e0 + D_1F800074 * 4 + ((signed char)o->b0f * 4 + 0x10), s);
            DAT_1f800164 += 0x14;
        }
        p += 0x10;
        n--;
        q += 0x10;
    } while (n != 0);
    s = (SP *)DAT_1f800164;
    SetDrawMode(s, 0, 0, o->w1e + (short)tp, 0);
    AddPrim(DAT_1f8001e0 + D_1F800074 * 4 + ((signed char)o->b0f * 4 + 0x10), s);
    DAT_1f800164 += 0xc;
}

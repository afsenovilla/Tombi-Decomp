// FUNC 80052db8 1348 MAIN0
// MATCHING 80052db8 1348
// FLAGS -O2 -G0 -fno-strength-reduce
#include "TOBJ.H"
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
typedef struct { char pad[0x4c]; unsigned short w4c; } C4C;
extern PolyFT4 *DAT_1f800164;
extern char *DAT_1f8001e0;
extern int D_8009C960;
extern C4C *D_1F8001D4;
extern short D_1F800060[];
extern short D_1F800062, D_1F800064;
extern long D_1F80008C;
extern long D_1F800070;
extern long D_1F800074;
extern unsigned short D_1F8001C8;
extern short D_1F8000F6, D_1F8000EE;
extern int D_1F8002B4;
extern char D_1F8000C0[], D_1F8000C0b[];
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
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
    if ((unsigned)(d - (int)DAT_1f8001e0) >= 0xca0) return 1;
    {
        unsigned long v = *(unsigned long *)d;
        *(unsigned long *)d = (unsigned long)a;
        *a = v | e;
    }
    return 0;
}

void FUN_80052db8(TObj *o)
{
    PolyFT4 *p;
    short *e;
    char *q;
    unsigned char *f;
    int n;
    unsigned int k;
    int d;

    if (o->anim == 0) return;
    D_1F800060[0] = o->a.p.whole;
    D_1F800062 = o->y.p.whole;
    if (D_8009C960 == 0x10005 && D_1F8001D4->w4c != 3)
        D_1F800064 = o->b.p.whole >> 2;
    else
        D_1F800064 = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (proj()) return;
    e = (short *)(o->d3c + *(short *)o->anim * 4);
    n = e[0];
    q = (char *)(*(volatile int *)&o->d3c + e[1]);
    if (o->b0a == 1) {
        switch (D_1F8001C8 & 1) {
        case 0:
            d = D_1F8000F6 - o->b.p.whole;
            k = o->d64 * (d * 7 + 0x1000) >> 12;
            break;
        case 1:
            d = o->a.p.whole - D_1F8000EE;
            k = o->d64 * (d * 7 + 0x1000) >> 12;
            break;
        }
    } else {
        k = D_1F8002B4;
    }
    f = (unsigned char *)q + 0xc;
    do {
        p = DAT_1f800164;
        if (o->animFrame & 1) {
            p->x0 = D_1F800070 + (((signed char)f[2] * k - 0x1000) >> 12);
            p->y0 = (D_1F800070 >> 16) + (((signed char)f[3] * k - 0x1000) >> 12);
            p->x1 = p->x0 + ((f[-2] * k + 0x1000) >> 12);
            p->y1 = p->y0;
            p->x2 = p->x0;
            p->y2 = p->y0 + ((f[-1] * k + 0x1000) >> 12);
            p->x3 = p->x1;
            p->y3 = p->y2;
        } else {
            p->x0 = D_1F800070 - (((signed char)f[2] * k - 0x1000) >> 12);
            p->y0 = (D_1F800070 >> 16) + (((signed char)f[3] * k - 0x1000) >> 12);
            p->x1 = p->x0 - ((f[-2] * k + 0x1000) >> 12);
            p->y1 = p->y0;
            p->x2 = p->x0;
            p->y2 = p->y0 + ((f[-1] * k + 0x1000) >> 12);
            p->x3 = p->x1;
            p->y3 = p->y2;
        }
        if (onscreen(p)) {
            p->code = 0x2d;
            SetSemiTrans(p, o->b0d >> 7);
            *(unsigned long *)&p->uv0 = *(unsigned long *)q;
            *(unsigned long *)&p->uv1 = *(unsigned long *)(f - 8);
            p->uv2 = *(unsigned short *)(f - 4);
            p->uv3 = *(unsigned short *)f;
            p->tpage += o->w1e;
            if (o->b0d & 1)
                p->clut = o->w08;
            if (o->b0b) {
                if (otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, 0, o->b0f, 0x9000000)) goto next;
            } else {
                if (otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800074, o->b0f, 0x9000000)) goto next;
            }
            DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
        }
    next:
        f += 0x10;
        n--;
        q += 0x10;
    } while (n != 0);
}

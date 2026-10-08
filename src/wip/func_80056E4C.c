// FUNC 80056e4c 860 MAIN0
// wip score 40 (was 59): xy read volatile fixes the load order/size; left: la a0 for the volatile xy (game lui s1 direct), regs in uv block and v0/v1 at +0x2dc.
#include "TOBJ.H"
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    unsigned short x2, y2;
    unsigned char u2, v2;
    unsigned short pad2;
    unsigned short x3, y3;
    unsigned char u3, v3;
    unsigned short pad3;
} PolyFT4;

typedef struct { short vx, vy, vz, pad; } SVec;

extern int D_8009C960;
extern SVec D_1F800060;
extern long D_1F80008C;
extern long D_1F800070;
extern long D_1F800074;
extern char D_1F8000C0[];
extern char D_1F8000C0b[];
extern TObj *DAT_1f8001d4;
extern PolyFT4 *DAT_1f800164;
extern int DAT_1f8001e0;
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetSemiTrans(PolyFT4 *, int);

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

static __inline__ int project(void)
{
    gte_ldv0(&D_1F800060);
    gte_rtps();
    gte_stflg(&D_1F80008C);
    if (D_1F80008C < 0)
        return 1;
    gte_stsxy(&D_1F800070);
    gte_stszotz(&D_1F800074);
    return 0;
}

static __inline__ int addprim(unsigned *a, char *b, int c, int d, unsigned e)
{
    d = d << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - DAT_1f8001e0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)d;
        *(unsigned *)d = (unsigned)a;
        *a = v | e;
    }
    return 0;
}

void func_80056E4C(TObj *o)
{
    PolyFT4 *p;
    unsigned char *s;
    int xy, t, z;
    short *hh;
    unsigned char w, h, u, v;

    D_1F800060.vx = o->a.p.whole;
    D_1F800060.vy = o->y.p.whole;
    if (D_8009C960 == 0x10005 && *(unsigned short *)&DAT_1f8001d4->w4c != 3)
        D_1F800060.vz = o->b.p.whole >> 2;
    else
        D_1F800060.vz = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (project())
        return;
    hh = (short *)(*(volatile int *)&o->d3c + *(unsigned short *)o->anim * 4);
    xy = *(volatile long *)&D_1F800070;
    s = (unsigned char *)*(volatile int *)&o->d3c;
    p = DAT_1f800164;
    s += hh[1];
    p->code = 0x2c;
    p->r0 = o->wb4;
    p->g0 = o->wb4;
    p->b0 = o->wb4;
    SetSemiTrans(p, 1);
    p->tpage = o->w1e;
    u = s[0];
    v = s[1];
    w = s[0xa];
    h = s[0xb];
    p->u0 = u;
    p->v0 = v;
    p->v1 = v;
    p->u1 = w + u;
    p->u2 = u;
    p->v2 = v + h;
    p->u3 = w + u;
    p->v3 = v + h;
    p->x0 = xy + ((signed char)s[0xe] * (unsigned short)o->wb8) / 200;
    p->y0 = (xy >> 16) + ((signed char)s[0xf] * (unsigned short)o->wb8) / 200;
    t = (w * (unsigned short)o->wb8) / 100;
    p->y1 = *(volatile unsigned short *)&p->y0;
    p->x2 = p->x0;
    p->x1 = p->x0 + t - 1;
    p->y2 = p->y0 + (h * (unsigned short)o->wb8) / 100 - 1;
    p->x3 = p->x1;
    p->y3 = p->y2;
    z = D_1F800074;
    p->clut = o->w08;
    if (addprim((unsigned *)p, (char *)(DAT_1f8001e0 + 0x10), 0, z + (signed char)o->b0f, 0x9000000) == 0)
        DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
}

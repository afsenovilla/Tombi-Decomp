// FUNC 80055cb8 756 MAIN0
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
    unsigned long w0;
    unsigned long w4;
    unsigned short h8;
    unsigned char w, h;
    unsigned short hc;
    signed char dx, dy;
} Ent;

typedef struct {
    char p0[8];
    unsigned short w08;
    char p0a[3];
    unsigned char b0d;
    char p0e;
    signed char b0f;
    char p10[2];
    unsigned short x;
    char p14[2];
    unsigned short y;
    char p18[2];
    short z;
    char p1c[2];
    unsigned short w1e;
    char p20[4];
    unsigned short *anim;
    char p28[0x2e - 0x28];
    unsigned short animFrame;
    char p30[0x3c - 0x30];
    char *tab;
} O;

extern PolyFT4 *DAT_1f800164;
extern char *DAT_1f8001e0;
extern void SetSemiTrans(PolyFT4 *, int);

static __inline__ int onscreen(PolyFT4 *p)
{
    if ((*(volatile unsigned short *)&p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100 || p->y3 < 0x100) &&
        (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140 || p->x3 < 0x140))
        return 1;
    return 0;
}

static __inline__ int addprim(unsigned *a, char *b, int c, int d, unsigned e)
{
    d = ((signed char)d + c) << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - (int)DAT_1f8001e0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)d;
        *(unsigned *)d = (unsigned)a;
        *a = v | e;
    }
    return 0;
}

/* score 21: volatile x3/y3 stores + volatile y0 read fix the onscreen block; addprim(p, base, z, b0f, e) with d = ((signed char)d + c) << 2 (as FUN_80052db8) fixed the sum. Left: lui 0x9000000 (e) lands in the beqz delay slot of the b0d&1 if (game: after lh z). Tried: e inside the inline (45), all 120 param orders (same), e|v / v|=e, goto-next form of FUN_80052db8, short/schar param types, z read into a local, open-coded OT insert (47). The li is the first insn of the join block in ours (reorg steals it into the beqz slot); game schedules lh z first. b35: rewrite in the style of matched twin FUN_80052db8 (TObj, q/f pointers, goto next, -fno-strength-reduce) gives the same 21 once the volatile onscreen/x3/y3 are kept (50 without); otadd variants (b inside, e inside, int b) x all param orders: 21/45/53; e |= v makes gcc hoist the li into an s-reg (31). b46: -dS dump: it is sched2 that puts `li a1` (inline param e, reg/v a1) first in the join block (before lh z), then reorg steals it from the target; game's nop suggests lh first and the li not stealable after a trapping load. */
void func_80055CB8(O *o)
{
    PolyFT4 *p;
    Ent *e;
    short *h;
    int n;

    h = (short *)(o->tab + *o->anim * 4);
    n = h[0];
    e = (Ent *)(*(char * volatile *)&o->tab + h[1]);
    do {
        p = DAT_1f800164;
        if (o->animFrame & 1) {
            p->x0 = o->x + e->dx;
            p->y0 = o->y + e->dy;
            p->x1 = p->x0 + e->w;
            p->x2 = p->x0;
            p->y1 = p->y0;
        } else {
            p->x0 = o->x - e->dx;
            p->y0 = o->y + e->dy;
            p->x1 = p->x0 - e->w;
            p->x2 = p->x0;
            p->y1 = p->y0;
        }
        p->y2 = p->y0 + e->h;
        *(volatile unsigned short *)&p->x3 = p->x1;
        *(volatile unsigned short *)&p->y3 = p->y2;
        if (onscreen(p)) {
            p->code = 0x2d;
            SetSemiTrans(p, o->b0d >> 7);
            *(unsigned long *)&p->uv0 = e->w0;
            *(unsigned long *)&p->uv1 = e->w4;
            p->uv2 = e->h8;
            p->uv3 = e->hc;
            p->tpage += o->w1e;
            if (o->b0d & 1) {
                p->clut = o->w08;
            }
            if (addprim((unsigned *)p, DAT_1f8001e0 + 0x10, o->z, o->b0f, 0x9000000) == 0) {
                DAT_1f800164 = DAT_1f800164 + 1;
            }
        }
        e++;
    } while (--n != 0);
}

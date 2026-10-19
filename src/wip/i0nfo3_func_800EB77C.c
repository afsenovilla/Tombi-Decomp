// FUNC 800eb77c 1248 I0NFO3
// FLAGS -O2 -G0 -fno-strength-reduce
#include "TOBJ.H"
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    unsigned short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    unsigned short x2, y2;
    unsigned char u2, v2; unsigned short pad2;
    unsigned short x3, y3;
    unsigned char u3, v3; unsigned short pad3;
} PolyFT4;
typedef struct { short vx, vy, vz, pad; } SVec;
extern long D_1F8000C0[];
extern char *D_1F800164;
typedef struct { char pad[0x10]; unsigned long ot[1]; } OTS;
typedef struct { unsigned long v; } UL;
extern OTS *D_1F8001E0;

#define gte_SetRotMatrix(r0) __asm__ volatile ("lw $12, 0( %0 );lw $13, 4( %0 );ctc2 $12, $0;ctc2 $13, $1;lw $12, 8( %0 );lw $13, 12( %0 );lw $14, 16( %0 );ctc2 $12, $2;ctc2 $13, $3;ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile ("lw $12, 20( %0 );lw $13, 24( %0 );ctc2 $12, $5;lw $14, 28( %0 );ctc2 $13, $6;ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

void func_800EB77C(TObj *o)
{
    SVec *v = (SVec *)0x1F800048;
    PolyFT4 *p = (PolyFT4 *)0x1F800050;
    long *cnt = (long *)0x1F800078;
    long *sxy = (long *)0x1F80007C;
    UL *tmp = (UL *)0x1F800080;
    long *otz = (long *)0x1F800084;
    unsigned long *code = (unsigned long *)0x1F800088;
    unsigned short *frm = (unsigned short *)0x1F80008C;
    unsigned char *e;
    unsigned char *f;
    short *h;
    unsigned char c;
    unsigned short cl;
    PolyFT4 *q;
    unsigned long *slot;
    int k;

    if (o->b6b == 0) return;
    gte_SetRotMatrix(D_1F8000C0);
    gte_SetTransMatrix(D_1F8000C0);
    v->vx = o->a.p.whole;
    v->vy = o->y.p.whole;
    v->vz = o->b.p.whole;
    gte_ldv0(v);
    gte_rtps();
    gte_stsxy(sxy);
    gte_stszotz(otz);
    *frm = *(unsigned short *)o->anim;
    h = (short *)(o->d3c + *frm * 4);
    e = (unsigned char *)(*(volatile int *)&o->d3c + h[1]);
    f = e + 6;
    *cnt = h[0];
    *code = 0x09000000;
    c = o->b6b;
    do {
        p->code = 0x2C;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        *(unsigned long *)&p->u0 = *(unsigned long *)e;
        *(unsigned long *)&p->u1 = *(unsigned long *)(f - 2);
        *(unsigned short *)&p->u2 = *(unsigned short *)(f + 2);
        *(unsigned short *)&p->u3 = *(unsigned short *)(f + 6);
        switch (o->category & 0x7F) {
        case 2:
        case 9:
            p->code |= 2;
            if (o->animFrame == 2) {
                p->x0 = (signed char)f[8] + *sxy;
                p->y0 = (signed char)f[9] + (*sxy >> 16);
                p->x1 = p->x0 + f[4];
                p->x2 = p->x0;
                p->y1 = p->y0;
            } else {
                p->x0 = *sxy - (signed char)f[8];
                p->y0 = (signed char)f[9] + (*sxy >> 16);
                p->x1 = p->x0 - f[4];
                p->x2 = p->x0;
                p->y1 = p->y0;
            }
            p->y2 = p->y0 + f[5];
            p->x3 = p->x1;
            p->y3 = p->y2;
            p->tpage = o->w1e + *(unsigned short *)f;
            if (o->b0d != 0) p->clut = o->w08;
            break;
        case 3:
            p->x0 = (signed char)f[8] + *sxy;
            p->y0 = (signed char)f[9] + (*sxy >> 16);
            p->x1 = p->x0 + f[4];
            p->x2 = p->x0;
            p->y1 = p->y0;
            p->y2 = p->y0 + f[5];
            p->x3 = p->x1;
            p->y3 = p->y2;
            p->tpage = o->w1e + *(unsigned short *)f;
            p->clut = o->w08;
            break;
        }
        if ((p->y0 < 0xF1 || p->y1 < 0xF1 || p->y2 < 0xF1 || p->y3 < 0xF1)
            && (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141 || p->x3 < 0x141)) {
            q = (PolyFT4 *)D_1F800164;
            *q = *p;
            slot = (unsigned long *)((*otz << 2) - 0x10);
            k = (signed char)o->b0f;
            do { } while (0);
            slot = (unsigned long *)((int)slot + (int)&D_1F8001E0->ot[k]);
            tmp->v = *slot;
            *slot = (unsigned long)q;
            tmp->v |= *code;
            q->tag = tmp->v;
            D_1F800164 += 0x28;
        }
        f += 0x10;
        e += 0x10;
    } while (--*cnt);
}

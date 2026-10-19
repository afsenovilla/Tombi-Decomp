// FUNC 800ea124 2612 I0NFO
// MATCHING 800ea124 2612
#include "TOBJ.H"
#include "raw7.h"
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
typedef struct { unsigned char p0[0x18]; short w18, w1a; unsigned char p1c[0x28 - 0x1c]; unsigned short w28[1]; } P9C330;
extern long D_1F8000C0[];
extern char *D_1F800164;
extern int D_1F8001E0;
extern P9C330 *D_8009C330;
extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern void func_80037C80(TObj *, unsigned char *, int);
extern void func_8004FD6C(void *, int, long, int, unsigned long);

#define gte_SetRotMatrix(r0) __asm__ volatile ("lw $12, 0( %0 );lw $13, 4( %0 );ctc2 $12, $0;ctc2 $13, $1;lw $12, 8( %0 );lw $13, 12( %0 );lw $14, 16( %0 );ctc2 $12, $2;ctc2 $13, $3;ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile ("lw $12, 20( %0 );lw $13, 24( %0 );ctc2 $12, $5;lw $14, 28( %0 );ctc2 $13, $6;ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

#define SN(i) sn[i]
#define CS(i) cs[i]
#define C e[0xA]
#define D e[0xB]

void func_800EA124(TObj *o)
{
    PolyFT4 *P = (PolyFT4 *)0x1F800000;
    SVec *v = (SVec *)0x1F800028;
    long *sxy = (long *)0x1F800030;
    short *u = (short *)0x1F80003C;
    short *w = (short *)0x1F80003E;
    short *s40 = (short *)0x1F800040;
    unsigned short *x = (unsigned short *)0x1F800042;
    unsigned short *y = (unsigned short *)0x1F800044;
    unsigned short *frm = (unsigned short *)0x1F800046;
    unsigned short *a0 = (unsigned short *)0x1F800048;
    unsigned short *a1 = (unsigned short *)0x1F80004A;
    unsigned short *n = (unsigned short *)0x1F80004C;
    long otz;
    short *h;
    unsigned short n0;
    unsigned char *e;
    register unsigned char *pe asm("a1");
    PolyFT4 *q;
    unsigned short w0;
    int d, d2, ki;
    short *sn;
    short *cs;

    gte_SetRotMatrix(D_1F8000C0);
    gte_SetTransMatrix(D_1F8000C0);
    v->vx = o->a.p.whole;
    v->vy = o->y.p.whole;
    v->vz = o->b.p.whole;
    gte_ldv0(v);
    gte_rtps();
    gte_stsxy(sxy);
    gte_stszotz(&otz);
    *x = *sxy;
    *y = *sxy >> 16;
    *frm = *(unsigned short *)o->anim;
    h = (short *)(o->d3c + *frm * 4);
    n0 = h[0];
    ki = h[1];
    d = *(volatile int *)&o->d3c;
    *n = n0;
    w0 = D_8009C330->w28[*(short *)0x1F8001F4];
    /* identity; the fake dependence on w0 keeps the pointer add after the table compare */
    __asm__("" : "=r"(d2) : "0"(d), "r"(w0));
    pe = (unsigned char *)(d2 + ki);
    e = pe;
    if (w0 != *frm) {
        func_80037C80(o, pe, ki);
    }
    sn = D_8007A5F0;
    cs = D_8007A3F0;
    *a0 = (unsigned char)o->d8c;
    *a1 = (o->d8c + 0x80) & 0xFF;
    do {
        *u = (signed char)e[0xE];
        *w = (signed char)e[0xF];
        P->code = 0x2D;
        P->u0 = e[0] + (*(unsigned char *)0x1F8001F4 << 6) + 4;
        P->v0 = e[1] + 4;
        P->u1 = P->u0 + C;
        P->u2 = P->u0;
        P->v1 = P->v0;
        P->v2 = P->v0 + D;
        P->u3 = P->u0 + C;
        P->v3 = P->v0 + D;
        *s40 = 0;
        D_8009C330->w18 = *x;
        D_8009C330->w1a = *y;
        P->tpage = o->w1e;
        if (*n & 2)
            P->clut = o->w08;
        else
            P->clut = U16(o, 0xC4);
        if ((unsigned)((o->animFrame & 7) - 3) < 3) {
            *u = -(*u + C);
            P->x1 = *x + ((-*u * SN(*a1)) >> 12) + ((-*w * SN((*a1 + 0xc0) & 0xff)) >> 12);
            P->y1 = *y + ((-*u * CS(*a1)) >> 12) + ((-*w * CS((*a1 + 0xc0) & 0xff)) >> 12);
            P->x0 = *x + (((*u + C) * SN(*a0)) >> 12) + ((-*w * SN((*a0 + 0x40) & 0xff)) >> 12);
            P->y0 = *y + (((*u + C) * CS(*a0)) >> 12) + ((-*w * CS((*a0 + 0x40) & 0xff)) >> 12);
            P->x3 = *x + ((-*u * SN(*a1)) >> 12) + (((*w + D) * SN((*a1 + 0x40) & 0xff)) >> 12);
            P->y3 = *y + ((-*u * CS(*a1)) >> 12) + (((*w + D) * CS((*a1 + 0x40) & 0xff)) >> 12);
            P->x2 = *x + (((*u + C) * SN(*a0)) >> 12) + (((*w + D) * SN((*a0 + 0xc0) & 0xff)) >> 12);
            P->y2 = *y + (((*u + C) * CS(*a0)) >> 12) + (((*w + D) * CS((*a0 + 0xc0) & 0xff)) >> 12);
        } else {
            P->x0 = *x + ((-*u * SN(*a1)) >> 12) + ((-*w * SN((*a1 + 0xc0) & 0xff)) >> 12);
            P->y0 = *y + ((-*u * CS(*a1)) >> 12) + ((-*w * CS((*a1 + 0xc0) & 0xff)) >> 12);
            P->x1 = *x + (((*u + C) * SN(*a0)) >> 12) + ((-*w * SN((*a0 + 0x40) & 0xff)) >> 12);
            P->y1 = *y + (((*u + C) * CS(*a0)) >> 12) + ((-*w * CS((*a0 + 0x40) & 0xff)) >> 12);
            P->x2 = *x + ((-*u * SN(*a1)) >> 12) + (((*w + D) * SN((*a1 + 0x40) & 0xff)) >> 12);
            P->y2 = *y + ((-*u * CS(*a1)) >> 12) + (((*w + D) * CS((*a1 + 0x40) & 0xff)) >> 12);
            P->x3 = *x + (((*u + C) * SN(*a0)) >> 12) + (((*w + D) * SN((*a0 + 0xc0) & 0xff)) >> 12);
            P->y3 = *y + (((*u + C) * CS(*a0)) >> 12) + (((*w + D) * CS((*a0 + 0xc0) & 0xff)) >> 12);
        }
        q = (PolyFT4 *)D_1F800164;
        *q = *P;
        func_8004FD6C(q, D_1F8001E0 + 0x10, otz, (signed char)o->b0f, 0x09000000);
        e += 0x10;
        D_1F800164 += 0x28;
    } while (--*n);
}

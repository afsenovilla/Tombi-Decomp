// FUNC 8011e778 696 X004
// MATCHING 8011e778 696
// FLAGS -O2 -G0 -fno-strength-reduce
#include "TOBJ.H"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u_long;
typedef struct { unsigned addr : 24; unsigned len : 8; u8 r0, g0, b0, code; } P_TAG;
typedef struct { u_long tag; u8 r0, g0, b0, code; short x0, y0; u8 u0, v0; u16 clut; u16 w, h; } SPRT;
typedef struct { u_long tag; u_long code[2]; } DR_MODE;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u_long)(_addr))
#define getaddr(p) (u_long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define setlen(p, _len) (((P_TAG *)(p))->len = (u8)(_len))
#define setcode(p, _code) (((P_TAG *)(p))->code = (u8)(_code))
#define getcode(p) (u8)(((P_TAG *)(p))->code)
#define setSemiTrans(p, abe) ((abe) ? setcode(p, getcode(p) | 0x02) : setcode(p, getcode(p) & ~0x02))
#define setSprt(p) setlen(p, 4), setcode(p, 0x64)
typedef struct { char pad[0xc90]; u_long a[1]; } OTB;
extern char *D_1F800164;
extern OTB *D_1F8001E0;
extern void SetDrawMode(DR_MODE *, int, int, int, void *);

void func_8011E778(TObj *o)
{
    int i;
    int n;
    short *e;
    char *q;
    u8 *b;
    SPRT *p;
    DR_MODE *dm;
    int tp;

    for (i = 0; i < 2; i++) {
        e = (short *)(o->d3c + ((u16 *)o->anim)[i] * 4);
        n = e[0];
        q = (char *)(*(volatile int *)&o->d3c + e[1]);
        b = (u8 *)q + 0xf;
        tp = *(u16 *)(q + 6);
        do {
            p = (SPRT *)D_1F800164;
            p->w = b[-5];
            p->h = b[-4];
            p->x0 = o->a.p.whole + (signed char)b[-1];
            if (i) p->y0 = o->y.p.whole + (signed char)b[0] - o->timer;
            else p->y0 = o->y.p.whole + (signed char)b[0];
            if (((u16)p->y0 < 0x101 || (u16)(p->y0 + p->h) < 0x101) &&
                ((u16)p->x0 < 0x141 || (u16)(p->x0 + p->w) < 0x141)) {
                setSprt(p);
                setSemiTrans(p, o->b0d >> 7);
                p->code |= 1;
                *(int *)&p->u0 = *(int *)q;
                if (o->b0d & 1) p->clut = o->w08;
                addPrim(&D_1F8001E0->a[(signed char)o->b0f], p);
                D_1F800164 += 0x14;
            }
            b += 0x10;
            n--;
            q += 0x10;
        } while (n != 0);
    }
    dm = (DR_MODE *)D_1F800164;
    SetDrawMode(dm, 0, 0, o->w1e + (short)tp, 0);
    addPrim(&D_1F8001E0->a[(signed char)o->b0f], dm);
    D_1F800164 += 0xc;
}

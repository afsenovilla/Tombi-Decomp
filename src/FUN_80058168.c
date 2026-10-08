// FUNC 80058168 676 MAIN0
// MATCHING 80058168 676
// FLAGS -O2 -G0 -fno-strength-reduce
#include "TOBJ.H"
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short w, h;
} SP;
extern char *DAT_1f800164;
extern char *DAT_1f8001e0;
extern void SetDrawMode(void *, int, int, int, void *);
extern void AddPrim(void *, void *);
extern void SetSprt(void *);
extern void SetSemiTrans(void *, int);

void FUN_80058168(TObj *o)
{
    short pri;
    SP *s;
    short *e;
    char *q;
    unsigned char *p;
    int n;
    int tp;

    if (o->b0b == 2)
        pri = *(unsigned short *)&o->_pad0e;
    else
        pri = (signed char)o->b0f;
    s = (SP *)DAT_1f800164;
    SetDrawMode(s, 0, 0, 0, 0);
    AddPrim(DAT_1f8001e0 + (pri * 4 + 0xc90), s);
    DAT_1f800164 += 0xc;
    e = (short *)(o->d3c + *(unsigned short *)o->anim * 4);
    n = e[0];
    q = (char *)(*(volatile int *)&o->d3c + e[1]);
    p = (unsigned char *)q + 0xf;
    tp = *(unsigned short *)(q + 6);
    do {
        s = (SP *)DAT_1f800164;
        s->w = p[-5];
        s->h = p[-4];
        s->x0 = o->a.p.whole + (signed char)p[-1];
        s->y0 = o->y.p.whole + (signed char)p[0];
        if (((unsigned short)s->y0 < 0x101 || (unsigned short)(s->y0 + s->h) < 0x101)
            && ((unsigned short)s->x0 < 0x141 || (unsigned short)(s->x0 + s->w) < 0x141)) {
            SetSprt(s);
            SetSemiTrans(s, o->b0d >> 7);
            s->code |= 1;
            *(int *)&s->u0 = *(int *)q;
            if (o->b0d & 1)
                s->clut = o->w08;
            AddPrim(DAT_1f8001e0 + (pri * 4 + 0xc90), s);
            DAT_1f800164 += 0x14;
        }
        p += 0x10;
        n--;
        q += 0x10;
    } while (n != 0);
    s = (SP *)DAT_1f800164;
    SetDrawMode(s, 0, 0, o->w1e + (short)tp, 0);
    AddPrim(DAT_1f8001e0 + (pri * 4 + 0xc90), s);
    DAT_1f800164 += 0xc;
}

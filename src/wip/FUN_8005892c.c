// FUNC 8005892c 492 MAIN0
// FLAGS -O2 -G0 -fno-strength-reduce
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    char pad0[0x18];
    char * volatile b18;
    char pad1c[4];
    unsigned short **t20;
    unsigned short **a24;
    char pad28[0x14];
    unsigned char d3c[8];
} O58;
extern SPRT *DAT_1f800164;
extern unsigned long *DAT_1f8001e0;
extern void FUN_800594e4(O58 *, int, int, int);
extern void SetSprt(SPRT *);
extern void SetSemiTrans(SPRT *, int);
extern void AddPrim(void *, void *);
extern void FUN_80059464(int, int);

void FUN_8005892c(O58 *o, int x0, int y0)
{
    int x = x0, y = y0;
    SPRT *p;
    unsigned char *q;
    unsigned short *r;
    short *s;
    int i, n, dx, cnt;

    n = 0;
    q = o->d3c;
    i = 0;
    dx = -0x44;
    do {
        if (i == 7 || *q != 0 || n != 0) {
            FUN_800594e4(o, (short)(x + dx), (short)y, *o->a24[*q]);
            n++;
        }
        q++;
        i++;
        dx += 8;
    } while (i < 8);
    n -= 4;
    if (n < 0)
        n = 0;
    s = (short *)(o->b18 + *o->t20[n] * 4);
    cnt = s[0];
    q = o->b18 + s[1];
    r = (unsigned short *)(q + 2);
    do {
        p = DAT_1f800164;
        SetSprt(p);
        p->code |= 1;
        SetSemiTrans(p, 1);
        p->x0 = x + (signed char)((unsigned char *)r)[12];
        p->y0 = y + (signed char)((unsigned char *)r)[13];
        p->u0 = *q;
        cnt--;
        p->v0 = ((unsigned char *)r)[-1];
        p->w = ((unsigned char *)r)[8];
        p->h = ((unsigned char *)r)[9];
        p->clut = *r;
        AddPrim(DAT_1f8001e0 + 1, p);
        q += 16;
        DAT_1f800164++;
        r += 8;
    } while (cnt != 0);
    FUN_80059464(0x15, 1);
}

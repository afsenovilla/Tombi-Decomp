// FUNC 800595f4 308 MAIN0
typedef struct { unsigned tag; unsigned char r0, g0, b0, code; short x0, y0; unsigned char u0, v0; unsigned short clut; short w, h; } SPRT;
extern SPRT *DAT_1f800164;
extern int DAT_1f8001e0;
extern void SetSprt(SPRT *);
extern void SetSemiTrans(SPRT *, int);
extern void AddPrim(void *, SPRT *);

void FUN_800595f4(int a, short x, short y, unsigned short idx)
{
    short *e = (short *)(*(int *)(a + 0x18) + idx * 4);
    int n;
    unsigned char *s = (unsigned char *)(*(int *)(a + 0x18) + e[1]);
    unsigned short *t = (unsigned short *)(s + 2);
    SPRT *p;
    n = *e;
    do {
        p = DAT_1f800164;
        SetSprt(p);
        p->code |= 1;
        SetSemiTrans(p, 0);
        p->x0 = x + (char)(unsigned char)t[6];
        p->y0 = y + (char)((unsigned char *)t)[0xd];
        p->u0 = *s;
        n--;
        p->v0 = ((unsigned char *)t)[-1];
        p->w = (unsigned char)t[4];
        s += 0x10;
        p->h = ((unsigned char *)t)[9];
        p->clut = *t;
        AddPrim((void *)(DAT_1f8001e0 + 4), p);
        DAT_1f800164 = DAT_1f800164 + 1;
        t += 8;
    } while (n != 0);
}

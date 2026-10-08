// FUNC 800595f4 308 MAIN0
typedef struct { unsigned tag; unsigned char r0, g0, b0, code; short x0, y0; unsigned char u0, v0; unsigned short clut; short w, h; } SPRT;
typedef struct { char pad[0x18]; char *data; } A;
typedef struct { unsigned short clut; unsigned char pad[6]; unsigned char w, h; unsigned char pad2[2]; signed char dx, dy; unsigned char pad3[2]; } T;
extern SPRT *DAT_1f800164;
extern int DAT_1f8001e0;
extern void SetSprt(SPRT *);
extern void SetSemiTrans(SPRT *, int);
extern void AddPrim(void *, SPRT *);

void FUN_800595f4(A *a, short x, short y, unsigned short idx)
{
    short *e;
    int n;
    unsigned char *s;
    T *t;
    SPRT *p;
    e = (short *)(a->data + idx * 4);
    s = (unsigned char *)(a->data + e[1]);
    t = (T *)(s + 2);
    n = *e;
    do {
        p = DAT_1f800164;
        SetSprt(p);
        p->code |= 1;
        SetSemiTrans(p, 0);
        p->x0 = x + t->dx;
        p->y0 = y + t->dy;
        p->u0 = *s;
        n--;
        p->v0 = ((unsigned char *)t)[-1];
        p->w = t->w;
        s += 0x10;
        p->h = t->h;
        p->clut = t->clut;
        AddPrim((void *)(DAT_1f8001e0 + 4), p);
        DAT_1f800164 = DAT_1f800164 + 1;
        t++;
    } while (n != 0);
}

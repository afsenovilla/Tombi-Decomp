// FUNC 800594e4 272 MAIN0
// MATCHING 800594e4 272
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short w, h;
} SPRT;
extern SPRT *DAT_1f800164;
extern int DAT_1f8001e0;
extern void SetSprt(void *);
extern void SetSemiTrans(void *, int);
extern void AddPrim(void *, void *);

void FUN_800594e4(int obj, short x, short y, unsigned short idx)
{
    SPRT *p;
    unsigned char *s;
    int t;
    int tb;
    int i;
    i = idx * 4;
    p = DAT_1f800164;
    s = (unsigned char *)(*(volatile int *)(obj + 0x18) + *(short *)(*(volatile int *)(obj + 0x18) + i + 2));
    SetSprt(p);
    p->code = p->code | 1;
    SetSemiTrans(p, 0);
    p->x0 = x + (signed char)s[0xe];
    p->y0 = y + (signed char)s[0xf];
    p->u0 = s[0];
    p->v0 = s[1];
    t = DAT_1f8001e0;
    p->w = s[10];
    p->h = s[0xb];
    p->clut = *(unsigned short *)(s + 2);
    AddPrim((void *)(t + 4), p);
    DAT_1f800164 = DAT_1f800164 + 1;
}

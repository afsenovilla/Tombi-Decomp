// VERSION SLPS_01144 slps01144 80049704 424
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short w, h;
} Sprt;
typedef struct O { char p0[0x1e]; short v1e; char p1[0x96]; unsigned short b6; char p2[6]; unsigned short a, b, c, d; char p3[4]; unsigned short e; } O;
typedef struct L { short x0, x1, x2, x3; short a, b, c, d; short m; } L;

extern char *NEXT_PRIM;
extern char *CURRENT_OT;
extern void drawMessageGlyphs(int);
extern void SetSprt(Sprt *);
extern unsigned short GetClut(int, int);
extern void AddPrim(void *, void *);
extern int GetGraphType_slps01144(void);
extern void SetDrawMode(void *, int, int, int, void *);
extern void func_800498AC(L *, int, int);

void func_80049704_slps01144(O *o)
{
    char pad[0x60];
    L l;
    Sprt *s;
    char *p;
    int tp;

    drawMessageGlyphs(o->e);
    if (o->b6 == 0 || o->b6 == 3) {
        s = (Sprt *)NEXT_PRIM;
        SetSprt(s);
        s->r0 = 0x80;
        s->g0 = 0x80;
        s->b0 = 0x80;
        s->x0 = o->a + o->c - 48;
        s->y0 = o->b + (o->d >> 1) - 5;
        s->u0 = 0x38;
        s->v0 = 0x86;
        s->w = 0x28;
        s->h = 10;
        s->clut = GetClut(0x160, 0x1e4);
        AddPrim(CURRENT_OT + 8, s);
        NEXT_PRIM += 0x14;
        p = NEXT_PRIM;
        if (GetGraphType_slps01144() == 1 || GetGraphType_slps01144() == 2)
            tp = 0x24;
        else
            tp = 0x14;
        SetDrawMode(p, 0, 0, tp, 0);
        AddPrim(CURRENT_OT + 8, p);
        NEXT_PRIM += 0xc;
    }
    l.m = -1;
    l.a = o->a;
    l.b = o->b;
    l.c = o->c;
    l.d = o->d;
    func_800498AC(&l, 0, o->v1e);
}
#endif

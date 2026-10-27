// FUNC 800eb2f4 500 D0SPSUB
// MATCHING 800eb2f4 500
// FLAGS -O2 -G0 -fno-strength-reduce
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct { unsigned short clut; unsigned char pad[6]; unsigned char w, h; unsigned char pad2[2]; signed char dx, dy; unsigned char pad3[2]; } T;
typedef struct { short x, y, w; unsigned short attr; } Text;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern Text SUBSCREEN_DRAW_PARAMS;
extern short SUBSCREEN_DRAW_TPAGE;
extern short SUBSCREEN_DRAW_CLUT;
extern unsigned short *SUBSCREEN_DRAW_FRAME;
extern char *D_1F8002EC;
#define BANK (*(char * volatile *)&D_1F8002EC)
extern SPRT *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern short D_1F800018;
extern void AddDrawMode(int, int);

void subScreenDrawSprite(int pri)
{
    SPRT *s = (SPRT *)0x1F800000;
    int *cnt = (int *)0x1F800014;
    unsigned short idx = *SUBSCREEN_DRAW_FRAME;
    short *e = (short *)(BANK + idx * 4);
    unsigned char *u;
    T *t;
    SPRT *p;
    *(short *)0x1F800018 = idx;
    u = (unsigned char *)(BANK + e[1]);
    t = (T *)(u + 2);
    *cnt = *e;
    do {
        s->len = 4;
        s->code = 0x65;
        s->r0 = 0x80;
        s->g0 = 0x80;
        s->b0 = 0x80;
        if (SUBSCREEN_DRAW_CLUT & 0x8000) s->code |= 2;
        else s->code &= ~2;
        s->x0 = SUBSCREEN_DRAW_PARAMS.x + t->dx;
        s->y0 = SUBSCREEN_DRAW_PARAMS.y + t->dy;
        s->u0 = *u;
        s->v0 = ((unsigned char *)t)[-1];
        s->w = t->w;
        s->h = t->h;
        if (SUBSCREEN_DRAW_PARAMS.attr & 0x7FFF) s->clut = SUBSCREEN_DRAW_PARAMS.attr;
        else s->clut = t->clut;
        t++;
        p = NEXT_PRIM;
        u += 0x10;
        *p = *s;
        addPrim(CURRENT_OT + pri + 4, p);
        NEXT_PRIM++;
    } while (--*cnt);
    AddDrawMode(SUBSCREEN_DRAW_TPAGE, pri + 4);
}

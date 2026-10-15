// FUNC 800eb440 704 D0SPSUB3
// MATCHING 800eb440 704
// FLAGS -O2 -G0 -fno-strength-reduce
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;
typedef struct { unsigned short clut; unsigned char u1, v1; unsigned char pad[2]; unsigned char u2, v2; unsigned char w, h; unsigned char u3, v3; signed char dx, dy; unsigned char pad3[2]; } T;
typedef struct { short x, y, w; unsigned short attr; } Text;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern Text D_80100B70;
extern unsigned short D_80100B76;
extern unsigned short *D_80100B78;
extern char *D_1F8002EC;
#define BANK (*(char * volatile *)&D_1F8002EC)
extern POLY_FT4 *D_1F800164;
extern unsigned long *D_1F8001E0;

void func_800EB440(int pri)
{
    POLY_FT4 *s = (POLY_FT4 *)0x1F800000;
    int *cnt = (int *)0x1F800028;
    unsigned short idx = *D_80100B78;
    short *e = (short *)(BANK + idx * 4);
    unsigned char *u;
    T *t;
    POLY_FT4 *p;
    *(short *)0x1F80002C = idx;
    u = (unsigned char *)(BANK + e[1]);
    t = (T *)(u + 2);
    *cnt = *e;
    do {
        s->len = 9;
        s->code = 0x2D;
        s->r0 = 0x80;
        s->g0 = 0x80;
        s->b0 = 0x80;
        if (D_80100B76 & 0x80) s->code |= 2;
        else s->code &= ~2;
        s->x0 = D_80100B70.x + t->dx;
        s->y0 = D_80100B70.y + t->dy;
        s->x1 = s->x0 + t->w;
        s->x2 = s->x0;
        s->y1 = s->y0;
        s->y2 = s->y0 + t->h;
        s->x3 = s->x1;
        s->y3 = s->y2;
        s->u0 = *u;
        s->v0 = ((unsigned char *)t)[-1];
        s->u1 = t->u1;
        s->v1 = t->v1;
        s->u2 = t->u2;
        s->v2 = t->v2;
        s->u3 = t->u3;
        s->v3 = t->v3;
        s->tpage = D_80100B70.w;
        if (D_80100B70.attr & 0x7F) s->clut = D_80100B70.attr;
        else s->clut = t->clut;
        p = D_1F800164;
        *p = *s;
        if (*cnt == 1) addPrim(D_1F8001E0 + pri + 3, p);
        else addPrim(D_1F8001E0 + pri + 4, p);
        t++;
        u += 0x10;
        D_1F800164++;
    } while (--*cnt);
}

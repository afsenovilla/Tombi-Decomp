// FUNC 800eb61c 812 D0SPSUB2
// MATCHING 800eb61c 812
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
typedef struct { short x, y, w, h; } RECT;
typedef struct {
    RECT clip;
    short ofs[2];
    RECT tw;
    unsigned short tpage;
    unsigned char dtd, dfe, isbg, r0, g0, b0;
    unsigned long dr_env[17];
} DRAWENV;
typedef struct { unsigned char u, v, w, h, flag; } Glyph;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
typedef struct { short x, y, w; unsigned short attr; int img, pC; unsigned short *str; Glyph *font; } Text;
extern Text D_800F70D8;
extern short D_1F8003D4, D_1F8003D6;
extern SPRT *D_1F800164;
extern unsigned long *D_1F8001E0;
extern DRAWENV *GetDrawEnv(DRAWENV *env);
extern void AddDrawMode(int tpage, int pri);

void func_800EB61C(int pri, int mode)
{
    DRAWENV env;
    Glyph g;
    SPRT *s = (SPRT *)0x1F800000;
    SPRT *p;
    register unsigned short *str asm("$8");
    register unsigned short x asm("$7");
    unsigned short y;
    short w;
    unsigned short c;
    Glyph *font;
    Glyph *gp;
    GetDrawEnv(&env);
    AddDrawMode(env.tpage, pri);
    w = 0;
    str = D_800F70D8.str;
    x = D_800F70D8.x;
    y = D_800F70D8.y;
    font = D_800F70D8.font;
    D_1F8003D4 = 0;
    D_1F8003D6 = 0;
    while ((short)(c = *str++) != -1) {
        if (mode == 2 && (short)c == 0xA8) {
            x = D_800F70D8.x = 0x50;
            D_800F70D8.y += 0x10;
            y = D_800F70D8.y;
        }
        switch ((short)c) {
        case -7:
        case -5:
        case -1:
            break;
        case -6:
            str += 3;
            break;
        case -3:
            if (mode) {
                D_1F8003D6 += 8;
                x = D_800F70D8.x;
                y += 9;
                if (D_1F8003D4 < w) D_1F8003D4 = w;
                w = 0;
            }
            break;
        default:
            gp = (Glyph *)(((short)c * 5) + (unsigned long)font);
            g.u = gp->u;
            g.v = gp->v;
            g.w = gp->w;
            g.h = gp->h;
            g.flag = gp->flag;
            w += 8;
            s->len = 4;
            s->code = 0x64;
            s->r0 = 0x80;
            s->g0 = 0x80;
            s->b0 = 0x80;
            s->x0 = x;
            if (g.flag == 0) {
                if (g.w >= 0xB) s->y0 = y - 2;
                else s->y0 = y;
                s->u0 = g.u;
                s->v0 = g.v;
                { int t = g.w; x += t; }
            } else {
                s->x0 = x - 0xB;
                s->y0 = y - 0xA;
                s->u0 = g.u;
                s->v0 = g.v;
            }
            s->w = g.w;
            s->h = g.h;
            s->clut = D_800F70D8.attr;
            p = D_1F800164;
            *p = *s;
            addPrim(D_1F8001E0 + pri, p);
            D_1F800164++;
            break;
        }
    }
    if (w > D_1F8003D4) D_1F8003D4 = w;
    AddDrawMode(D_800F70D8.w, pri);
}

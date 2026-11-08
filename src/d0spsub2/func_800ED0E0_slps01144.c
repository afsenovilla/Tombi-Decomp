// VERSION SLPS_01144 dspsub2 800ED0E0 616
#if defined(VERSION_SLPS_01144)
// JP memCardDrawString (PAL d0spsub2 800eb61c): no mode, no width tracking, 14-pixel lines
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
asm(".set NP_,0x1f800164");
asm(".set OT_,0x1f8001e0");
extern SPRT *NP_;
extern unsigned long *OT_;
/* the draw params (Text), field by field as retail names them */
extern unsigned short D_800F3080, D_800F3082, D_800F3086;
extern short D_800F3084;
extern unsigned short *D_800F3090;
extern Glyph *D_800F3094;
extern DRAWENV *func_8005C2DC(DRAWENV *env); /* GetDrawEnv */
extern void func_8004C7BC(int tpage, int pri); /* AddDrawMode */

void func_800ED0E0(int pri)
{
    DRAWENV env;
    Glyph g;
    SPRT *s = (SPRT *)0x1F800000;
    SPRT *p;
    register unsigned short *str asm("$7");
    register unsigned short x asm("$8");
    unsigned short y;
    unsigned short c;
    Glyph *font;
    Glyph *gp;
    func_8005C2DC(&env);
    func_8004C7BC(env.tpage, pri);
    str = D_800F3090;
    x = D_800F3080;
    y = D_800F3082;
    font = D_800F3094;
    while ((short)(c = *str++) != -1) {
        switch ((short)c) {
        case -7:
        case -5:
        case -1:
            break;
        case -6:
            str += 3;
            break;
        case -3:
            x = D_800F3080;
            y += 14;
            break;
        default:
            gp = (Glyph *)(((short)c * 5) + (unsigned long)font);
            g.u = gp->u;
            g.v = gp->v;
            g.w = gp->w;
            g.h = gp->h;
            g.flag = gp->flag;
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
            s->clut = D_800F3086;
            p = NP_;
            *p = *s;
            addPrim(OT_ + pri, p);
            NP_++;
            break;
        }
    }
    func_8004C7BC(D_800F3084, pri);
}
#endif

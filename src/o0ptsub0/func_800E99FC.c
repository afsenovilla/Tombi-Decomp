// FUNC 800e99fc 780 O0PTSUB0
// MATCHING 800e99fc 780
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
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned long code[2];
} DR_MODE;
typedef struct { unsigned char u, v, w, h, big; } Glyph;
typedef struct { short x, y, tp, pad; short *str; Glyph *font; unsigned char *pal; } MenuItem;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern char *D_1F800164;
extern unsigned long *DAT_1f8001e0;
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void func_800E99FC(MenuItem *m, int pri, int wrap, int pal)
{
    unsigned char pad[0x60];
    unsigned char g[5];
    Glyph *font;
    Glyph *f;
    SPRT *sp = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *dm;
    short *s;
    unsigned short x, y;
    short c;

    s = m->str;
    x = m->x;
    y = m->y;
    font = m->font;
    c = *s;
    while (*s++ != -1) {
        switch ((short)c) {
        case -7:
        case -5:
        case -1:
            break;
        case -6:
            s += 3;
            break;
        case -3:
            if (wrap) {
                x = m->x;
                y += 8;
            }
            break;
        default:
            f = (Glyph *)((short)c * 5 + (int)font);
            g[0] = f->u;
            g[1] = f->v;
            g[2] = f->w;
            g[3] = f->h;
            if ((short)c == 0) {
                x += g[2];
                break;
            }
            g[4] = f->big;
            sp->len = 4;
            sp->code = 0x64;
            sp->r0 = 0x80;
            sp->g0 = 0x80;
            sp->b0 = 0x80;
            sp->x0 = x;
            if (g[4] == 0) {
                sp->x0 = x;
                sp->y0 = y;
                sp->u0 = g[0];
                x += g[2];
                sp->v0 = g[1];
            } else {
                sp->x0 = x - 10;
                sp->y0 = y - 10;
                sp->u0 = g[0];
                sp->v0 = g[1];
            }
            sp->w = g[2];
            sp->h = g[3];
            switch (pal) {
            case 0:
                sp->clut = 0x7808;
                break;
            case 1:
                sp->clut = ((*m->pal++ + 0x1E0) << 6) | 0x15;
                break;
            case 2:
                sp->clut = 0x7C15;
                break;
            case 3:
                sp->clut = 0x7855;
                break;
            }
            p = (SPRT *)D_1F800164;
            *p = *sp;
            addPrim(DAT_1f8001e0 + pri, p);
            D_1F800164 += sizeof(SPRT);
            break;
        }
        c = *s;
    }
    dm = (DR_MODE *)D_1F800164;
    SetDrawMode(dm, 0, 0, m->tp, 0);
    addPrim(DAT_1f8001e0 + pri, dm);
    D_1F800164 += sizeof(DR_MODE);
}

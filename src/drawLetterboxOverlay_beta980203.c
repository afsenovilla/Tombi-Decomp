// VERSION BETA_980203 beta980203 8004CF98 424
#if defined(VERSION_BETA_980203)
// Beta version: the sprites only (no 0x18 offset, no black bars)
#include "TOBJ.H"
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short w, h;
} SPRT;
typedef struct { unsigned long tag; unsigned long code[2]; } DR_MODE;
typedef struct { short x, y, w, h; } RECT;
typedef struct { short mode, x, y; unsigned char u, v; short w, h, cx, cy, abe; } ENT;
extern ENT *PTR_DAT_80080258[];
extern char *NEXT_PRIM;
extern int CURRENT_OT;
extern void SetSprt(SPRT *);
extern void SetSemiTrans(void *, int);
extern unsigned short GetClut(int, int);
extern void AddPrim(void *, void *);
extern void SetDrawMode(DR_MODE *, int, int, int, RECT *);
extern void addTilePrim(RECT *, int, int, int);
typedef struct { char pad[0xc90]; int a[1]; } OTB;

void drawLetterboxOverlay(TObj *o)
{
    ENT *e;
    SPRT *p;
    DR_MODE *dm;
    for (e = PTR_DAT_80080258[o->subtype]; e->mode != 0x7fff; e++) {
        p = (SPRT *)NEXT_PRIM;
        SetSprt(p);
        SetSemiTrans(p, e->abe);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->code |= 1;
        p->w = e->w;
        p->h = e->h;
        p->x0 = o->a.p.whole + e->x;
        p->y0 = o->y.p.whole + e->y;
        p->u0 = e->u;
        p->v0 = e->v;
        p->clut = GetClut(e->cx, e->cy);
        AddPrim(&((OTB *)CURRENT_OT)->a[(signed char)o->b0f], p);
        NEXT_PRIM += sizeof(SPRT);
        dm = (DR_MODE *)NEXT_PRIM;
        SetDrawMode(dm, 0, 0, e->mode, 0);
        AddPrim(&((OTB *)CURRENT_OT)->a[(signed char)o->b0f], dm);
        NEXT_PRIM += sizeof(DR_MODE);
    }
}
#endif

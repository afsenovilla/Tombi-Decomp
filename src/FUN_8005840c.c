// FUNC 8005840c 504 MAIN0
// MATCHING 8005840c 504
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
extern char *DAT_1f800164;
extern int DAT_1f8001e0;
extern void SetSprt(SPRT *);
extern void SetSemiTrans(void *, int);
extern unsigned short GetClut(int, int);
extern void AddPrim(void *, void *);
extern void SetDrawMode(DR_MODE *, int, int, int, RECT *);
extern void FUN_80059728(RECT *, int, int, int);
typedef struct { char pad[0xc90]; int a[1]; } OTB;

void FUN_8005840c(TObj *o)
{
    ENT *e;
    SPRT *p;
    DR_MODE *dm;
    RECT r;
    for (e = PTR_DAT_80080258[o->subtype]; e->mode != 0x7fff; e++) {
        p = (SPRT *)DAT_1f800164;
        SetSprt(p);
        SetSemiTrans(p, e->abe);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->code |= 1;
        p->w = e->w;
        p->h = e->h;
        p->x0 = o->a.p.whole + e->x;
        p->y0 = o->y.p.whole + e->y + 0x18;
        p->u0 = e->u;
        p->v0 = e->v;
        p->clut = GetClut(e->cx, e->cy);
        AddPrim(&((OTB *)DAT_1f8001e0)->a[(signed char)o->b0f], p);
        DAT_1f800164 += sizeof(SPRT);
        dm = (DR_MODE *)DAT_1f800164;
        SetDrawMode(dm, 0, 0, e->mode, 0);
        AddPrim(&((OTB *)DAT_1f8001e0)->a[(signed char)o->b0f], dm);
        DAT_1f800164 += sizeof(DR_MODE);
    }
    r.x = 0;
    r.y = 0;
    r.w = 0x140;
    r.h = 0x10;
    FUN_80059728(&r, 0, 0, 1);
    r.y = 0xf0;
    FUN_80059728(&r, 0, 0, 1);
}

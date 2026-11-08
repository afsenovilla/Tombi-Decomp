// VERSION SLPS_01144 info 800EC658 752
// JP version of PAL i0nfo 800eb044 (overheadDrawMapLayers.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u_long;
typedef struct { unsigned addr : 24; unsigned len : 8; u8 r0, g0, b0, code; } P_TAG;
typedef struct { u_long tag; u8 r0, g0, b0, code; short x0, y0; u8 u0, v0; u16 clut; u16 w, h; } SPRT;
typedef struct { u_long tag; u_long code[2]; } DR_MODE;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u_long)(_addr))
#define getaddr(p) (u_long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
typedef struct { char pad[0xc90]; u_long a[1]; } OTB;
extern char *NEXT_PRIM;
extern OTB *CURRENT_OT;
extern void func_8005C954(DR_MODE *, int, int, int, void *);
typedef struct {
    unsigned short tp0, clut0, tp1, clut1, tp2, clut2;
} TexInfo;
typedef struct { unsigned short v; } U16S;
extern U16S D_800A61B0, D_800A61B2, D_800A61B4, D_800A61B6;
typedef struct { u16 x, y, u, v, w, h; } Ent8D0;
extern Ent8D0 D_800EFD84[];

void func_800EC658(TObj *o)
{
    short x, y, i;
    SPRT *p;
    DR_MODE *dm;
    Ent8D0 *e;
    u8 c;

    x = o->h->p.whole;
    if (x > 0) x -= 0x20;
    y = o->y.p.whole;
    if (y > 0) y -= 0x20;
    if (o->b6b != 0) {
        dm = (DR_MODE *)NEXT_PRIM;
        func_8005C954(dm, 0, 0, 0, 0);
        addPrim(&CURRENT_OT->a[(signed char)o->b0f], dm);
        NEXT_PRIM += 0xC;
        i = 0;
        do {
            p = (SPRT *)NEXT_PRIM;
            *((u8 *)p + 3) = 4;
            p->code = 0x65;
            e = &D_800EFD84[i];
            p->x0 = e->x + x;
            p->y0 = e->y + y;
            p->u0 = e->u;
            p->v0 = e->v;
            dm = (DR_MODE *)(NEXT_PRIM += 0x14);
            p->w = e->w;
            p->h = e->h;
            if (i == 0) {
                p->clut = D_800A61B2.v;
                func_8005C954(dm, 0, 0, D_800A61B0.v, 0);
            } else {
                p->clut = D_800A61B6.v;
                func_8005C954(dm, 0, 0, D_800A61B4.v, 0);
            }
            c = o->b6b << 7;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            addPrim(&CURRENT_OT->a[(signed char)o->b0f], p);
            i++;
            addPrim(&CURRENT_OT->a[(signed char)o->b0f], dm);
            NEXT_PRIM += 0xC;
        } while (i < 2);
    }
}
#endif

// FUNC 800ec710 868 I0NFO2
// MATCHING 800ec710 868
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
extern char *D_1F800164;
extern OTB *D_1F8001E0;
extern void SetDrawMode(DR_MODE *, int, int, int, void *);
typedef struct {
    unsigned short tp0, clut0, tp1, clut1, tp2, clut2;
} TexInfo;
extern TexInfo D_800A4668;
extern unsigned short D_8009C960;
typedef struct { u16 x, y, u, v, w, h; } Ent8D0;
extern Ent8D0 D_800FECB8[];

void topdownDrawMapLayers(TObj *o)
{
    short x, y, i;
    SPRT *p;
    DR_MODE *dm;
    Ent8D0 *e;
    u8 c;
    int a;

    x = o->h->p.whole;
    if (x > 0) x -= 0x20;
    y = o->y.p.whole;
    if (y > 0) y -= 0x20;
    if (o->b6b != 0) {
        dm = (DR_MODE *)D_1F800164;
        SetDrawMode(dm, 0, 0, 0, 0);
        addPrim(&D_1F8001E0->a[(signed char)o->b0f], dm);
        D_1F800164 += 0xC;
        i = 0;
        do {
            a = 0;
            if (i >= 2) a = (D_8009C960 == 5) << 7;
            p = (SPRT *)D_1F800164;
            *((u8 *)p + 3) = 4;
            p->code = 0x65;
            e = &D_800FECB8[i];
            p->x0 = e->x + x;
            p->y0 = e->y + y;
            p->u0 = e->u;
            p->v0 = a + e->v;
            dm = (DR_MODE *)(D_1F800164 += 0x14);
            p->w = e->w;
            p->h = e->h;
            switch (i) {
            case 0:
                p->clut = D_800A4668.clut0;
                SetDrawMode(dm, 0, 0, D_800A4668.tp0, 0);
                break;
            case 1:
                p->clut = D_800A4668.clut1;
                SetDrawMode(dm, 0, 0, D_800A4668.tp1, 0);
                break;
            case 2: case 3: case 4:
                p->clut = D_800A4668.clut2;
                SetDrawMode(dm, 0, 0, D_800A4668.tp2, 0);
                break;
            }
            c = o->b6b << 7;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            addPrim(&D_1F8001E0->a[(signed char)o->b0f], p);
            i++;
            addPrim(&D_1F8001E0->a[(signed char)o->b0f], dm);
            D_1F800164 += 0xC;
        } while (i < 5);
    }
}

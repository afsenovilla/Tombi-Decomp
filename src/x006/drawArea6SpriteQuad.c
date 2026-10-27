// FUNC 8011cef0 716 X006
// MATCHING 8011cef0 716
// VERSION SLPS_01144 x06 8011C070 716
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x06/nonmatchings/x06", func_8011C070_slps01144);
#else
#include "spad.h"
#include "TOBJ.H"

typedef struct {
    unsigned int tag;
    unsigned char r, g, b, code;
    short x0, y0; unsigned short uv0, clut;
    short x1, y1; unsigned short uv1, tpage;
    short x2, y2; unsigned short uv2, pad2;
    short x3, y3; unsigned short uv3, pad3;
} FT4;

typedef struct {
    int uvclut;
    int uvtp;
    unsigned short uv2;
    unsigned char w, h;
    unsigned short uv3;
    signed char dx, dy;
} EL;

extern int LZ_FILE_CTRL;
extern FT4 *NEXT_PRIM;
extern int projectActorPosition(void *, int *, int *);
extern int isQuadOnScreen(void *);
extern void SetSemiTrans(void *, int);

static __inline__ int ot(unsigned *a, int b, int c, int z, unsigned e)
{
    int d = (z + c) << 2;
    if (d < 0) d = 0;
    d += b;
    if ((unsigned)(d - SPAD.i1e0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)d;
        *(unsigned *)d = (unsigned)a;
        *a = v | e;
    }
    return 0;
}

void drawArea6SpriteQuad(TObj *o)
{
    short *f;
    int n;
    EL *e;
    FT4 *p;
    int r;

    if (o->anim == 0) return;
    projectActorPosition(o, &LZ_FILE_CTRL, &SPAD.i74);
    f = (short *)(o->d3c + (*(short *)o->anim << 2));
    n = f[0];
    e = (EL *)(*(volatile int *)&o->d3c + f[1]);
    do {
        p = NEXT_PRIM;
        if (o->animFrame & 1) {
            p->x0 = e->dx + LZ_FILE_CTRL;
            p->y0 = e->dy + (LZ_FILE_CTRL >> 16);
            p->x1 = p->x0 + e->w;
            p->x2 = p->x0;
            p->y1 = *(short *)((char *)p + 10);
            p->y2 = p->y0 + e->h;
            p->x3 = p->x1;
            p->y3 = p->y2;
        } else {
            p->x0 = LZ_FILE_CTRL - e->dx;
            p->y0 = e->dy + (LZ_FILE_CTRL >> 16);
            p->x1 = p->x0 - e->w;
            p->x2 = p->x0;
            p->y1 = *(short *)((char *)p + 10);
            p->y2 = p->y0 + e->h;
            p->x3 = p->x1;
            p->y3 = p->y2;
        }
        if (isQuadOnScreen(p)) {
            p->code = 0x2d;
            SetSemiTrans(p, o->b0d >> 7);
            *(int *)&p->uv0 = e->uvclut;
            *(int *)&p->uv1 = e->uvtp;
            p->uv2 = e->uv2;
            p->uv3 = e->uv3;
            p->tpage += o->w1e;
            if (o->b0d & 1) p->clut = o->w08;
            if (o->b0b) r = ot((unsigned *)p, SPAD.i1e0 + 0x10, 0, (signed char)o->b0f, 0x9000000);
            else { int c = SPAD.i74; r = ot((unsigned *)p, SPAD.i1e0 + 0x10, c, (signed char)o->b0f, 0x9000000); }
            if (!r) NEXT_PRIM = (FT4 *)((char *)NEXT_PRIM + 0x28);
        }
        e++;
    } while (--n);
}
#endif

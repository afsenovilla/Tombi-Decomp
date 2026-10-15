// FUNC 8011f240 1836 X009
// MATCHING 8011f240 1836
#include "spad.h"
/* Rotated FT4 sprite strip (family of drawObjectSpriteRotatedFT4). All element reads go through the one pointer q: loop.c then
   combines the 7 address givs into a reduced reg based on the LAST one (q+12 = uv3), which is the game's s2;
   a separate f = q+12 pointer gets reduced to q+8 instead. p = D_1F800164 is read between the A/B and C/D bytes. */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; unsigned short uv0, clut;
    short x1, y1; unsigned short uv1, tpage;
    short x2, y2; unsigned short uv2, pad2;
    short x3, y3; unsigned short uv3, pad3;
} PFT4;

extern long LZ_FILE_CTRL;
extern int D_1F800074i;
extern int D_1F800078;
extern PFT4 *D_1F800164;
extern short D_8007A5F0[];
extern short D_8007A3F0[];
int projectActorPosition(void *o, long *sxy, long *z);
int isQuadOnScreen(void *p);
int insertPrim(void *a, int b, int c, int d, unsigned e);
int insertPrimWithBias(void *a, int b, int c, int d, unsigned e);

#define setSemiTrans(p, abe) ((abe) ? ((p)->code |= 2) : ((p)->code &= ~2))
#define SN(i) D_8007A5F0[i]
#define a1 ((ang + 0x80) & 0xff)
#define a0 (ang & 0xff)
#define CS(i) D_8007A3F0[i]

void drawRotatedSpriteKind8(TObj *o)
{
    short *e;
    unsigned char *q;
    int xy;
    short x;
    short y;
    int d;
    int ang;
    short A, B, C, D;
    short u, w;
    PFT4 *p;
    int r;

    if (o->anim == 0)
        return;
    projectActorPosition(o, &LZ_FILE_CTRL, &SPAD.i74);
    xy = LZ_FILE_CTRL;
    x = xy;
    y = (unsigned)xy >> 16;
    ang = o->d8c;
    d = o->d3c;
    d += *(unsigned short *)o->anim * 4;
    e = (short *)d;
    q = (unsigned char *)(o->d3c + e[1]);
    D_1F800078 = e[0];
    do {
        A = (signed char)q[14];
        B = (signed char)q[15];
        p = D_1F800164;
        C = q[10];
        D = q[11];
        if (o->animFrame & 1) {
            u = A;
            w = B;
            p->x0 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            p->y0 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            p->x1 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            p->y1 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            p->x2 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            p->y2 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            p->x3 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            p->y3 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        } else {
            u = -(A + C);
            w = B;
            p->x1 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            p->y1 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            p->x0 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            p->y0 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            p->x3 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            p->y3 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            p->x2 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            p->y2 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        }
        if (isQuadOnScreen(p)) {
            p->code = 0x2d;
            setSemiTrans(p, o->b0d >> 7);
            *(int *)&p->uv0 = *(int *)q;
            *(int *)&p->uv1 = *(int *)(q + 4);
            p->uv2 = *(unsigned short *)(q + 8);
            p->uv3 = *(unsigned short *)(q + 12);
            p->tpage += o->w1e;
            if (o->b0d & 1)
                p->clut = o->w08;
            p->code &= ~1;
            p->r = *(int *)&o->w5c;
            p->g = o->d60;
            p->b = o->d64;
            if (o->b0b != 0)
                r = insertPrim(p, SPAD.i1e0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            else
                r = insertPrimWithBias(p, SPAD.i1e0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            if (!r)
                D_1F800164 = (PFT4 *)((char *)D_1F800164 + 0x28);
        }
        q += 16;
    } while (--D_1F800078);
}

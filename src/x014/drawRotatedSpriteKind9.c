// FUNC 8011b830 2012 X014
// MATCHING 8011b830 2012
// VERSION SLPS_01144 x14 80118D60 2012
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x14/nonmatchings/x14", func_80118D60_slps01144);
#else
#include "spad.h"
/* Rotated/scaled FT4 sprite strip (X014 twin of drawObjectSpriteRotatedFT4; csv said 1660 B, real function 2012 B).
   Keys: per-axis scale from ba5/ba6 as `c = (signed char)o->ba5; short sc = (signed char)c; if (sc) ... (unsigned char)sc`
   (the folded QI sign-extension leaves two dead combine USEs = the 16 B frame gap at sp+0x20); single frame pointer q with
   q[10..15]/q+4/q+8/q+12 offsets so strength reduction makes the q+12 giv (s2) itself and keeps q on the stack. */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; int uv0;
    short x1, y1; int uv1;
    short x2, y2; unsigned short uv2, pad2;
    short x3, y3; unsigned short uv3, pad3;
} PFT4_8011B830;

extern long LZ_FILE_CTRL;
extern int D_1F800074i;
extern int D_1F800078;
extern PFT4_8011B830 *D_1F800164;
extern short COS_TABLE[];
extern short NEG_SIN_TABLE[];
int projectActorPosition(void *o, long *sxy, long *z);
int isQuadOnScreen(void *p);
int insertPrim(void *a, int b, int c, int d, unsigned e);
int insertPrimWithBias(void *a, int b, int c, int d, unsigned e);

#define setSemiTrans(p, abe) ((abe) ? ((p)->code |= 2) : ((p)->code &= ~2))
#define SN(i) COS_TABLE[i]
#define a1 ((ang + 0x80) & 0xff)
#define a0 (ang & 0xff)
#define CS(i) NEG_SIN_TABLE[i]

void drawRotatedSpriteKind9(TObj *o)
{
    short *e;
    unsigned char *q;
    int xy;
    short x;
    unsigned int y;
    unsigned int s;
    int d;
    int ang;
    short A, B, C, D;
    short u, w;
    int c;
    short sc;
    PFT4_8011B830 *f;

    if (o->anim == 0) return;
    projectActorPosition(o, &LZ_FILE_CTRL, &SPAD.i74);
    xy = LZ_FILE_CTRL;
    x = xy;
    y = (unsigned)xy >> 16;
    d = o->d3c;
    d += *(unsigned short *)o->anim * 4;
    e = (short *)d;
    q = (unsigned char *)(o->d3c + e[1]);
    D_1F800078 = e[0];
    ang = o->d8c;
    do {
        f = D_1F800164;
        c = (signed char)o->ba5;
        sc = (signed char)c;
        if (sc != 0) {
            if (o->ba7 != 0) {
                s = 0x100 - (unsigned char)sc;
                if (s == 0) goto next;
            } else {
                s = (unsigned char)sc + 0x100;
            }
            A = (((signed char *)q)[14] * s) >> 8;
            C = (q[10] * s) >> 8;
        } else {
            A = (signed char)q[14];
            C = q[10];
        }
        c = (signed char)o->ba6;
        sc = (signed char)c;
        if (sc != 0) {
            if (o->ba7 != 0) {
                s = 0x100 - (unsigned char)sc;
                if (s == 0) goto next;
            } else {
                s = (unsigned char)sc + 0x100;
            }
            B = (((signed char *)q)[15] * s) >> 8;
            D = (q[11] * s) >> 8;
        } else {
            B = (signed char)q[15];
            D = q[11];
        }
        if (o->animFrame & 1) {
            u = A;
            w = B;
            f->x0 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y0 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x1 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y1 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x2 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y2 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x3 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y3 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        } else {
            u = -(A + C);
            w = B;
            f->x1 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y1 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x0 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y0 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x3 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y3 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x2 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y2 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        }
        if (isQuadOnScreen(f)) {
            f->code = 0x2d;
            setSemiTrans(f, o->b0d >> 7);
            f->uv0 = *(int *)q;
            f->uv1 = *(int *)(q + 4);
            f->uv2 = *(unsigned short *)(q + 8);
            f->uv3 = *(unsigned short *)(q + 12);
            *(unsigned short *)((char *)f + 0x16) += o->w1e;
            if (o->b0d & 1) *(unsigned short *)((char *)f + 0xe) = o->w08;
            if (o->b0b != 0)
                insertPrim(f, SPAD.i1e0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            else
                insertPrimWithBias(f, SPAD.i1e0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            D_1F800164 = (PFT4_8011B830 *)((char *)D_1F800164 + 0x28);
        }
    next:
        q += 16;
    } while (--D_1F800078 != 0);
}
#endif

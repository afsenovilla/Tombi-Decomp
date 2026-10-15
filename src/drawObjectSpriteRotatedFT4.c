// FUNC 80123fd0 1968 X000
// MATCHING 80123fd0 1968
// VERSION SLPM_80165 x00 8011A394 1964
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80123FD0 (drawObjectSpriteRotatedFT4.c): No off-screen early exit, texture rows from v 0 (PAL 0xa0), then func_8003F160.
#include "spad.h"
/* Rotated FT4 sprite strip. Keys: short x = xy (keeps the move s3,a0 copy), and the Psy-Q setSemiTrans macro
   form (if/else |= 2 / &= ~2 after f->code = 0x2d): cse folds both branches to constants and deletes the dead else
   store, but the extra insns at flow time make the scale s keep fp over &CS[a1] (global-alloc tie) and put the 0x2d
   constant in v1. */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad1;
    short x3, y3; unsigned char u3, v3; unsigned short pad2;
} PFT4_80123FD0;

extern long LZ_FILE_CTRL;
extern int D_1F800074i;
extern PFT4_80123FD0 *D_1F800164;
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

extern void func_8003F160(TObj *, int);
extern int D_1F800070;
void drawObjectSpriteRotatedFT4(TObj *o)
{
    short *e;
    unsigned char *q;
    int n;
    int xy;
    short x;
    short y;
    unsigned int s;
    int d;
    int ang;
    short A, B, C, D;
    short u, w;
    PFT4_80123FD0 *f;

    projectActorPosition(o, &LZ_FILE_CTRL, &SPAD.i74);
    xy = LZ_FILE_CTRL;
    x = xy;
    y = (unsigned)xy >> 16;
    d = o->d3c;
    d += *(unsigned short *)o->anim * 4;
    e = (short *)d;
    n = e[0];
    q = (unsigned char *)(o->d3c + e[1]);
    switch ((*(short*)&SPAD.s1c8)) {
    case 0:
        s = ((*(short*)((char*)&SPAD.if4 + 2)) - o->b.p.whole) * 7 + 0x1000;
        break;
    case 1:
        s = (o->a.p.whole - (*(short*)((char*)&SPAD.iec + 2))) * 7 + 0x1000;
        break;
    }
    ang = o->d8c;
    do {
        A = ((signed char)q[14] * s - 0x1800) >> 12;
        B = ((signed char)q[15] * s - 0x1800) >> 12;
        C = (q[10] * s + 0x1800) >> 12;
        D = (q[11] * s + 0x1800) >> 12;
        f = D_1F800164;
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
            f->u0 = ((unsigned char *)o)[0xc6] << 7;
            f->v0 = 0;
            f->u1 = q[10] + (((unsigned char *)o)[0xc6] << 7);
            f->v1 = 0;
            f->u2 = ((unsigned char *)o)[0xc6] << 7;
            f->v2 = q[11];
            f->u3 = q[10] + (((unsigned char *)o)[0xc6] << 7);
            f->v3 = q[11];
            f->tpage = o->w1e;
            f->clut = o->w08;
            if (o->b0b != 0)
                insertPrim(f, SPAD.i1e0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            else
                insertPrimWithBias(f, SPAD.i1e0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            D_1F800164 = (PFT4_80123FD0 *)((char *)D_1F800164 + 0x28);
        }
        q += 16;
    } while (--n);
    func_8003F160(o, D_1F800070);
}
#else
#include "spad.h"
/* Rotated FT4 sprite strip. Keys: short x = xy (keeps the move s3,a0 copy), and the Psy-Q setSemiTrans macro
   form (if/else |= 2 / &= ~2 after f->code = 0x2d): cse folds both branches to constants and deletes the dead else
   store, but the extra insns at flow time make the scale s keep fp over &CS[a1] (global-alloc tie) and put the 0x2d
   constant in v1. */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad1;
    short x3, y3; unsigned char u3, v3; unsigned short pad2;
} PFT4_80123FD0;

extern long LZ_FILE_CTRL;
extern int D_1F800074i;
extern PFT4_80123FD0 *D_1F800164;
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

void drawObjectSpriteRotatedFT4(TObj *o)
{
    short *e;
    unsigned char *q;
    int n;
    int xy;
    short x;
    short y;
    unsigned int s;
    int d;
    int ang;
    short A, B, C, D;
    short u, w;
    PFT4_80123FD0 *f;

    if (projectActorPosition(o, &LZ_FILE_CTRL, &SPAD.i74) != 0)
        return;
    xy = LZ_FILE_CTRL;
    x = xy;
    y = (unsigned)xy >> 16;
    d = o->d3c;
    d += *(unsigned short *)o->anim * 4;
    e = (short *)d;
    n = e[0];
    q = (unsigned char *)(o->d3c + e[1]);
    switch ((*(short*)&SPAD.s1c8)) {
    case 0:
        s = ((*(short*)((char*)&SPAD.if4 + 2)) - o->b.p.whole) * 7 + 0x1000;
        break;
    case 1:
        s = (o->a.p.whole - (*(short*)((char*)&SPAD.iec + 2))) * 7 + 0x1000;
        break;
    }
    ang = o->d8c;
    do {
        A = ((signed char)q[14] * s - 0x1800) >> 12;
        B = ((signed char)q[15] * s - 0x1800) >> 12;
        C = (q[10] * s + 0x1800) >> 12;
        D = (q[11] * s + 0x1800) >> 12;
        f = D_1F800164;
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
            f->u0 = ((unsigned char *)o)[0xc6] << 7;
            f->v0 = 0xa0;
            f->u1 = q[10] + (((unsigned char *)o)[0xc6] << 7);
            f->v1 = 0xa0;
            f->u2 = ((unsigned char *)o)[0xc6] << 7;
            f->v2 = q[11] + 0xa0;
            f->u3 = q[10] + (((unsigned char *)o)[0xc6] << 7);
            f->v3 = q[11] + 0xa0;
            f->tpage = o->w1e;
            f->clut = o->w08;
            if (o->b0b != 0)
                insertPrim(f, SPAD.i1e0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            else
                insertPrimWithBias(f, SPAD.i1e0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            D_1F800164 = (PFT4_80123FD0 *)((char *)D_1F800164 + 0x28);
        }
        q += 16;
    } while (--n);
}
#endif

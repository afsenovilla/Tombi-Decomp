// FUNC 8011ebc0 1872 X003
// MATCHING 8011ebc0 1872
/* X003 twin of x014/func_8011C00C (rotated sprite, no semitrans): no anim check, byte scale `s = (unsigned char)sc` (no ba7),
   C/D int (`(C + u)`, `(D + w)` operand order), code 0x2d/0x2f via `sc = 0x2d; f->code = sc;` (li v1), no rgb copy,
   OT call result ignored. */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; int uv0;
    short x1, y1; int uv1;
    short x2, y2; unsigned short uv2, pad2;
    short x3, y3; unsigned short uv3, pad3;
} PFT4_8011C00C;

extern long LZ_FILE_CTRL;
extern long D_1F800074;
extern int D_1F800070i;
extern int D_1F800074i;
extern int D_1F800078;
extern PFT4_8011C00C *D_1F800164;
extern int D_1F8001E0;
extern short D_8007A5F0[];
extern short D_8007A3F0[];
int FUN_8004fba8(void *o, long *sxy, long *z);
int FUN_8004fcc0(void *p);
int insertPrim(void *a, int b, int c, int d, unsigned e);
int insertPrimWithBias(void *a, int b, int c, int d, unsigned e);

#define setSemiTrans(p, abe) ((abe) ? ((p)->code |= 2) : ((p)->code &= ~2))
#define SN(i) D_8007A5F0[i]
#define a1 ((ang + 0x80) & 0xff)
#define a0 (ang & 0xff)
#define CS(i) D_8007A3F0[i]

void func_8011EBC0(TObj *o)
{
    short *e;
    unsigned char *q;
    int xy;
    short x;
    unsigned int y;
    unsigned int s;
    int d;
    int ang;
    short A, B;
    int C, D;
    short u, w;
    int c;
    unsigned char cv;
    short sc;
    PFT4_8011C00C *f;

    FUN_8004fba8(o, &LZ_FILE_CTRL, &D_1F800074);
    xy = D_1F800070i;
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
            s = (unsigned char)sc;
            A = (((signed char *)q)[14] * s) >> 8;
            C = (q[10] * s) >> 8;
        } else {
            A = (signed char)q[14];
            C = q[10];
        }
        c = (signed char)o->ba6;
        sc = (signed char)c;
        if (sc != 0) {
            s = (unsigned char)sc;
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
            f->x1 = x + (((C + u) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y1 = y + (((C + u) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x2 = x + ((-u * SN(a1)) >> 12) + (((D + w) * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y2 = y + ((-u * CS(a1)) >> 12) + (((D + w) * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x3 = x + (((C + u) * SN(a0)) >> 12) + (((D + w) * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y3 = y + (((C + u) * CS(a0)) >> 12) + (((D + w) * CS((a0 + 0xc0) & 0xff)) >> 12);
        } else {
            u = -(A + C);
            w = B;
            f->x1 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y1 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x0 = x + (((C + u) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y0 = y + (((C + u) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x3 = x + ((-u * SN(a1)) >> 12) + (((D + w) * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y3 = y + ((-u * CS(a1)) >> 12) + (((D + w) * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x2 = x + (((C + u) * SN(a0)) >> 12) + (((D + w) * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y2 = y + (((C + u) * CS(a0)) >> 12) + (((D + w) * CS((a0 + 0xc0) & 0xff)) >> 12);
        }
        if (FUN_8004fcc0(f)) {
            sc = 0x2d;
            f->code = sc;
            if (o->b0d >> 7) f->code = 0x2f;
            f->uv0 = *(int *)q;
            f->uv1 = *(int *)(q + 4);
            f->uv2 = *(unsigned short *)(q + 8);
            f->uv3 = *(unsigned short *)(q + 12);
            *(unsigned short *)((char *)f + 0x16) += o->w1e;
            if (o->b0d & 1) *(unsigned short *)((char *)f + 0xe) = o->w08;
            if (o->b0b != 0) {
                insertPrim(f, D_1F8001E0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            } else {
                insertPrimWithBias(f, D_1F8001E0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            }
            D_1F800164 = (PFT4_8011C00C *)((char *)D_1F800164 + 0x28);
        }
    next:
        q += 16;
    } while (--D_1F800078 != 0);
}

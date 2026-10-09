// FUNC 801198f0 3660 X006
/* score 6: only case 2 differs: game loads d34 after the lh of y and leaves a dead copy of y (lh v0; lw a0; move v1,v0); ours hoists the lw d34 earlier, no copy. Tried y/d/lim/r temps of each type, inline clamp with param copy, assign-in-compare, ternaries. Real start 801198F0 (3660 B) covers csv piece 8011A448. Siblings: the 8-byte unused E local fixes the 0x38 frame, and the d34 ternary puts the div result in v0 (both open in wip/func_80119120 and func_80118C6C). Found (o23): the game's lh/lw/move order comes out exactly when d is still live after case 2 (a test read `o->w74 = d;` at the end of the function gives lh v0; lw a0; move; slt), so the source reads that short variable later on some path without emitting code; not found where. */
#include "TOBJ.H"

typedef struct {
    short a;     /* 0xb4 */
    short b;     /* 0xb6 */
    short ang;   /* 0xb8 */
    short ba;    /* 0xba */
    int rad;     /* 0xbc */
    int div;     /* 0xc0 */
    int c4;      /* 0xc4 */
    short c8;    /* 0xc8 */
    short ca;    /* 0xca */
    short cc;    /* 0xcc */
    short ce;    /* 0xce */
    short d0;    /* 0xd0 */
    short d2;    /* 0xd2 */
} Sub;
typedef struct { short x, y, z, w; } E;

extern TObj *D_8009C950;
extern int D_800A4550[];
extern short D_800A457E;
extern volatile unsigned short D_8009D670[];
extern char D_8011FEA0[], D_8011F3F8[];
extern E D_8011F3F8e[];
extern unsigned short D_8011F3E8[];
extern void *D_801229A8[], *D_801229CC[], *D_801229E0[];
extern short D_1F8000E2, D_1F8000E6, D_1F8000EA, D_1F8000EE[];
extern int D_1F8000EC[], D_1F800190, D_1F8000F4;
extern short D_1F80016A[], D_1F80016E[], D_1F800172;
extern unsigned short D_1F8001FC, D_1F8003C6;
extern unsigned char D_8009C964, D_8009C93A[], D_8009C93E, D_8009C93F[], D_8009C942;
extern short D_8009CFE2;
extern int D_8009C948;
extern int Rand(void);
extern int rsin(int);
extern int rcos(int);
extern void AnimLoadDuration(TObj *);
extern void FUN_80025f40(int, int, int, int);
extern void func_801183F0(TObj *);
extern void func_801184F0(TObj *);
extern void func_80118810(TObj *);
extern void func_8011866C(TObj *);
extern void func_80119120(TObj *);
extern void func_801196FC(TObj *);
extern void func_80118C6C(TObj *);

#define SUB(o) ((Sub *)((char *)(o) + 0xb4))

static __inline__ void upd(TObj *o, Sub *t)
{
    switch (SUB(o)->c8) {
    case 0:
        if (o->animTimer < 0x40) {
            if (o->animTimer >= 0x3e && (D_8009D670[0] & 0x10)) {
                *(char **)&o->wa8 = D_8011FEA0;
                SUB(o)->c8 = 2;
                o->animTimer -= 0x3e;
            }
        } else {
            SUB(o)->c8 = 1;
        }
        break;
    case 1:
        if ((unsigned short)(o->animTimer - 0x72) < 0x12) SUB(o)->cc = 1;
        else SUB(o)->cc = 0;
        break;
    case 2:
        if ((unsigned short)(o->animTimer - 0x35) < 0x19) SUB(o)->cc = 1;
        else SUB(o)->cc = 0;
        if (o->animTimer == 0x77) {
            *(char **)&o->wa8 = D_8011F3F8;
            o->animTimer = 0xaa;
            t->c8 = 1;
        }
        break;
    }
}

static __inline__ void pos(TObj *o)
{
    Sub *s;
    E *e;
    int v;

    func_80118810(o);
    upd(o, SUB(o));
    func_801184F0(o);
    s = SUB(o);
    e = *(E **)&o->wa8;
    e += o->animTimer;
    o->d34 = (SUB(o)->div > 0 ? (o->d34 = SUB(o)->rad * o->w76 / SUB(o)->div) + e->y : e->y) - 0x20;
    o->d38 = (rsin(s->ang) * s->rad) >> 20;
    o->d30 = (rcos(s->ang) * s->rad) >> 20;
    o->a.p.whole = e->x + o->d30;
    o->b.p.whole = e->z + o->d38;
}

static __inline__ int turn(int a, unsigned short target)
{
    unsigned short u = (target - a) & 0xfff;
    short d = u;
    short r;

    if (d == 0) return a & 0xfff;
    if (u < 0x800) {
        if (d >= 0x28) r = a + 0x28;
        else if (d >= 0x18) r = a + 0x18;
        else { r = a; r += u; }
    } else {
        if (d < 0xfd9) r = a - 0x28;
        else if (d < 0xfe9) r = a - 0x18;
        else { r = a; r += u; }
    }
    return r & 0xfff;
}

static __inline__ void turn4(int *p, int target)
{
    int a = p[9];
    unsigned short u = (target - a) & 0xfff;
    short d = u;

    if (d != 0) {
        if (u < 0x800) {
            if (d < 4) p[9] = a + d;
            else p[9] = a + 4;
        } else {
            if (d < 0xffd) p[9] = a - 4;
            else p[9] = a + d;
        }
        p[9] &= 0xfff;
    }
    p[9] &= 0xfff;
}

void func_801198F0(TObj *o)
{
    int *g = D_800A4550;
    TObj *p = D_8009C950;
    Sub *t = SUB(o);
    E *e;
    unsigned short y;
    short lim;
    int r;
    int a;
    unsigned short u;
    short d;
    E tmp;

    switch (o->step) {
    case 0:
        o->b9c = 0;
        o->state = 0;
        o->step++;
        D_1F8000E6 = -0x1e;
        D_1F8000E2 = 0x50;
        g[8] = 0x80;
        g[31] = -0xf00;
        D_1F8000EA = -0x2a8;
        o->b69 = 1;
        t->b = 0;
        t->a = 0;
        t->rad = 0;
        t->c4 = 0;
        t->c8 = 0;
        o->animTimer = 0;
        o->velH = 0;
        *(char **)&o->wa8 = D_8011F3F8;
        o->a.p.whole = D_8011F3F8e[o->animTimer].x;
        o->y.p.whole = D_8011F3F8e[o->animTimer].y - 0x20;
        o->b.p.whole = D_8011F3F8e[o->animTimer].z;
        D_1F80016A[0] = o->a.p.whole;
        D_1F80016E[0] = o->y.p.whole;
        D_1F800172 = o->b.p.whole;
        D_8009C964 = 0;
        func_801183F0(o);
        t->d0 = 0x80;
        t->ce = 0x1e;
        t->d2 = 0;
        t->cc = 0;
        t->ca = 0;
        D_8009CFE2 = 0;
        t->c4 += t->div;
        D_8009C93A[0] = 1;
        p->anim = D_801229A8[0];
        AnimLoadDuration(p);
        o->w7a = 0;
        break;
    case 1:
        pos(o);
        func_8011866C(o);
        if (o->b69) {
            if (D_8009C93E == 0 && t->cc == 0 && (D_1F8003C6 & D_1F8001FC)) {
                short vv = -0x300;
                p->anim = D_801229CC[0];
                AnimLoadDuration(p);
                o->b69 = 0;
                o->velV = vv;
                o->step++;
                o->velV = vv - (t->b >> 1);
                o->b9c = 1;
            }
            o->velX = t->b >> 1;
        } else {
            o->velX -= 0x10;
            if (o->velX < 0) {
                o->velX = 0;
            } else if (o->velV > 0x200) {
                o->step = 3;
                o->b9c = 2;
                p->anim = D_801229E0[0];
                AnimLoadDuration(p);
            }
        }
        break;
    case 2:
        pos(o);
        o->velX -= 0x10;
        if (o->velX < 0) o->velX = 0;
        o->velV += 0x40;
        if (o->velV > 0) {
            o->velV = 0;
            o->b9c = 2;
            o->step++;
        }
        o->y.raw += o->velV << 8;
        d = o->y.p.whole;
        r = o->d34;
        if (d >= r) o->y.p.whole = r;
        break;
    case 3:
        pos(o);
        o->velX -= 0x10;
        if (o->velX < 0) o->velX = 0;
        o->velV += 0x40;
        if (o->velV > 0x800) o->velV = 0x800;
        o->y.raw += o->velV << 8;
        y = o->y.p.whole;
        switch (o->w7a & 3) {
        case 0:
            o->w74 = 0;
            break;
        case 1:
            o->w74 = Rand() & 3;
            if (SUB(o)->b == 0) o->w74 = 2;
            break;
        case 2:
            o->w74 = (Rand() & 3) + 2;
            if (SUB(o)->b == 0) o->w74 = 4;
            break;
        case 3:
            o->w74 = 4;
            break;
        }
        lim = o->d34 + (unsigned short)o->w74;
        if ((short)lim <= (short)y) {
            r = 1;
            o->b69 = 1;
            o->y.p.whole += lim - y;
        } else {
            r = 0;
            o->b69 = 0;
        }
        if (r) {
            FUN_80025f40(0, 0, 0xe0, 5);
            p->anim = D_801229A8[0];
            AnimLoadDuration(p);
            o->b9c = 0;
            func_801196FC(o);
        }
        break;
    case 4:
        func_80119120(o);
        break;
    case 5:
        func_80118C6C(o);
        break;
    }
    if (t->ca != 0 && o->animTimer < 0xc1) {
        D_8009CFE2 = ++t->d2;
        if (t->d0 == 0 || t->d2 >= 0x2a31) {
            t->b -= 0xe;
            if (t->b < 0) {
                t->b = 0;
                D_8009C93F[0] = 1;
                D_8009C942 = 1;
                o->b04 = 2;
                o->step = 0;
            }
        } else if (--t->ce == 0) {
            d = 0x500 - t->b;
            d = d < 0 ? 0 : d >> 8;
            t->ce = D_8011F3E8[d];
            if (--t->d0 <= 0) t->d0 = 0;
        }
    }
    o->velH += o->velX;
    if (D_1F8000EE[0] < D_800A457E) {
        D_1F800190 = o->y.raw;
        D_1F8000EC[0] = o->a.raw;
        D_1F8000F4 = o->b.raw;
    }
    if (o->b69) o->d8c = turn(o->d8c, (unsigned short)t->ba);
    o->d88 = turn(o->d88, 0x1000 - (unsigned short)t->ang);
    if (o->animTimer < 0xc1) {
        D_8009C948 = t->c4 >> 8;
        turn4(g, (0x1000 - o->d88) & 0xfff);
    }
}

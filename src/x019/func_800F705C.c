// FUNC 800f705c 2524 X019
// MATCHING 800f705c 2524
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char b1;
    short w2;
    char p4[4];
    unsigned char b8;
    unsigned char b9;
    char pa[2];
    short wc;
    short we;
    char p10[0x28 - 0x10];
    unsigned short w28, w2a;
    unsigned short w2c, w2e;
} PL705C;

extern PL705C *D_8009C330;
extern PL705C *D_8009C330A[];
extern unsigned char *D_8009D2E8;
extern unsigned char *D_800A611C;
extern TObj *D_8009F0EC;
extern int D_8009C934;
extern char D_80011108[];
extern unsigned char D_80115228[][2];
extern unsigned char DAT_801152e8[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001e5f4(int, int);
extern int FUN_8001fe3c(int, int);
extern int FUN_8001fe0c(int, int);
extern short FUN_8003fd78(TObj *, int, int);
extern short FUN_800f6e28(TObj *);
extern void FUN_800f6f38(TObj *, int);
extern void FUN_800f6c34(TObj *);
extern void FUN_800ef1ac(TObj *);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    PL705C *p = D_8009C330;
    p->w2c = anim;
    if (p->w2e != anim) {
        p->w2c = anim;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0);
        D_8009C330->w2e = D_8009C330->w2c;
    }
}

static __inline__ void rot(TObj *o)
{
    PL705C *p = D_8009C330;
    o->wb6 += p->we;
    if ((unsigned short)o->wb6 < 0x800) p->b8 = 1;
    if ((unsigned)((unsigned short)o->wb6 - 0x800) < 0x800) D_8009C330->b8 = 0;
    if ((unsigned short)(o->wb6 + 0x7ff) < 0x800) D_8009C330->b8 = 0;
    if ((unsigned short)(o->wb6 + 0xfff) < 0x800) D_8009C330->b8 = 1;
}

static __inline__ void swing(TObj *o, int p)
{
    int s;
    int ang;
    short r;
    int c = p & 0xff;
    if (o->animFrame & 1) {
        ang = 0x1bf; ang -= p;
        s = FUN_8001fe3c(ang & 0xff, D_8009C330->b1);
        o->h->p.whole = s + (o->wb8 + o->d30);
        s = FUN_8001fe0c(ang & 0xff, D_8009C330->b1);
        o->d84 = 0;
        o->d8c = 0x100 - c;
        r = s + (o->wba + o->d34);
    } else {
        ang = p + 0xc0;
        s = FUN_8001fe3c(ang & 0xff, D_8009C330->b1);
        o->h->p.whole = s + (o->wb8 + o->d30);
        s = FUN_8001fe0c(ang & 0xff, D_8009C330->b1);
        o->d84 = 0;
        o->d8c = c;
        r = s + (o->wba + o->d34);
    }
    o->y.p.whole = r;
}

#define ADDWE() { \
    PL705C *p = D_8009C330A[0]; \
    int w = p->we; int v; \
    if (p->b8) v = w - p->w2; else v = w + p->w2; p->we = v; }

#define CATCH() { \
    int d = 0x10; \
    D_8009C330->b9 = 0; \
    U8(o, 0xac) = 1; \
    o->b9e = 0; \
    o->wb2 = 0; \
    o->velH = 0; \
    o->velV = 0; \
    o->d30 = 0; \
    o->d34 = 0; \
    o->velX = 0; \
    o->velY = 0; \
    o->timer = 10; \
    o->d84 = 0; \
    if (o->animFrame & 1) d = 0xf0; \
    o->d88 = d; \
    o->d8c = DAT_801152e8[o->wb0]; \
    { Fix16 *h = o->h; int pv = h->p.whole; short x; \
      if (o->animFrame & 1) x = pv + 14; else x = pv - 14; \
      h->p.whole = x; } \
    D_8009F0EC->b69 = 0; \
    if (r) { o->step = 2; o->state = 3; o->substep = 0; } \
    else { o->step = 0; o->state = 0; o->substep = 0; } }

void func_800F705C(TObj *o)
{
    PL705C *q;
    short r;
    int t;
    int t1;
    char pad[4];

    switch (o->state) {
    case 0:
        if (U8(o, 0xac) >= 2) {
            D_8009D2E8 = D_800A611C;
            D_8009D2E8[4] = 2;
            D_8009D2E8[5] = 2;
            D_8009D2E8[6] = 0;
        }
        U8(o, 0xac) = 0;
        D_8009C934 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        D_8009C330->b0 = 0;
        D_8009F0EC->b69 = 1;
        {
            PL705C *q = D_8009C330;
            o->wb6 = -0x420;
            q->w2 = 0x10;
            q->we = 0;
            o->wb2 = 2;
            q->b8 = 0;
        }
        D_8009C330->b9 = 0;
        D_8009C330->wc = 0;
        D_8009C330->w28 = 0xffff;
        D_8009C330->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d30 = D_8009F0EC->h->p.whole;
        o->d34 = D_8009F0EC->y.p.whole;
        U8(o, 0xac) = 0;
        if (o->animFrame & 1) {
            o->d8c = 0x100;
            o->wb8 -= 6;
        } else {
            o->d8c = 0;
            o->wb8 += 6;
        }
        o->wba -= 0x10;
        FUN_8001e5f4(4, 0x7f);
        o->state++;
    case 1:
        rot(o);
        if (o->wb6 == 0) {
            q = D_8009C330;
            if (q->we > 0) {
                unsigned char *b = D_80115228[--o->wb2];
                q->we = b[0];
                q->w2 = b[1];
            }
        }
        t1 = (short)o->wb6 >> 4;
        D_8009C330->w2e = 0xffff;
        o->anim = D_80011108;
        FUN_8001fe94(o, 0);
        D_8009C330->b1 = 0x18;
        r = FUN_800f6e28(o);
        FUN_800f6f38(o, t1);
        ADDWE();
        if (o->wb2 < 2) o->state++;
        if (o->b69) CATCH();
        break;
    case 2:
        rot(o);
        if (o->wb6 == 0) {
            q = D_8009C330;
            if (q->we > 0) {
                unsigned char *b = D_80115228[--o->wb2];
                q->we = b[0];
                q->w2 = b[1];
            } else {
                q->b9 = 1;
            }
        }
        setanim(o, 6);
        FUN_8001fec0(o);
        t = (short)o->wb6 >> 4;
        r = FUN_800f6e28(o);
        FUN_800f6f38(o, t);
        swing(o, t);
        ADDWE();
        if (o->wb2 == 0) {
            o->d8c = 0;
            o->state++;
        }
        if (D_8009C330->b9 == 0) break;
        if (o->b69 || FUN_8003fd78(o, 0, 0)) CATCH();
        FUN_800f6c34(o);
        break;
    case 3:
        r = FUN_800f6e28(o);
        o->h->p.whole = FUN_8001fe3c(0xc0, D_8009C330->b1) + (o->wb8 + o->d30);
        o->y.p.whole = FUN_8001fe0c(0xc0, D_8009C330->b1) + (o->wba + o->d34);
        FUN_8001fec0(o);
        if (o->b69) CATCH();
        FUN_800f6c34(o);
        break;
    }
    FUN_800ef1ac(o);
}

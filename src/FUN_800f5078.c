// FUNC 800f5078 5740 X000
// MATCHING 800f5078 5740
/* note: ((unsigned)(t << 0) >> 2) keeps the srl the game has; (unsigned)t >> 2 gives sra with short t. */
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char b1;
    short w2;
    char p4;
    signed char b5;
    char p6[2];
    unsigned char b8;
    unsigned char b9;
    unsigned char ba;
    char pb;
    short wc;
    short we;
    char p10[0x28 - 0x10];
    unsigned short w28, w2a;
    unsigned short w2c, w2e;
} PL;

extern PL *D_8009C330;
extern TObj *D_8009F0EC;
extern unsigned short D_8009d670;
extern unsigned short D_1f8001fc, D_1f8003c6;
extern unsigned char D_8009d2b0;
extern char D_80011108[];
extern unsigned char D_80115228[][2];
extern unsigned char D_80115388[];
extern void FUN_80025f40(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void FUN_8001e5f4(int, int);
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_800f4ed0(TObj *, short, short);
extern void FUN_8010a168(TObj *);
extern void FUN_8010a518(TObj *);

#define PAD (*(volatile unsigned short *)&D_8009d670)

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    PL *p = D_8009C330;
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
    PL *p = D_8009C330;
    o->wb6 += p->we;
    if ((unsigned short)o->wb6 < 0x800) p->b8 = 1;
    if ((unsigned)((unsigned short)o->wb6 - 0x800) < 0x800) D_8009C330->b8 = 0;
    if ((unsigned short)(o->wb6 + 0x7ff) < 0x800) D_8009C330->b8 = 0;
    if ((unsigned short)(o->wb6 + 0xfff) < 0x800) D_8009C330->b8 = 1;
}

static __inline__ void setspd(TObj *o)
{
    unsigned char *t = D_80115228[o->wb2];
    D_8009C330->we = t[0];
    D_8009C330->w2 = t[1];
}

static __inline__ void attach(TObj *o)
{
    TObj *f = D_8009F0EC;
    switch (f->type) {
    case 14:
        o->d30 = f->d30 - 4 + (o->animFrame & 1) * 6;
        o->d34 = f->d34 + 6;
        break;
    case 66:
        o->d30 = f->h->p.whole;
        o->d34 = f->y.p.whole;
        break;
    }
}

#define SND(n) { \
    FUN_80025f40(0, 0, 0xff, 2); \
    FUN_8001e560(3, n); \
    D_8009C330->ba = D_8009F0EC->b0c; }

#define INCA(n) { short r = o->wb2; int m; if (r < n) m = r + 1; else m = r; o->wb2 = m; }
#define INCB(n) { short r = o->wb2; int m; if (r >= n) m = r; else m = r + 1; o->wb2 = m; }
#define DEC2() { int q = o->wb2; if (q > 0) q--; o->wb2 = q; }
#define INCA2(n) { r = o->wb2; if (r < n) m = r + 1; else m = r; o->wb2 = m; }
#define INCB2(n) { r = o->wb2; if (r >= n) m = r; else m = r + 1; o->wb2 = m; }
#define DEC() { int q = o->wb2; if (q > 0) q--; o->wb2 = q; }

void FUN_800f5078(TObj *o)
{
    short t;
    short idx;
    short i2;
    short f;
    unsigned short v;
    short r;
    int m;

    D_8009C330->ba = 0xff;
    switch (o->state) {
    case 0:
        o->wb6 = 0;
        {
            signed char c = D_8009C330->b5;
            U8(o, 0xac) = 2;
            o->wb2 = c + 1;
        }
        D_8009C330->b5 = 0;
        D_8009C330->ba = D_8009F0EC->b0c;
        D_8009C330->w2 = 0;
        D_8009C330->we = 0;
        o->ba4 = 0;
        D_8009C330->b8 = 0;
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
        FUN_8001e5f4(4, 0x7f);
        setspd(o);
        o->state = 1;
    case 1:
        rot(o);
        if (o->wb6 == 0) {
            if (o->animFrame & 1) {
                if (D_8009C330->we > 0) {
                    D_8009C330->ba = D_8009F0EC->b0c;
                    if (PAD & 0x80) INCB(9);
                    if (PAD & 0x20) {
                        o->animFrame = 0;
                        D_8009F0EC->state = 0;
                    }
                    setspd(o);
                }
            } else if (D_8009C330->we > 0) {
                D_8009C330->ba = D_8009F0EC->b0c;
                if (PAD & 0x20) INCB(9);
                if (PAD & 0x80) {
                    o->animFrame = 1;
                    D_8009F0EC->state = 0;
                }
                setspd(o);
            }
        }
        setanim(o, 6);
        FUN_8001fec0(o);
        D_8009C330->b1 = 0x20;
        t = (short)o->wb6 >> 4;
        idx = ((unsigned)(t << 0) >> 2) & 0x3f;
        attach(o);
        FUN_800f4ed0(o, idx, t);
        if (D_1f8003c6 & D_1f8001fc) {
            D_8009d2b0 = 0;
            o->b9c = 1;
            U8(o, 0xac) = 0;
            FUN_8010a168(o);
            {
                Fix16 *h = o->h;
                int pv = h->p.whole;
                short x;
                if (o->animFrame & 1) x = pv + 16; else x = pv - 16;
                h->p.whole = x;
            }
            S8(o, 0xf) = -8;
            o->wb0 = 0;
            D_8009C330->b1 = 0;
            o->timer = 6;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb0 = 0;
            o->b9e = 0;
            o->step = 9;
            o->state = 0;
        } else if (o->wb2 >= 2) {
            o->state = 2;
        }
        break;
    case 2:
        rot(o);
        if (o->wb6 == 0) {
            if (o->animFrame & 1) {
                if (D_8009C330->we > 0) {
                    SND(0);
                    if (PAD & 0x80) INCA2(9);
                    v = PAD & 0x20;
                    goto d1;
                }
                SND(0);
                if (PAD & 0x80) INCA2(3);
                v = PAD & 0x20;
                goto d2;
            } else {
                if (D_8009C330->we > 0) {
                    SND(0);
                    if (PAD & 0x20) INCB2(9);
                    v = PAD & 0x80;
                d1:
                    if (v) DEC2();
                    setspd(o);
                    goto c2;
                }
                SND(0);
                if (PAD & 0x20) INCB2(3);
                v = PAD & 0x80;
            d2:
                if (v) DEC2();
                setspd(o);
                D_8009C330->we = -D_8009C330->we;
            }
        }
    c2:
        t = (short)o->wb6 >> 4;
        D_8009C330->w2e = 0xffff;
        o->anim = D_80011108;
        idx = ((unsigned)(t << 0) >> 2) & 0x3f;
        FUN_8001fe94(o, D_80115388[idx]);
        D_8009C330->b1 = 0x20;
        attach(o);
        FUN_800f4ed0(o, idx, t);
        if (D_1f8003c6 & D_1f8001fc) {
            D_8009d2b0 = 0;
            FUN_8010a518(o);
            if (o->d88 < o->wb6)
                D_8009C330->b9 = 1;
            else
                D_8009C330->b9 = 0;
            o->state = 3;
            break;
        }
        if (o->wb2 >= 4) o->state = 4;
        if (o->wb2 < 2) o->state = 1;
        break;
    case 3:
        rot(o);
        if (o->wb6 == 0) {
            if (o->animFrame & 1) {
                if (D_8009C330->we > 0) goto x3;
            } else if (D_8009C330->we > 0) {
            x3:
                SND(2);
                D_8009C330->b9 = 0;
                setspd(o);
                goto c3;
            }
            SND(2);
        }
    c3:
        t = (short)o->wb6 >> 4;
        D_8009C330->w2e = 0xffff;
        o->anim = D_80011108;
        idx = ((unsigned)(t << 0) >> 2) & 0x3f;
        FUN_8001fe94(o, D_80115388[idx]);
        attach(o);
        FUN_800f4ed0(o, idx, t);
        if (D_8009C330->b9 != 0) break;
        if (o->d88 >= o->wb6) break;
        U8(o, 0xac) = 0;
        S8(o, 0xf) = -8;
        o->wb0 = 0;
        D_8009C330->b1 = 0;
        o->timer = 6;
        o->b9e = 4;
        o->d30 = 0;
        o->d34 = 0;
        o->velX = 0;
        o->velY = 0;
        o->wb0 = 0;
        o->step = 9;
        o->state = 0;
        break;
    case 4:
        {
            PL *p = D_8009C330;
            p->b9 = 0;
            p->w2e = 0xffff;
        }
        D_8009C330->ba = 0;
        f = 0;
        rot(o);
        if (o->wb6 < -4094) {
            setspd(o);
            f = 1;
        }
        if (o->wb6 > 4094) {
            setspd(o);
            f = 1;
        }
        if (f) {
            o->wb6 = 0;
            if (o->animFrame & 1) {
                SND(2);
                if (PAD & 0x80) INCA(9);
                v = PAD & 0x20;
            } else {
                SND(2);
                if (PAD & 0x20) INCB(9);
                v = PAD & 0x80;
            }
            if (v) DEC();
            if ((D_1f8003c6 & D_1f8001fc) && o->wb2 < 4) o->wb2 = 4;
            setspd(o);
        }
        t = (short)o->wb6 >> 4;
        idx = ((unsigned)(t << 0) >> 2) & 0x3f;
        attach(o);
        FUN_800f4ed0(o, idx, t);
        i2 = idx;
        o->anim = D_80011108;
        if (o->animFrame & 1) {
            FUN_8001fe94(o, D_80115388[i2]);
            o->d8c = 0x100 - i2 * 4;
        } else {
            FUN_8001fe94(o, D_80115388[i2]);
            o->d8c = i2 * 4;
        }
        if (D_1f8003c6 & D_1f8001fc) {
            FUN_8010a168(o);
            if (o->d88 < o->wb6)
                D_8009C330->b9 = 1;
            else
                D_8009C330->b9 = 0;
            o->state = 5;
            break;
        }
        if (o->wb2 < 4) o->state = 2;
        break;
    case 5:
        D_8009C330->w2e = 0xffff;
        f = 0;
        rot(o);
        if (o->wb6 < -4095) {
            setspd(o);
            f = 1;
        }
        if (o->wb6 > 4095) {
            setspd(o);
            f = 1;
        }
        if (f) {
            o->wb6 = 0;
            if (D_8009C330->we > 0) {
                SND(2);
                D_8009C330->b9 = 0;
                setspd(o);
            } else {
                SND(2);
            }
        }
        t = (short)o->wb6 >> 4;
        idx = ((unsigned)(t << 0) >> 2) & 0x3f;
        attach(o);
        FUN_800f4ed0(o, idx, t);
        i2 = idx;
        o->anim = D_80011108;
        if (o->animFrame & 1) {
            FUN_8001fe94(o, D_80115388[i2]);
            o->d8c = 0x100 - i2 * 4;
        } else {
            FUN_8001fe94(o, D_80115388[i2]);
            o->d8c = i2 * 4;
        }
        if (D_8009C330->b9 == 0 && o->d88 < o->wb6) {
            o->wb6 = o->d88;
            FUN_800f4ed0(o, idx, (short)o->wb6 >> 4);
            {
                PL *p = D_8009C330;
                p->b8 = 0;
                p->we = 0;
            }
            D_8009C330->w2 = 0;
            U8(o, 0xac) = 0;
            S8(o, 0xf) = -8;
            o->wb0 = 0;
            D_8009C330->b1 = 0;
            o->timer = 6;
            o->b9e = 4;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb0 = 0;
            o->step = 9;
            o->state = 0;
        }
        break;
    }
}

// FUNC 800f5078 5740 X017
// MATCHING 800f5078 5740
#include "TOBJ.H"
#include "raw7.h"
typedef struct { unsigned char b0; unsigned char b1; short w2; char p4[8]; short wc; short we; char p10[0x28 - 0x10]; unsigned short w28, w2a; } PL;

extern TObj *D_8009C330;
extern TObj *D_8009F0EC;
extern volatile unsigned short D_8009D670[];
extern unsigned short D_1f8001fc, D_1f8003c6;
extern unsigned char D_8009D2B0;
extern unsigned char D_80115228[][2];
extern unsigned char D_80115388[];
extern char D_80011108[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001e5f4(int, int);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void FUN_800f4ed0(TObj *, int, short);
extern void FUN_8010a168(TObj *);
extern void FUN_8010a518(TObj *);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = D_8009C330;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0);
        D_8009C330->animFrame = D_8009C330->animTimer;
    }
}

static __inline__ void rot(TObj *o)
{
    PL *p = (PL *)D_8009C330;
    o->wb6 += p->we;
    if ((unsigned short)o->wb6 < 0x800) U8(p, 8) = 1;
    if ((unsigned)((unsigned short)o->wb6 - 0x800) < 0x800) U8(D_8009C330, 8) = 0;
    if ((unsigned short)(o->wb6 + 0x7ff) < 0x800) U8(D_8009C330, 8) = 0;
    if ((unsigned short)(o->wb6 + 0xfff) < 0x800) U8(D_8009C330, 8) = 1;
}

#define SETTBL(o) \
    { \
        unsigned char *b = D_80115228[o->wb2]; \
        PL *q = (PL *)D_8009C330; \
        q->we = b[0]; \
        q->w2 = b[1]; \
    }

#define SND(k) \
    FUN_80025f40(0, 0, 0xff, 2); \
    FUN_8001e560(3, k); \
    U8(D_8009C330, 10) = U8(D_8009F0EC, 12);

#define INC1(o, n) \
    { \
        int w = o->wb2; \
        int x; \
        if (w >= n) x = w; \
        else x = w + 1; \
        o->wb2 = x; \
    }
#define INC2(o, n) \
    { \
        int w = o->wb2; \
        short x; \
        if (w < n) x = w + 1; \
        else x = w; \
        o->wb2 = x; \
    }
#define DEC(o) \
    { \
        int w = o->wb2; \
        if (w > 0) w--; \
        o->wb2 = w; \
    }

#define QBLK(o) \
    { \
        TObj *q = D_8009F0EC; \
        switch (q->type) { \
        case 14: \
            { int t = (o->animFrame & 1) * 6 - 4; o->d30 = q->d30 + t; } \
            o->d34 = q->d34 + 6; \
            break; \
        case 66: \
            o->d30 = q->h->p.whole; \
            o->d34 = q->y.p.whole; \
            break; \
        } \
    }

static __inline__ void swing(TObj *o, short i, short a)
{
    FUN_800f4ed0(o, i, a);
    o->anim = D_80011108;
    if (o->animFrame & 1) {
        FUN_8001fe94(o, D_80115388[i]);
        o->d8c = 0x100 - (i << 2);
    } else {
        FUN_8001fe94(o, D_80115388[i]);
        o->d8c = i << 2;
    }
}

void func_800F5078(TObj *o)
{
    PL *p;
    short a;
    short i;
    short f;

    U8(D_8009C330, 10) = 0xff;
    switch (o->state) {
    case 0:
        o->wb6 = 0;
        {
            signed char c = S8(D_8009C330, 5);
            U8(o, 0xac) = 2;
            o->wb2 = c + 1;
        }
        U8(D_8009C330, 5) = 0;
        U8(D_8009C330, 10) = U8(D_8009F0EC, 12);
        {
            PL *q = (PL *)D_8009C330;
            q->w2 = 0;
            q->we = 0;
        }
        o->ba4 = 0;
        U8(D_8009C330, 8) = 0;
        U8(D_8009C330, 9) = 0;
        {
            PL *q = (PL *)D_8009C330;
            q->wc = 0;
            q->w28 = 0xffff;
            q->w2a = 0xffff;
        }
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        FUN_8001e5f4(4, 0x7f);
        SETTBL(o);
        o->state = 1;
    case 1:
        rot(o);
        if (o->wb6 == 0) {
            if (o->animFrame & 1) {
                if (((PL *)D_8009C330)->we > 0) {
                    U8(D_8009C330, 10) = U8(D_8009F0EC, 12);
                    if (D_8009D670[0] & 0x80) INC1(o, 9);
                    if (D_8009D670[0] & 0x20) {
                        o->animFrame = 0;
                        D_8009F0EC->state = 0;
                    }
                    SETTBL(o);
                }
            } else {
                if (((PL *)D_8009C330)->we > 0) {
                    U8(D_8009C330, 10) = U8(D_8009F0EC, 12);
                    if (D_8009D670[0] & 0x20) INC1(o, 9);
                    if (D_8009D670[0] & 0x80) {
                        o->animFrame = 1;
                        D_8009F0EC->state = 0;
                    }
                    SETTBL(o);
                }
            }
        }
        setanim(o, 6);
        FUN_8001fec0(o);
        U8(D_8009C330, 1) = 0x20;
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        QBLK(o);
        FUN_800f4ed0(o, i, a);
        if (D_1f8001fc & D_1f8003c6) {
            D_8009D2B0 = 0;
            o->b9c = 1;
            U8(o, 0xac) = 0;
            FUN_8010a168(o);
            o->h->p.whole += (o->animFrame & 1) ? 16 : -16;
            S8(o, 0xf) = -8;
            o->wb0 = 0;
            U8(D_8009C330, 1) = 0;
            o->timer = 6;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb0 = 0;
            o->b9e = 0;
            o->step = 9;
            o->state = 0;
            break;
        }
        if (o->wb2 >= 2) o->state = 2;
        break;
    case 2:
        rot(o);
        if (o->wb6 == 0) {
            if (o->animFrame & 1) {
                if (((PL *)D_8009C330)->we > 0) {
                    SND(0);
                    if (D_8009D670[0] & 0x80) INC2(o, 9);
                    if (D_8009D670[0] & 0x20) DEC(o);
                    SETTBL(o);
                } else {
                    SND(0);
                    if (D_8009D670[0] & 0x80) INC2(o, 3);
                    if (D_8009D670[0] & 0x20) DEC(o);
                    SETTBL(o);
                    ((PL *)D_8009C330)->we = -((PL *)D_8009C330)->we;
                }
            } else {
                if (((PL *)D_8009C330)->we > 0) {
                    SND(0);
                    if (D_8009D670[0] & 0x20) INC1(o, 9);
                    if (D_8009D670[0] & 0x80) DEC(o);
                    SETTBL(o);
                } else {
                    SND(0);
                    if (D_8009D670[0] & 0x20) INC1(o, 3);
                    if (D_8009D670[0] & 0x80) DEC(o);
                    SETTBL(o);
                    ((PL *)D_8009C330)->we = -((PL *)D_8009C330)->we;
                }
            }
        }
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        D_8009C330->animFrame = 0xffff;
        o->anim = D_80011108;
        FUN_8001fe94(o, D_80115388[i]);
        U8(D_8009C330, 1) = 0x20;
        QBLK(o);
        FUN_800f4ed0(o, i, a);
        if (D_1f8001fc & D_1f8003c6) {
            D_8009D2B0 = 0;
            FUN_8010a518(o);
            if (o->wb6 > o->d88) U8(D_8009C330, 9) = 1;
            else U8(D_8009C330, 9) = 0;
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
                if (((PL *)D_8009C330)->we > 0) {
                    SND(2);
                    U8(D_8009C330, 9) = 0;
                    SETTBL(o);
                } else {
                    SND(2);
                }
            } else {
                if (((PL *)D_8009C330)->we > 0) {
                    SND(2);
                    U8(D_8009C330, 9) = 0;
                    SETTBL(o);
                } else {
                    SND(2);
                }
            }
        }
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        D_8009C330->animFrame = 0xffff;
        o->anim = D_80011108;
        FUN_8001fe94(o, D_80115388[i]);
        QBLK(o);
        FUN_800f4ed0(o, i, a);
        if (U8(D_8009C330, 9) != 0) break;
        if (o->wb6 > o->d88) {
            U8(o, 0xac) = 0;
            S8(o, 0xf) = -8;
            o->wb0 = 0;
            U8(D_8009C330, 1) = 0;
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
    case 4:
        {
            TObj *q = D_8009C330;
            U8(q, 9) = 0;
            q->animFrame = 0xffff;
            U8(D_8009C330, 10) = 0;
        }
        f = 0;
        rot(o);
        if (o->wb6 < -0xffe) {
            SETTBL(o);
            f = 1;
        }
        if (o->wb6 > 0xffe) {
            SETTBL(o);
            f = 1;
        }
        if (f) {
            o->wb6 = 0;
            if (o->animFrame & 1) {
                SND(2);
                if (D_8009D670[0] & 0x80) INC2(o, 9);
                if (D_8009D670[0] & 0x20) DEC(o);
            } else {
                SND(2);
                if (D_8009D670[0] & 0x20) INC1(o, 9);
                if (D_8009D670[0] & 0x80) DEC(o);
            }
            if ((D_1f8001fc & D_1f8003c6) && o->wb2 < 4) o->wb2 = 4;
            SETTBL(o);
        }
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        QBLK(o);
        swing(o, i, a);
        if (D_1f8001fc & D_1f8003c6) {
            FUN_8010a168(o);
            if (o->wb6 > o->d88) U8(D_8009C330, 9) = 1;
            else U8(D_8009C330, 9) = 0;
            o->state = 5;
            break;
        }
        if (o->wb2 < 4) o->state = 2;
        break;
    case 5:
        p = (PL *)D_8009C330;
        ((TObj *)p)->animFrame = 0xffff;
        f = 0;
        rot(o);
        if (o->wb6 < -0xfff) {
            SETTBL(o);
            f = 1;
        }
        if (o->wb6 > 0xfff) {
            SETTBL(o);
            f = 1;
        }
        if (f) {
            o->wb6 = 0;
            if (((PL *)D_8009C330)->we > 0) {
                SND(2);
                U8(D_8009C330, 9) = 0;
                SETTBL(o);
            } else {
                SND(2);
            }
        }
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        QBLK(o);
        swing(o, i, a);
        if (U8(D_8009C330, 9) != 0) break;
        if (o->wb6 > o->d88) {
            o->wb6 = o->d88;
            FUN_800f4ed0(o, i, o->wb6 >> 4);
            {
                PL *q = (PL *)D_8009C330;
                U8(q, 8) = 0;
                q->we = 0;
                ((PL *)D_8009C330)->w2 = 0;
            }
            U8(o, 0xac) = 0;
            S8(o, 0xf) = -8;
            o->wb0 = 0;
            U8(D_8009C330, 1) = 0;
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

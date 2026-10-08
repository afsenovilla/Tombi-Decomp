// FUNC 800f98a8 1908 X005
// MATCHING 800f98a8 1908
#include "TOBJ.H"
#include "raw7.h"
extern TObj *D_8009C330;
extern unsigned char *D_8009D2E8;
extern unsigned char *D_800A611C;
extern int D_8009C934;
extern char D_80011108[];
extern unsigned char D_80115220[][2];
extern volatile unsigned short D_8009D670[];
extern unsigned short D_1f8003c6;
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001e5f4(int, int);
extern void FUN_80041ca8(TObj *, int, int);
extern void FUN_800f9548(TObj *, int, int);
extern void FUN_800f9684(TObj *);
extern void FUN_800ef1ac(TObj *);

typedef struct { unsigned char b0; char p1; short w2; char p4[8]; short wc; short we; char p10[0x28 - 0x10]; unsigned short w28, w2a; } PL;

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

#define STEP(o) \
    o->h->p.whole += (o->animFrame & 1) ? -4 : 4; \
    FUN_80041ca8(o, (short)(o->h->p.whole + ((o->animFrame & 1) ? -8 : 8)), (short)(o->y.p.whole - 0x10));

void func_800F98A8(TObj *o)
{
    PL *p;
    PL *q;
    short u; /* never assigned: the game passes uninitialized $s1 to FUN_800f9548 in state 1 */
    int t;

    switch (o->state) {
    case 0:
        q = (PL *)D_8009C330;
        o->wb6 = -0x420;
        q->w2 = 0x10;
        q->we = 0;
        o->wb2 = 2;
        U8(q, 8) = 0;
        U8(D_8009C330, 9) = 0;
        ((PL *)D_8009C330)->wc = 0;
        ((PL *)D_8009C330)->w28 = 0xffff;
        ((PL *)D_8009C330)->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
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
        U8(D_8009C330, 0) = 0;
        o->anim = D_80011108;
        FUN_8001fe94(o, 0);
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole - 4;
        FUN_8001e5f4(4, 0x7f);
        o->state++;
    case 1:
        rot(o);
        if (o->wb6 == 0) {
            p = (PL *)D_8009C330;
            if (p->we > 0) {
                unsigned char *b = D_80115220[--o->wb2];
                p->we = b[0];
                p->w2 = b[1];
            }
        }
        t = (short)o->wb6 >> 4;
        D_8009C330->animFrame = 0xffff;
        U8(D_8009C330, 1) = 0x12;
        FUN_800f9548(o, u, t);
        if (o->wb2 < 2) o->state++;
        break;
    case 2:
        rot(o);
        if (o->wb6 == 0) {
            p = (PL *)D_8009C330;
            if (p->we > 0) {
                unsigned char *b = D_80115220[--o->wb2];
                p->we = b[0];
                p->w2 = b[1];
            } else {
                U8(p, 9) = 1;
            }
        }
        setanim(o, 7);
        FUN_8001fec0(o);
        {
            unsigned int v = (short)o->wb6 >> 4;
            FUN_800f9548(o, (v >> 2) & 0x3f, v);
        }
        if (o->wb2 == 0) {
            o->d8c = 0;
            o->h->p.whole = o->w74 + ((o->animFrame & 1) ? -4 : 4);
            o->y.p.whole = o->w76 + 0xc;
            o->state++;
        } else if (U8(D_8009C330, 9) != 0) {
            FUN_800f9548(o, 0, 0);
            if (D_8009D670[0] & D_1f8003c6) {
                U8(D_8009C330, 9) = 0;
                o->step = 0xb;
                o->state = 0;
            }
            if (D_8009D670[0] & 0x10) {
                U8(D_8009C330, 9) = 0;
                STEP(o);
                o->step = 0xb;
                o->state = 0;
            }
            if (D_8009D670[0] & 0x40) {
                STEP(o);
                FUN_800f9684(o);
            }
        }
        break;
    case 3:
        FUN_8001fec0(o);
        if (D_8009D670[0] & D_1f8003c6) {
            U8(D_8009C330, 9) = 0;
            o->step = 0xb;
            o->state = 0;
        }
        if (D_8009D670[0] & 0x10) {
            U8(D_8009C330, 9) = 0;
            o->step = 0xb;
            o->state = 0;
        }
        if (D_8009D670[0] & 0x40) {
            U8(D_8009C330, 9) = 0;
            o->y.p.whole += 9;
            FUN_80041ca8(o, o->h->p.whole, o->y.p.whole);
            setanim(o, 10);
            o->step = 0xd;
            o->state = 0;
        }
        break;
    }
    FUN_800ef1ac(o);
}

// FUNC 8013b51c 1116 X001
// MATCHING 8013b51c 1116
#include "TOBJ.H"

extern unsigned char D_8009CEE1, D_8009CEE2;
extern unsigned char D_8009D0B4[];
extern unsigned short D_800A604A[];
extern unsigned char D_8009CE56;
extern unsigned char D_8009C93E, D_8009C93F, D_8009C942;
extern TObj D_800A6038;
extern char D_80010748[];
extern Fix16 *D_800A6078[];
extern short D_800A604E[];
extern TObj *D_8009F2EC[];
extern unsigned char D_8009CFCF, D_8009C975;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern TObj *D_1F8001D4;
extern int FUN_8001f9e0(void);
extern void FUN_801148d0(TObj *, char, int, int, short);
extern void FUN_8001fe6c(void *);
extern int FUN_8001fdac(int, int);

#define S16(o, off) (*(short *)((char *)(o) + (off)))

void func_8013B51C(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;
    short i;
    short n;
    int a, b;

    switch (o->step) {
    case 0:
        o->step++;
    case 1:
        if (D_8009CEE2) {
            n = D_8009D0B4[0];
            if (n > 30) n = 30;
            for (i = 0; i < n; i++) {
                a = FUN_8001f9e0() & 3;
                b = FUN_8001f9e0() & 3;
                FUN_801148d0(e, 1, (short)(D_800A604A[0] + a), (short)(D_800A604A[2] - b), (short)(D_800A604A[4] - 0x10));
            }
            D_8009CEE1 = 1;
            D_8009CEE2 = 0;
        }
        if (D_8009CE56 != 2) break;
        D_8009C93F = 1;
        D_8009C93E = 1;
        D_8009C942 = 1;
        D_800A6038.anim = D_80010748;
        FUN_8001fe6c(&D_800A6038);
        o->a.raw = e->h->raw;
        {
            int y = e->y.raw;
            o->step++;
            o->w08 = 0x3c;
            o->y.raw = y;
        }
        break;
    case 2:
        e->h->raw = o->a.raw + ((((FUN_8001f9e0() & 3) << 8) - 0x180) << 8);
        e->y.raw = o->y.raw + ((((FUN_8001f9e0() & 7) << 8) - 0x380) << 8);
        if (o->w08 != 0 && --o->w08 <= 0) D_8009CE56 = 3;
        if (D_8009CE56 != 4) break;
        e->h->raw = o->a.raw;
        e->y.raw = o->y.raw;
        o->y.raw = 0;
        p = D_8009F2EC[0];
        S16(o, 0x34) = D_800A6078[0]->p.whole - e->h->p.whole;
        S16(o, 0x36) = D_800A604E[0] - e->y.p.whole;
        S16(o, 0x38) = p->h->p.whole - e->h->p.whole;
        S16(o, 0x3a) = p->y.p.whole - e->y.p.whole;
        S16(o, 0xa) = 0;
        o->step++;
        break;
    case 3:
        if (--S16(o, 0xa) > 0) break;
        S16(o, 0xa) = 0x3c;
        o->step++;
        break;
    case 4:
        o->w08 = (o->w08 + 2) & 0xff;
        e->h->raw = o->a.raw + ((FUN_8001fdac(o->w08, 0x40) << 16) >> 8);
        e->y.raw += o->y.raw << 8;
        D_800A6078[0]->p.whole = e->h->p.whole + S16(o, 0x34);
        p = D_8009F2EC[0];
        D_800A604E[0] = e->y.p.whole + S16(o, 0x36);
        p->h->p.whole = e->h->p.whole + S16(o, 0x38);
        p->y.p.whole = e->y.p.whole + S16(o, 0x3a);
        o->y.raw -= 8;
        if (--S16(o, 0xa) > 0) break;
        D_8009CFCF = 1;
        D_8009C975 = 3;
        o->step++;
        break;
    case 5:
        if (D_8009C975 != 1) break;
        D_8009CD94 = 0x13;
        D_8009CD96 = 2;
        D_8009CDA0 = 1;
        {
            TObj *q = D_1F8001D4;
            q->w4c = 7;
            q->w4e = 0;
        }
        o->step++;
        break;
    }
}

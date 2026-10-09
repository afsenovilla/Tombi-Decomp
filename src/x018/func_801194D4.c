// FUNC 801194d4 616 X018
// MATCHING 801194d4 616
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;
typedef struct { TObj o; char pc0[0x12]; unsigned short d2; } TObjX;

extern unsigned char D_8009C93F[], D_8009C942[], D_8009D2B0;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[], D_1F8001C6;
extern AT3 D_80116564[];
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);

static __inline__ void chg(TObjX *e, unsigned short n)
{
    if (e->d2 != n) {
        e->o.anim = D_80116564[e->o.subtype].p[n];
        FUN_8001fe94(&e->o, 0);
        e->d2 = n;
    }
}

void func_801194D4(TObjX *e)
{
    TObj *o = &e->o;
    B12 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(B12 *)&o->a;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        chg(e, 4);
        v = *(B12 *)&o->a;
        o->d90 = (int)FUN_8002dcc8(3, 0xd, &v);
        o->state++;
        break;
    case 1:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        chg(e, 1);
        o->state = 9;
        break;
    case 9:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

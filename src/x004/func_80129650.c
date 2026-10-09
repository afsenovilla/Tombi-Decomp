// FUNC 80129650 652 X004
// MATCHING 80129650 652
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80131188[];
extern unsigned char D_8009D2B0;
extern short D_800A60EA[];
extern unsigned char D_8009C93E[], D_8009C93F[], D_8009C942[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009CFDD, D_8009CFF7;
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void func_8004D620(int, int);

#define WD2(o) (*(unsigned short *)((char *)(o) + 0xd2))

static __inline__ void setanim(TObj *o, short n)
{
    if (WD2(o) != n) {
        o->anim = D_80131188[o->subtype].anims[n];
        AnimJump(o, 0);
        WD2(o) = n;
    }
}

void func_80129650(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_8009D2B0 = 2;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        setanim(o, 2);
        *(short *)((char *)o + 0xbc) = o->animFrame;
        if (D_8009CFDD == 0) {
            D_8009CFDD = 1;
            D_8009CFF7++;
        }
        func_8004D620(D_8009CFF7 + 0x32, 2);
        o->animFrame = o->b68 & 1;
        o->d90 = FUN_8002dcc8(5, 1, &v);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setanim(o, 0);
        D_800A60EA[0] = 0;
        o->state = 9;
        break;
    case 9:
        D_8009C942[0] = 0;
        D_8009C93F[0] = 0;
        D_8009C93E[0] = 0;
        D_800A60EA[0] = 0;
        o->animFrame = o->wbc;
        D_1F8001C6 = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

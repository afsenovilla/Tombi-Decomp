// FUNC 801191dc 760 X018
// MATCHING 801191dc 760
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8011A564[];
extern unsigned char D_8009D2B0[];
extern short D_800A60EA[];
extern unsigned char D_8009C93F[], D_8009C942[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern Fix16 *D_800A6078;
extern unsigned char D_8009CE28;
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a9a4(int, int);
extern void FUN_80026f4c(void);
extern void func_8004D620(int, int);
extern void FUN_80026e0c(int, int);

#define WD2(o) (*(unsigned short *)((char *)(o) + 0xd2))

static __inline__ void setanim(TObj *o, short n)
{
    if (WD2(o) != n) {
        o->anim = D_8011A564[o->subtype].anims[n];
        AnimJump(o, 0);
        WD2(o) = n;
    }
}

void func_801191DC(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        v = *(V6 *)&o->a;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009D2B0[0] = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = D_800A6078->p.whole < o->h->p.whole;
        setanim(o, 4);
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(3, 0xc, &v);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->timer = 200;
        setanim(o, 1);
        o->state++;
        break;
    case 2:
        if (--o->timer > 0) break;
        if (D_8009CE28 != 0xff) FUN_8005a9a4(0x84, 0);
        FUN_80026f4c();
        func_8004D620(0x15, 3);
        FUN_80026e0c(0x99, 1);
        o->timer = 0x50;
        o->state = 8;
        break;
    case 8:
        if (--o->timer > 0) break;
        o->state = 9;
        break;
    case 9:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
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

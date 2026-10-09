// FUNC 80119034 488 X011
// MATCHING 80119034 488
#include "TOBJ.H"
typedef struct { char c[12]; } V12;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119C48[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009D2B0[];
extern unsigned char D_800A603C[];
extern unsigned char D_800A603D[];
extern unsigned char D_800A603E[];
extern short D_800A60EA[];
extern short D_1F8001C6;
extern void AnimLoadDuration(TObj *);
extern int FUN_8002dcc8(int, int, V12 *);

void func_80119034(TObj *o)
{
    V12 b;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68) {
            D_8009C942[0] = 1;
            D_8009D2B0[0] = 2;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->wbc = o->animFrame;
            o->animFrame = o->b68 & 1;
            b = *(V12 *)&o->a;
            o->anim = D_80119C48[o->subtype].anims[2];
            AnimLoadDuration(o);
            o->d90 = FUN_8002dcc8(2, 5, &b);
            o->state++;
        }
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->anim = D_80119C48[o->subtype].anims[0];
            AnimLoadDuration(o);
            o->state++;
        }
        break;
    case 2:
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

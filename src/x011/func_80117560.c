// FUNC 80117560 636 X011
// MATCHING 80117560 636
#include "TOBJ.H"

typedef struct V { char c[12]; } V;
typedef struct { void **p; int pad[2]; } E12;
extern E12 D_80119C48[];
extern TObj D_800A6038;
extern unsigned char D_8009C940[], D_8009C941;
extern unsigned char D_8009C942[];
extern signed char D_8009D2B0[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[];
extern short D_1F8001C6;
extern unsigned short D_1F8001FC, D_1F8003C4;
extern TObj *FUN_8002dcc8(int, int, V *);
extern void AnimLoadDuration(TObj *);

void func_80117560(TObj *o)
{
    TObj *p;
    V v;

    switch (o->state) {
    case 0:
        if (D_8009C940[0]) {
            if (D_8009C941 == 0x41) {
                o->step = 3;
                o->state = 0;
                D_8009C940[0] = 0;
            }
            break;
        }
        if (D_8009D2B0[0] == 3)
            break;
        if ((unsigned short)(D_800A6038.a.p.whole - 0x40) >= 0x20)
            break;
        if (D_800A6038.d->p.whole != 0)
            break;
        if ((D_1F8001FC & D_1F8003C4) == 0)
            break;
        v = *(V *)&o->a;
        D_8009C942[0] = 1;
        D_8009D2B0[0] = 0;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->anim = D_80119C48[o->subtype].p[2];
        AnimLoadDuration(o);
        o->d90 = (int)FUN_8002dcc8(2, 0xa, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_80119C48[o->subtype].p[0];
        AnimLoadDuration(o);
        o->state++;
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

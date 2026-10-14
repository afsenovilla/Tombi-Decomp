// FUNC 80131fec 552 X003
// MATCHING 80131fec 552
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;

extern TObj *D_8009F2E0;
extern unsigned char PLAYER;
extern short D_800A6066, D_800A60EA;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void AnimAdvance(TObj *);
extern void func_801289C8(TObj *, int, int);

void func_80131FEC(TObj *o)
{
    TObj *p = D_8009F2E0;
    TObj *q;
    V6 v;

    switch (o->state) {
    case 0:
        if (GCD.bcdc1) {
            o->step = 2;
            o->state = 0;
            break;
        }
        o->state++;
        break;
    case 1:
        if (GVARS.b3a == 0) break;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        PLAYER = 1;
        D_800A6066 = 0;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        setEventStarted(0x1d, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 2:
        if (--o->w08 > 0) break;
        v = *(V6 *)&p->a;
        p->d90 = showMessageBoxDirect(2, 0xf, &v);
        func_801289C8(p, 9, 0);
        o->state++;
        break;
    case 3:
        AnimAdvance(p);
        q = (TObj *)p->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        func_801289C8(p, 0, 0);
        setEventComplete(0x1a, 0);
        o->w08 = 200;
        o->state = 15;
        break;
    case 15:
        if (--o->w08 > 0) break;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        D_800A60EA = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        p->b68 = 0;
        p->step = 1;
        p->state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

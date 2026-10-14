// FUNC 80132d80 1148 X003
// MATCHING 80132d80 1148
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;

extern TObj *SCRIPT_OBJECTS;
extern unsigned char PLAYER;
extern short D_800A6066, D_800A60EA;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);
extern void FUN_80026c50(int, int, int);
extern void func_801289C8(TObj *, int, int);

void flameBlockedNpcTalk(TObj *o)
{
    TObj *p = SCRIPT_OBJECTS;
    TObj *q;
    V6 v;

    switch (o->state) {
    case 0:
        if (GVARS.b3a == 0) break;
        o->state = 10;
        if (GCD.bce50) break;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        D_800A6066 = 0;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        setEventStarted(0xac, 0, 0);
        o->w08 = 300;
        o->state = 1;
        break;
    case 1:
        if (--o->w08 > 0) break;
        o->state = 11;
        break;
    case 10:
        if (p->b68 == 0) break;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        PLAYER = 1;
        D_800A6066 = 0;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        o->state++;
        if (GCD.bcffc == 0) break;
        v = *(V6 *)&p->a;
        p->d90 = showMessageBoxDirect(7, 2, &v);
        func_801289C8(p, 3, 0);
        o->state = 17;
        break;
    case 11:
        p->animFrame = 1;
        v = *(V6 *)&p->a;
        p->d90 = showMessageBoxDirect(7, 0, &v);
        func_801289C8(p, 3, 0);
        o->state++;
        break;
    case 12:
        q = (TObj *)p->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        v = *(V6 *)&p->a;
        p->d90 = showMessageBoxDirect(7, 1, &v);
        func_801289C8(p, 3, 0);
        o->state++;
        break;
    case 13:
        q = (TObj *)p->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        p->animFrame = 0;
        v = *(V6 *)&p->a;
        p->d90 = showMessageBoxDirect(7, 2, &v);
        func_801289C8(p, 3, 0);
        o->state++;
        break;
    case 14:
        {
        TObj *q = (TObj *)p->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        func_801289C8(p, 0, 0);
        o->w08 = 8;
        o->state++;
        }
        break;
    case 15:
        if (--o->w08 > 0) break;
        p->animFrame = 1;
        v = *(V6 *)&p->a;
        p->d90 = showMessageBoxDirect(7, 3, &v);
        func_801289C8(p, 3, 0);
        o->state++;
        break;
    case 16:
        {
        TObj *q = (TObj *)p->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        p->animFrame = 0;
        FUN_80026c50(0x8d, 1, 1);
        GCD.bcffc = 1;
        o->w08 = 0x3c;
        o->state = 25;
        }
        break;
    case 17:
        q = (TObj *)p->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->w08 = 2;
        o->state = 25;
        break;
    case 25:
        if (--o->w08 > 0) break;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        p->animFrame = 1;
        p->b68 = 0;
        PLAYER = 1;
        D_800A60EA = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        func_801289C8(p, 0, 0);
        o->state = 0;
        break;
    }
}

// FUNC 8011973c 2300 X018
// MATCHING 8011973c 2300
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **p; int a, b; } T12;
typedef struct { char p[0xd2]; unsigned short wd2; } E;
extern T12 D_8011A564[];
extern TObj PLAYER;
#define P PLAYER
extern signed char D_8009D2B0[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[];
extern short D_1F8001C6;
extern int AnimAdvance(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern int showMessageBoxDirect(int, int, V6 *);
extern void FUN_80026e0c(int, int);
extern void setEventComplete(int, int);
extern void FUN_80026c50(int, int, int);

#define WD2(o) (((E *)(o))->wd2)

static __inline__ void setanim(TObj *o, short n)
{
    if (WD2(o) != n) {
        o->anim = D_8011A564[o->subtype].p[n];
        FUN_8001fe94(o, 0);
        WD2(o) = n;
    }
}

void func_8011973C(TObj *o)
{
    V6 v;
    TObj *p;
    short i;

    switch (o->state) {
    case 0:
        v = *(V6 *)&o->a;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        D_8009D2B0[0] = 2;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b3e)[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = P.h->p.whole < o->h->p.whole;
        setanim(o, 4);
        v = *(V6 *)&o->a;
        switch (GVARS.b41) {
        case 8:
            ((unsigned char*)&GCD.bcff0)[0] = 1;
            break;
        case 0x66:
            GCD.bcff1 = 1;
            break;
        case 0x67:
            GCD.bcff2 = 1;
            break;
        case 0x68:
            GCD.bcff3 = 1;
            break;
        case 0x69:
            GCD.bcff4 = 1;
            break;
        case 0x6a:
            GCD.bcff5 = 1;
            break;
        }
        FUN_80026e0c(GVARS.b41, 1);
        for (i = 0; i < 6; i++)
            ;
        for (i = 0; i < 6; i++)
            if (((unsigned char*)&GCD.bcff0)[i] == 0) break;
        if (i == 6) {
            if (GVARS.b41 != 8) {
                o->state = 0x60;
                break;
            }
            o->timer = 0x12c;
            o->state = 0x65;
            setEventComplete(0xa9, 0);
        } else if (GVARS.b41 == 8) {
            o->timer = 0x12c;
            o->state = 0x64;
            setEventComplete(0xa9, 0);
        } else {
            o->d90 = showMessageBoxDirect(3, 0xf, &v);
            o->state = 0x61;
        }
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state = 0x63;
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x10, &v);
        WD2(o) = 1;
        o->state = 9;
        break;
    case 3:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x11, &v);
        WD2(o) = 2;
        o->state = 9;
        break;
    case 4:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x12, &v);
        WD2(o) = 3;
        o->state = 9;
        break;
    case 5:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x13, &v);
        WD2(o) = 4;
        o->state = 9;
        break;
    case 6:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x14, &v);
        WD2(o) = 5;
        o->state = 9;
        break;
    case 7:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x15, &v);
        WD2(o) = 6;
        o->state = 9;
        break;
    case 8:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x16, &v);
        o->animFrame = P.h->p.whole < o->h->p.whole;
        setanim(o, 4);
        o->state = 1;
        break;
    case 9:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x16, &v);
        o->state = 1;
        break;
    case 0x60:
        setEventComplete(0x78, 0);
        FUN_80026c50(0x73, 1, 1);
        o->timer = 0x12c;
        o->state = 0x62;
        break;
    case 0x61:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        for (i = 0; i < 6; i++)
            if (((unsigned char*)&GCD.bcff0)[i] == 0) break;
        if (i == 6) break;
        v = *(V6 *)&o->a;
        setanim(o, 4);
        o->timer = 0;
        o->state = i + 2;
        break;
    case 0x62:
        if (--o->timer <= 0) o->state = 0x63;
        break;
    case 0x63:
        setanim(o, 0);
        ((unsigned char*)&GVARS.b40)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
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
    case 0x64:
        AnimAdvance(o);
        if (--o->timer != -1) break;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0xf, &v);
        o->state = 0x61;
        break;
    case 0x65:
        AnimAdvance(o);
        if (--o->timer == -1) o->state = 0x60;
        break;
    }
}

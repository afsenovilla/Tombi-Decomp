// FUNC 80125fb4 1280 X010
/* score 124: everything matches except that case 5 is cross-jumped into case 8 from the
   FUN_8002dcc8 call (identical arg setup); in the game case 8 sets up a2 first (li a1,9 in the
   jal delay slot), so the tails only merge from setanim(o, 2). Tried: pointer/extra local for
   the V6 copy, an fx() inline, moving the copy among the global stores. */
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012F3C4[];
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_8009D0E7[];
extern unsigned char D_8009C970, D_8009C971;
extern short D_800A6108, D_800A610A, D_800A60EA;
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a9a4(int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_80026c50(int, int, int);
extern void FUN_8001e4f0(int);

static __inline__ void setanim(TObj *o, int n)
{
    o->anim = D_8012F3C4[o->subtype].anims[n];
    FUN_8001fe94(o, 0);
}

void func_80125FB4(TObj *o)
{
    V6 v;
    short i;
    unsigned char *c;

    switch (o->state) {
    case 0:
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        FUN_8005a9a4(0x7e, 0);
        o->timer = 300;
        o->state++;
        break;
    case 2:
        if (--o->timer > 0) break;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(5, 6, &v);
        setanim(o, 2);
        for (i = 0; i < 10; i++)
            FUN_80026e0c(i + 0x43, D_8009D0E7[i]);
        o->state++;
        break;
    case 3:
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_80026c50(0x2c, 1, 1);
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(5, 7, &v);
        o->state++;
        }
        break;
    case 4:
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setanim(o, 0);
        o->timer = 0x3c;
        o->state++;
        }
        break;
    case 5:
        if (--o->timer > 0) break;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(5, 8, &v);
        setanim(o, 2);
        o->state++;
        break;
    case 6:
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_80026c50(0x95, 1, 1);
        setanim(o, 0);
        FUN_8005a8a8(0x9c, 0, 0);
        o->timer = 300;
        o->state++;
        }
        break;
    case 7:
        if (--o->timer <= 0) o->state++;
        break;
    case 8:
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(5, 9, &v);
        setanim(o, 2);
        o->state++;
        break;
    case 9:
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        c = &D_8009C970;
        if (*c < D_8009C971) {
            int t = D_8009C971;
            D_800A6108 = t;
            D_800A610A = t;
            *c = t;
            FUN_8001e4f0(10);
        }
        setanim(o, 0);
        D_800A603C = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_800A60EA = 0;
        D_800A603D = 0;
        D_800A603E = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        }
        break;
    }
}

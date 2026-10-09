// FUNC 801253ac 1844 X010
/* score 225: all case bodies written. The sparse-switch comparison tree differs: the game splits the lower half at
   0x46 (one more node than this source gives; adding `case 0x48:` to 0x49 gets the shape but emits a range test).
   Also case 10 keeps D_8009CDF2 in a1 (re-read later) and the talk-0x9f test is a materialized bool (a0/v0). */
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012F3B8[];
extern unsigned char D_8009C93E, D_8009C93F[], D_8009C940, D_8009C941, D_8009C942[];
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern short D_800A60EA;
extern unsigned char D_8009CDF2, D_8009D143, D_8009C975;
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_80026e0c(int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void AnimJump(TObj *, int);

#define NEXT(o) (*(short *)((char *)(o) + 0xc2))

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimJump(o, 0);
}

static __inline__ int talk9f(void)
{
    if (D_8009C940 == 1) return D_8009C941 == 0x9f;
    return 0;
}

void func_801253AC(TObj *o)
{
    V6 v;
    TObj *p;
    int m, n;
    unsigned char k;

    switch (o->state) {
    case 0:
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        o->animFrame = o->b68 & 1;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        break;
    case 1:
        D_8009C93E = 1;
        o->state = 10;
        break;
    case 10:
        k = D_8009CDF2;
        if (k == 0xff) break;
        if (talk9f() && D_8009CDF2 != 0xff) {
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(4, 3, &v);
            FUN_80026e0c(0x9f, 1);
            D_8009C940 = 0;
            o->state = 0x5a;
            NEXT(o) = 0x46;
            break;
        }
        if (k == 0) {
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(4, 0, &v);
            o->state = 0x5a;
            NEXT(o) = 0x32;
            break;
        }
        if (o->b68 && D_8009D143) {
            v = *(V6 *)&o->a;
            m = 4;
            n = 2;
        } else {
            v = *(V6 *)&o->a;
            m = 4;
            n = 1;
        }
        o->d90 = FUN_8002dcc8(m, n, &v);
        o->state = 0x5a;
        NEXT(o) = 0x7a;
        break;
    case 0x32:
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(4, 1, &v);
        o->state = 0x5a;
        NEXT(o) = 0x33;
        break;
    case 0x33:
        if (D_8009CDF2 == 0) FUN_8005a8a8(0x4e, 0, 0);
        o->state = 0x78;
        NEXT(o) = 0xfb;
        break;
    case 0x46:
        o->animFrame = 1;
        o->velX = 0x600;
        o->state = 0x47;
        o->b69 = 0;
        o->timer = 200;
        setAnim(o, D_8012F3B8[o->subtype].anims[1]);
        FUN_8005a9a4(0x4e, 0);
        break;
    case 0x47:
        o->d->raw += o->velX << 8;
        if (--o->timer != 0) break;
        o->state = 0x78;
        NEXT(o) = 0xfa;
        break;
    case 0x48:
    case 0x49:
        break;
    case 0x5a:
        o->anim = D_8012F3B8[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state = 0x5b;
        break;
    case 0x5b:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_8012F3B8[o->subtype].anims[0];
        AnimJump(o, 0);
        o->state = NEXT(o);
        break;
    case 0x64:
        D_8009C975 = 4;
        o->state = NEXT(o);
        break;
    case 0x65:
        D_8009C975 = 3;
        o->state = NEXT(o);
        break;
    case 0x66:
        D_8009C975 = 4;
        o->state = 0x68;
        break;
    case 0x67:
        D_8009C975 = 3;
        o->state = 0x68;
        break;
    case 0x68:
        if ((unsigned char)(D_8009C975 - 3) < 2) break;
        o->state = NEXT(o);
        break;
    case 0x6e:
        o->timer = 200;
        o->state = 0x6f;
        break;
    case 0x6f:
        if (--o->timer != 0) break;
        o->state = NEXT(o);
        break;
    case 0x78:
        o->timer = 200;
        o->state = 0x79;
        break;
    case 0x79:
        if (--o->timer != 0) break;
        o->state = 0x7a;
        break;
    case 0x7a:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E = 0;
        D_800A60EA = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->b68 = 0;
        switch (NEXT(o)) {
        case 0xf0:
            o->state = 0xf0;
            break;
        case 0xfa:
            o->state = 0xfa;
            break;
        case 0xfc:
            o->state = 0;
            o->step++;
            break;
        case 0xfd:
            o->b04 = 0;
            o->step = 0;
            o->state = 0;
            break;
        case 0xfe:
            o->step = 0;
            o->state = 0;
            o->b04++;
            break;
        default:
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    case 0xf0:
        o->anim = 0;
        break;
    case 0xfa:
        o->b04 = 2;
        break;
    }
}

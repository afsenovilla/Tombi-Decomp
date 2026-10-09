// FUNC 8012c00c 1664 X003
// MATCHING 8012c00c 1664
#include "TOBJ.H"

typedef struct V { char c[12]; } V;
typedef struct { void **p; int pad[2]; } E12;
typedef struct { TObj o; char pc0[2]; unsigned short wc2; } TX;
#define X(o) ((TX *)(o))
extern E12 D_80135DF0[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009C93F[], D_8009C942[], D_8009C93E[];
extern unsigned char D_8009CDC5;
extern unsigned char D_8009CDC5b; /* same byte as D_8009CDC5: second name makes chk() reload it (game reloads; volatile does not match) */
extern unsigned short D_8009C962;
extern unsigned char D_8009C975;
extern short D_800A60EA[];
extern TObj *FUN_8002dcc8(int, int, V *);
extern void AnimJump(TObj *, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern short func_8004065C(TObj *, short, short, short);

static __inline__ int chk(void)
{
    return D_8009C962 == 5 && D_8009CDC5b == 1;
}

void func_8012C00C(TObj *o)
{
    V v;
    int f;
    TObj *p;
    unsigned char c;

    switch (o->state) {
    case 0:
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->animFrame = o->b68 & 1;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        break;
    case 1:
        D_8009C93E[0] = 1;
        o->state = 10;
        break;
    case 10:
        c = D_8009CDC5;
        if (c != 0xff) {
            if (chk()) {
                v = *(V *)&o->a;
                o->d90 = (int)FUN_8002dcc8(2, 3, &v);
                o->state = 0x5a;
                X(o)->wc2 = 0x46;
                break;
            }
            if (c == 0) {
                o->state = 0x1e;
                break;
            }
        }
        v = *(V *)&o->a;
        o->d90 = (int)FUN_8002dcc8(2, 3, &v);
        o->state = 0x5a;
        X(o)->wc2 = 0x7a;
        break;
    case 30:
        v = *(V *)&o->a;
        o->d90 = (int)FUN_8002dcc8(2, 0, &v);
        o->state = 0x5a;
        X(o)->wc2 = 0x1f;
        break;
    case 31:
        v = *(V *)&o->a;
        o->d90 = (int)FUN_8002dcc8(2, 1, &v);
        o->state = 0x5a;
        X(o)->wc2 = 0x32;
        break;
    case 50:
        FUN_8005a8a8(0x21, 0, 0);
        o->state = 0x5a;
        X(o)->wc2 = 0x33;
        break;
    case 51:
        v = *(V *)&o->a;
        o->d90 = (int)FUN_8002dcc8(2, 2, &v);
        o->state = 0x5a;
        X(o)->wc2 = 0x34;
        break;
    case 70:
        o->animFrame = 1;
        o->velX = -0xa0;
        o->b69 = 0;
        o->state = 0x47;
        o->anim = D_80135DF0[o->subtype].p[6];
        AnimJump(o, 0);
        break;
    case 71:
        o->h->raw += o->velX << 8;
        if (func_8004065C(o, o->h->p.whole, o->y.p.whole, o->animFrame) == 2) {
            o->state = 0x48;
        }
        break;
    case 72:
        FUN_8005a9a4(0x21, 0);
        o->anim = D_80135DF0[o->subtype].p[4];
        AnimJump(o, 0);
    case 52:
        o->state = 0x7a;
        X(o)->wc2 = 0xfb;
        break;
    case 90:
        o->anim = D_80135DF0[o->subtype].p[1];
        AnimJump(o, 0);
        o->state = 0x5b;
        break;
    case 91:
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        if (D_8009CDC5 != 0xff) {
            o->anim = D_80135DF0[o->subtype].p[5];
            AnimJump(o, 0);
        } else {
            o->anim = D_80135DF0[o->subtype].p[4];
            AnimJump(o, 0);
        }
        goto next;
    case 110:
        o->timer = 200;
        o->state = 0x6f;
        break;
    case 111:
        if (--o->timer == 0)
            goto next;
        break;
    case 100:
        D_8009C975 = 4;
        goto next;
    case 101:
        D_8009C975 = 3;
        goto next;
    case 102:
        D_8009C975 = 4;
        o->state = 0x68;
        break;
    case 103:
        D_8009C975 = 3;
        o->state = 0x68;
        break;
    case 104:
        if ((unsigned char)(D_8009C975 - 3) < 2)
            break;
    next:
        o->state = X(o)->wc2;
        break;
    case 120:
        o->animFrame = 1;
        o->timer = 200;
        o->state = 0x79;
        break;
    case 121:
        if (--o->timer == 0) {
            o->state = 0x7a;
        }
        break;
    case 122:
        o->animFrame = 1;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        D_800A60EA[0] = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        switch (X(o)->wc2) {
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
    case 240:
        o->anim = 0;
        break;
    case 250:
        o->b04 = 2;
        break;
    }
}

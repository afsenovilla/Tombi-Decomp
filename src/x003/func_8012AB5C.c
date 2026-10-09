// FUNC 8012ab5c 1928 X003
// MATCHING 8012ab5c 1928
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80135DCC[];
extern unsigned char D_8009CDC8;
extern unsigned char D_8009CDC0[];
extern unsigned char D_8009D003;
extern unsigned char D_8009C975;
extern Fix16 *D_800A6078;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009C93E[];
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942[];
extern short D_800A60EA;
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern short FUN_8004065c(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

#define WC2(o) (*(unsigned short *)((char *)(o) + 0xc2))

static __inline__ void setanim(TObj *o, int n)
{
    o->anim = D_80135DCC[o->subtype].anims[n];
    FUN_8001fe94(o, 0);
}

static __inline__ int chk(void)
{
    return D_8009D003 && D_8009CDC0[8] != 0xff;
}

void func_8012AB5C(TObj *o)
{
    V6 v;
    TObj *p;
    unsigned char k;

    switch (o->state) {
    case 0:
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->animFrame = D_800A6078->p.whole < o->h->p.whole;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        break;
    case 1:
        D_8009C93E[0] = 1;
        o->state = 10;
        break;
    case 10:
        k = D_8009CDC8;
        if (k != 0xff) {
            if (chk()) {
                v = *(V6 *)&o->a;
                o->d90 = FUN_8002dcc8(3, 1, &v);
                o->state = 0x5a;
                WC2(o) = 0x46;
            } else if (k == 0) {
                v = *(V6 *)&o->a;
                o->d90 = FUN_8002dcc8(3, 0, &v);
                o->state = 0x5a;
                WC2(o) = 0x32;
            } else {
                v = *(V6 *)&o->a;
                o->d90 = FUN_8002dcc8(3, 2, &v);
                o->state = 0x5a;
                WC2(o) = 0x7a;
            }
        } else {
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(3, 3, &v);
            o->state = 0x5a;
            WC2(o) = 0x7a;
        }
        break;
    case 0x33:
        break;
    case 0x32:
        FUN_8005a8a8(0x24, 0, 0);
        o->state = 0x78;
        WC2(o) = 0xfb;
        break;
    case 0x46:
        o->velX = (o->animFrame ^= 1) & 1 ? -0xa0 : 0xa0;
        o->velY = -0x600;
        o->b69 = 0;
        o->state = 0x47;
        setanim(o, 1);
        break;
    case 0x47:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        FUN_8004065c(o, o->h->p.whole, o->y.p.whole, o->animFrame);
        if (o->velY > 0) o->state = 0x48;
        break;
    case 0x48:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        FUN_8004065c(o, o->h->p.whole, o->y.p.whole, o->animFrame);
        if (FUN_80040278(o, o->h->p.whole, o->y.p.whole) || o->velY > 0x600) o->state = 0x49;
        break;
    case 0x49:
        FUN_8005a9a4(0x24, 0);
        o->animFrame ^= 1;
        setanim(o, 4);
        o->state = 0x78;
        WC2(o) = 0xfb;
        break;
    case 0x5a:
        setanim(o, 5);
        o->state = 0x5b;
        break;
    case 0x5b:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setanim(o, 4);
        o->state = WC2(o);
        break;
    case 0x6e:
        o->timer = 200;
        o->state = 0x6f;
        break;
    case 0x6f:
        if (--o->timer == 0) o->state = WC2(o);
        break;
    case 0x64:
        D_8009C975 = 4;
        o->state = WC2(o);
        break;
    case 0x65:
        D_8009C975 = 3;
        o->state = WC2(o);
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
        if (D_8009C975 != 3 && D_8009C975 != 4) o->state = WC2(o);
        break;
    case 0x78:
        o->timer = 200;
        o->state = 0x79;
        break;
    case 0x79:
        if (--o->timer == 0) o->state = 0x7a;
        break;
    case 0x7a:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        D_800A60EA = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        switch (WC2(o)) {
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
            goto reset;
        case 0xfe:
            o->step = 0;
            o->state = 0;
            o->b04++;
            break;
        default:
        reset:
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

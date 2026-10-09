// FUNC 8012d8e4 1352 X010
// MATCHING 8012d8e4 1352
#include "TOBJ.H"

typedef struct V { char c[12]; } V;
typedef struct { void **p; int pad[2]; } E12;
extern E12 D_8012F538[];
extern unsigned char D_8009C942[], D_8009D2B0[], D_8009CFCC[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009CE3B, D_8009D13E;
extern unsigned char D_8009CFCA;
extern short D_800A60EA[];
extern short D_1F8001C6;
extern TObj *FUN_8002dcc8(int, int, V *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, int, int);
extern void addItemToInventory(int, int, int);
extern void FUN_8005a9a4(int, int);

void func_8012D8E4(TObj *o)
{
    V v;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(V *)&o->a;
        D_8009D2B0[0] = 2;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        D_8009CFCC[0] = 1;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->anim = D_8012F538[o->subtype].p[1];
        AnimLoadDuration(o);
        if (D_8009CE3B == 1) {
            o->d90 = (int)FUN_8002dcc8(3, 0, &v);
            o->state = 10;
        } else {
            o->d90 = (int)FUN_8002dcc8(3, 0, &v);
            o->state = 1;
        }
        break;
    case 1: {
        TObj *p;

        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_8012F538[o->subtype].p[0];
        AnimLoadDuration(o);
        o->animFrame = 1;
        o->velX = -0x180;
        o->velY = -0x600;
        o->state++;
        break;
    }
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0) {
            o->state++;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole)) {
            o->anim = D_8012F538[o->subtype].p[0];
            AnimLoadDuration(o);
            o->velX = -0x180;
            o->velY = -0x600;
            o->state = 2;
        }
        if (o->visible == 0) {
            o->state = 9;
        }
        break;
    case 9: {
        unsigned char *c;

        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        c = &D_8009CFCA;
        o->animFrame = o->wbc;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        *c = *c + 1;
        o->b68 = 0;
        o->b04 = 3;
        o->step = 0;
        o->state = 0;
        break;
    }
    case 10: {
        TObj *p;

        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        if (D_8009D13E == 0) {
            o->anim = D_8012F538[o->subtype].p[0];
            AnimLoadDuration(o);
            v = *(V *)&o->a;
            o->d90 = (int)FUN_8002dcc8(3, 2, &v);
            o->state = 0xb;
        } else {
            o->anim = D_8012F538[o->subtype].p[0];
            AnimLoadDuration(o);
            v = *(V *)&o->a;
            o->d90 = (int)FUN_8002dcc8(3, 1, &v);
            o->state = 0xc;
        }
        break;
    }
    case 11: {
        TObj *p;

        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->state = 0xe;
        break;
    }
    case 12: {
        TObj *p;

        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        addItemToInventory(0x37, 1, 1);
        addItemToInventory(0x37, 1, 1);
        o->state = 0xe;
        break;
    }
    case 14:
        o->timer = 200;
        FUN_8005a9a4(0x97, 0);
        o->state++;
        break;
    case 15:
        if (--o->timer <= 0) {
            o->state = 1;
        }
        break;
    }
}

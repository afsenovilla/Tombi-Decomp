// FUNC 80123708 1552 X010
/* score 2: whole function incl. csv pieces 801237E4/80123B94. Only diff: case 1 velY address 'addu v0,v0(idx),a0(t)' vs game 'addu v0,a0,v0' (t first). t is a REG_EQUAL constant, so the symbol ends up second. Tried: t[i]/t+i/i+t, int/char* t with << and *, block-local t/k, k=idx<<2; k+=t (target swap), t+=i, &t[i], &D[0]. */
#include "TOBJ.H"
typedef struct { unsigned short a, b; } T4;
extern T4 D_8012F398[];
extern int Rand(void);
extern int FUN_8001fec0(TObj *);
extern short MulNegSinScaled(int, short);

static __inline__ void lo(TObj *o, TObj *e)
{
    short y = e->y.p.whole;
    short b = e->box2;
    if (o->y.p.whole < y - b) {
        o->y.p.whole = y - b;
        o->d34 = o->y.raw - e->y.raw;
    }
}

static __inline__ void hi(TObj *o, TObj *e)
{
    short y = e->y.p.whole;
    short b = e->box2;
    if (y + b < o->y.p.whole) {
        o->y.p.whole = y + b;
        o->d34 = o->y.raw - e->y.raw;
    }
}

static __inline__ void lo0(TObj *o, TObj *e)
{
    short y = e->y.p.whole;
    short b = e->box2;
    if (o->y.p.whole < y - b) {
        o->y.p.whole = y - b;
        o->d34 = o->y.raw - e->y.raw;
        o->state = 0;
    }
}

static __inline__ void hi0(TObj *o, TObj *e)
{
    short y = e->y.p.whole;
    short b = e->box2;
    if (y + b < o->y.p.whole) {
        o->y.p.whole = y + b;
        o->d34 = o->y.raw - e->y.raw;
        o->state = 0;
    }
}

#define LO(e) lo(o, e)
#define HI(e) hi(o, e)
#define MOVE() o->h->raw = o->d30 + hx; o->y.raw = o->d34 + ey

void func_80123708(TObj *o)
{
    TObj *e = (TObj *)o->d90;
    int hx = e->h->raw;
    int ey = e->y.raw;
    T4 *t;

    switch (o->state) {
    case 0:
        switch (Rand() & 3) {
        case 0:
            o->state = 1;
            break;
        case 1:
            o->state = 1;
            break;
        case 2:
            o->state = 5;
            break;
        case 3:
            o->state = 7;
            break;
        }
        LO(e);
        HI(e);
        break;
    case 1:
        o->animFrame = Rand() & 1;
        o->w74 = Rand() & 3;
        t = D_8012F398;
        o->velX = t[o->w74].a;
        if (o->animFrame & 1) o->velX = -o->velX;
        o->velY = (t + o->w74)->b;
        o->timer = 200;
        o->d84 = 0;
        o->d8c = 0;
        o->state++;
    case 2:
        FUN_8001fec0(o);
        if ((unsigned short)(o->h->p.whole - (e->h->p.whole - e->box0)) > e->box1) {
            o->animFrame ^= 1;
            o->velX = -o->velX;
        }
        o->d30 += o->velX << 8;
        o->h->raw = o->d30 + hx;
        o->d84 = (o->d84 + 2) & 0xff;
        o->y.raw = o->d34 + ey + (MulNegSinScaled(o->d84, o->velY) << 8);
        LO(e);
        HI(e);
        if (--o->timer <= 0) o->state = 0;
        break;
    case 3:
        o->animFrame = Rand() & 1;
        o->state++;
        o->velX = 0;
        o->velY = 0;
        o->velH = 0;
        o->velV = 0;
        o->timer = 0x3c;
        MOVE();
        break;
    case 4:
        FUN_8001fec0(o);
        MOVE();
        if (--o->timer <= 0) o->state = 0;
        break;
    case 5:
        o->animFrame = Rand() & 1;
        o->velY = 0x300;
        o->state++;
        o->velX = 0;
        o->velH = 0;
        o->velV = 0;
        o->timer = 0x28;
        MOVE();
        LO(e);
        hi0(o, e);
        break;
    case 6:
        FUN_8001fec0(o);
        o->d34 += o->velY << 8;
        o->velY -= 0x20;
        MOVE();
        if (o->velY < 0) o->velY = 0;
        HI(e);
        if (--o->timer <= 0) o->state = 0;
        break;
    case 7:
        o->animFrame = Rand() & 1;
        o->velY = -0x300;
        o->state++;
        o->velX = 0;
        o->velH = 0;
        o->velV = 0;
        o->timer = 0x28;
        HI(e);
        lo0(o, e);
        MOVE();
        break;
    case 8:
        FUN_8001fec0(o);
        o->d34 += o->velY << 8;
        o->velY += 0x20;
        MOVE();
        if (o->velY > 0) o->velY = 0;
        LO(e);
        if (--o->timer <= 0) o->state = 0;
        break;
    }
}

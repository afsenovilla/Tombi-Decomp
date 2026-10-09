// FUNC 80137d8c 1552 X001
/* score 145: logic decoded (9-state mover clamped to the parent's box). Left: frame 0xc8 vs 0xd0 (inline lo/hi
   param copies give the frame; one more 8 B slot missing), store scheduling in the init states, case 1 reads
   velY through a table pointer in a0, case 0 HI tail shares less. */
#include "TOBJ.H"
typedef struct { short x, y; } V2;

extern V2 D_8013C9DC[];
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern int MulNegSinScaled(short a, int b);

static __inline__ void lo(TObj *o, TObj *p, short y, short b)
{
    if (o->y.p.whole < y - b) {
        o->y.p.whole = y - b;
        o->d34 = o->y.raw - p->y.raw;
    }
}

static __inline__ void hi(TObj *o, TObj *p, short y, short b)
{
    if (y + b < o->y.p.whole) {
        o->y.p.whole = y + b;
        o->d34 = o->y.raw - p->y.raw;
    }
}

#define LO() lo(o, p, p->y.p.whole, p->box2)
#define HI() hi(o, p, p->y.p.whole, p->box2)

void func_80137D8C(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    int hx = p->h->raw;
    int py = p->y.raw;

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
        LO();
        HI();
        break;
    case 1:
        o->animFrame = Rand() & 1;
        o->w74 = Rand() & 3;
        o->velX = D_8013C9DC[o->w74].x;
        if (o->animFrame & 1) o->velX = -o->velX;
        o->state++;
        o->timer = 0xc8;
        o->d84 = 0;
        o->d8c = 0;
        { V2 *t = D_8013C9DC; o->velY = t[o->w74].y; }
    case 2:
        AnimAdvance(o);
        if (p->box1 < (unsigned short)(o->h->p.whole - (p->h->p.whole - p->box0))) {
            o->animFrame ^= 1;
            o->velX = -o->velX;
        }
        o->d30 += o->velX << 8;
        o->h->raw = o->d30 + hx;
        o->d84 = (o->d84 + 2) & 0xff;
        o->y.raw = o->d34 + py + ((MulNegSinScaled(o->d84, o->velY) << 16) >> 8);
        LO();
        HI();
        if (--o->timer <= 0) o->state = 0;
        break;
    case 3:
        o->animFrame = Rand() & 1;
        o->state++;
        o->timer = 0x3c;
        o->velX = 0;
        o->velY = 0;
        o->velH = 0;
        o->velV = 0;
        o->h->raw = o->d30 + hx;
        o->y.raw = o->d34 + py;
        break;
    case 4:
        AnimAdvance(o);
        o->h->raw = o->d30 + hx;
        o->y.raw = o->d34 + py;
        if (--o->timer <= 0) o->state = 0;
        break;
    case 5:
        o->animFrame = Rand() & 1;
        o->velY = 0x300;
        o->state++;
        o->timer = 0x28;
        o->velX = 0;
        o->velH = 0;
        o->velV = 0;
        o->h->raw = o->d30 + hx;
        o->y.raw = o->d34 + py;
        LO();
        if (p->y.p.whole + p->box2 < o->y.p.whole) {
            o->y.p.whole = p->y.p.whole + p->box2;
            o->state = 0;
            o->d34 = o->y.raw - p->y.raw;
        }
        break;
    case 6:
        AnimAdvance(o);
        o->d34 += o->velY << 8;
        o->velY -= 0x20;
        o->h->raw = o->d30 + hx;
        o->y.raw = o->d34 + py;
        if (o->velY < 0) o->velY = 0;
        HI();
        if (--o->timer <= 0) o->state = 0;
        break;
    case 7:
        o->animFrame = Rand() & 1;
        o->velY = -0x300;
        o->state++;
        o->timer = 0x28;
        o->velX = 0;
        o->velH = 0;
        o->velV = 0;
        HI();
        if (o->y.p.whole < p->y.p.whole - p->box2) {
            o->y.p.whole = p->y.p.whole - p->box2;
            o->state = 0;
            o->d34 = o->y.raw - p->y.raw;
        }
        o->h->raw = o->d30 + hx;
        o->y.raw = o->d34 + py;
        break;
    case 8:
        AnimAdvance(o);
        o->d34 += o->velY << 8;
        o->velY += 0x20;
        o->h->raw = o->d30 + hx;
        o->y.raw = o->d34 + py;
        if (o->velY > 0) o->velY = 0;
        LO();
        if (--o->timer <= 0) o->state = 0;
        break;
    }
}

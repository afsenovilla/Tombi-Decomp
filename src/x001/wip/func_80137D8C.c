// FUNC 80137d8c 1552 X001
/* score 2: only `addu v0,v0,a0` vs game `addu v0,a0,v0` for t[o->w74].y in case 1 (t = table pointer set before the velX negation if, global pseudo in a0). Tried t+idx/idx+t/char-pointer and int forms, index temp k (both decl orders), t += w74, cast index types; the swap happens in cse (expand gives (plus t k) for an index var k; cse dump has (plus k t)), even with t made opaque by an asm, and -fno-cse-skip-blocks/-follow-jumps/-rerun do not change it. Frame: 10 two-short-param inlines (each leaves two `(use (reg))` stack slots) + unused `V2 e` local (8 B) give 0xd0. State inits: `state++` first, then zero stores, then constants. */
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

static __inline__ void hi2(TObj *o, TObj *p, short y, short b)
{
    if (y + b < o->y.p.whole) {
        o->y.p.whole = y + b;
        o->d34 = o->y.raw - p->y.raw;
        o->state = 0;
    }
}

static __inline__ void lo2(TObj *o, TObj *p, short y, short b)
{
    if (o->y.p.whole < y - b) {
        o->y.p.whole = y - b;
        o->d34 = o->y.raw - p->y.raw;
        o->state = 0;
    }
}

#define LO() lo(o, p, p->y.p.whole, p->box2)
#define HI() hi(o, p, p->y.p.whole, p->box2)

void func_80137D8C(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    int hx = p->h->raw;
    int py = p->y.raw;
    V2 *t;
    V2 e;

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
        t = D_8013C9DC;
        if (o->animFrame & 1) o->velX = -o->velX;
        o->velY = t[o->w74].y;
        o->timer = 0xc8;
        o->d84 = 0;
        o->d8c = 0;
        o->state++;
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
        o->velX = 0;
        o->velY = 0;
        o->velH = 0;
        o->velV = 0;
        o->timer = 0x3c;
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
        o->state++;
        o->velX = 0;
        o->velH = 0;
        o->velV = 0;
        o->velY = 0x300;
        o->timer = 0x28;
        o->h->raw = o->d30 + hx;
        o->y.raw = o->d34 + py;
        LO();
        hi2(o, p, p->y.p.whole, p->box2);
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
        o->state++;
        o->velX = 0;
        o->velH = 0;
        o->velV = 0;
        o->velY = -0x300;
        o->timer = 0x28;
        HI();
        lo2(o, p, p->y.p.whole, p->box2);
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

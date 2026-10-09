// FUNC 80124e04 872 X009
// MATCHING 80124e04 872
#include "TOBJ.H"

typedef struct { short x, y; } P;

extern short D_8007A5F0[], D_8007A3F0[];
extern short D_1F800176, D_1F800186;
extern void *D_8012EE80[];
extern int FUN_8002078c(P, P);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

static __inline__ int blocked(TObj *o)
{
    int d;
    unsigned short f = o->animFrame;
    if ((o->b9d & 2) && f == (o->b9d & 1)) {
        return 1;
    }
    {
        short g = f;
        d = g ? -16 : 16;
        return func_8004065C(o, o->h->p.whole + d, o->y.p.whole + 0x30, g);
    }
}

void func_80124E04(TObj *o)
{
    P a, b;
    short r;
    short c;
    unsigned char st;
    short t;

    st = o->state;
    switch (st) {
    case 0:
        if (!o->visible) {
        o->state++;
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        if (o->h->p.whole < D_1F800176 + 0xa0) {
            b.x = D_1F800176 + 0x20;
            o->animFrame = 0;
        } else {
            b.x = D_1F800176 + 0x120;
            o->animFrame = 1;
        }
        b.y = D_1F800186 - 0xc8;
        o->d88 = FUN_8002078c(a, b);
        o->w74 = b.x;
        o->wac = 0;
        o->anim = D_8012EE80[0];
        AnimLoadDuration(o);
        } else {
            o->state = 2;
            AnimAdvance(o);
        }
        break;
    case 1:
        if (o->visible) {
            if (!(o->animFrame & 1)) c = o->h->p.whole < o->w74;
            else c = o->h->p.whole > o->w74;
            if (!c) o->state = st + 1;
            o->velH = 1;
            o->wac = 0;
            o->anim = D_8012EE80[0];
            AnimLoadDuration(o);
        } else {
            if (!(o->animFrame & 1)) c = o->h->p.whole < o->w74;
            else c = o->h->p.whole > o->w74;
            if (c) o->state = 0;
            o->velH = 0xc;
        }
        o->h->raw += (D_8007A5F0[(unsigned char)o->d88] * o->velH) << 4;
        o->y.raw += (D_8007A3F0[(unsigned char)o->d88] * o->velH) << 4;
        r = blocked(o);
        t = TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
        if (r && t) o->y.p.whole -= 2;
        break;
    case 2:
        if (o->subtype) o->step = 5;
        else o->step = 1;
        o->state = 0;
        break;
    }
}

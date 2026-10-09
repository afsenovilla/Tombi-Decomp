// FUNC 8012c97c 660 X003
// MATCHING 8012c97c 660
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009D2C3, D_8009CF03;
extern unsigned char D_800A603C, D_800A603D, D_800A603E, D_8009C93F, D_8009C942;
extern void *D_801399D8[], *D_801399D4[];
extern int AnimAdvance(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);

static __inline__ int hit(TObj *o)
{
    if (TileCollideAt(o, o->h->p.whole + 8, o->y.p.whole + 0x10)) return 1;
    return TileCollideAt(o, o->h->p.whole - 8, o->y.p.whole + 0x10) != 0;
}

void func_8012C97C(TObj *o)
{
    int k;
    switch (o->state) {
    case 0:
        if (D_8009D2C3 & 2) {
            o->step = 4;
            o->a.p.whole = 0x842;
            o->y.p.whole = -0x38e;
            break;
        }
        o->b6a = 0;
        o->active = 1;
        o->state++;
    case 1:
        AnimAdvance(o);
        if (o->b6a) {
            D_8009CF03 = k = 1;
            o->timer = 1;
            o->b69 = 0;
            o->velV = 0;
            o->wac = 0xc;
            o->state++;
            o->anim = D_801399D8[0];
            AnimLoadDuration(o);
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = k;
            D_8009C942 = k;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->a.raw += 0x14000;
        D_800A6038.a.raw += 0x14000;
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (hit(o)) {
            o->timer = 10;
            o->b69 = 0;
            o->wac = 0xb;
            o->state++;
            o->anim = D_801399D4[0];
            AnimLoadDuration(o);
        }
        break;
    case 3:
        AnimAdvance(o);
        if (--o->timer == 0) {
            o->state = 0;
            o->step++;
        }
        break;
    }
}

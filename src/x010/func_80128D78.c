// FUNC 80128d78 720 X010
// MATCHING 80128d78 720
#include "TOBJ.H"

extern unsigned char D_8012F448, D_8012F449, D_8012F44A, D_8012F44B;
extern unsigned char D_8012F420, D_8012F421, D_8012F422, D_8012F423;
extern void *D_80132384[], *D_8013235C[];
extern unsigned short D_1F8001F8, D_1F800172, D_1F80016A, D_1F80016E;
extern int D_1F800198;
extern void ObjSetFacingToPlayer(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern int AnimAdvance(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);

static __inline__ int near(TObj *o, int w)
{
    Fix16 *d = o->d;
    if ((unsigned short)(D_1F800172 - d->p.whole + 0x2d) >= 0x5b) return 0;
    d = o->h;
    if ((unsigned short)(D_1F80016A - d->p.whole + 0x60) > w) return 0;
    return (unsigned short)(D_1F80016E - o->y.p.whole + 0x60) <= w;
}

static __inline__ void setanim(TObj *p, void *a)
{
    p->anim = a;
    AnimLoadDuration(p);
}

void func_80128D78(TObj *o)
{
    switch (o->state) {
    case 0:
        ObjSetFacingToPlayer(o);
        o->timer = 8;
        o->state++;
        if (*(unsigned short *)&o->wb4) {
            o->wac = 0x1d;
            o->box0 = D_8012F448;
            o->box1 = D_8012F449;
            o->box2 = D_8012F44A;
            o->box3 = D_8012F44B;
            setanim(o, D_80132384[0]);
        } else {
            o->wac = 0x13;
            o->box0 = D_8012F420;
            o->box1 = D_8012F421;
            o->box2 = D_8012F422;
            o->box3 = D_8012F423;
            setanim(o, D_8013235C[0]);
        }
        break;
    case 1:
        o->y.p.whole -= 2;
        if (--o->timer == 0) {
            o->active = 1;
            o->y.p.whole += 8;
            if (o->b69 == 1 || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x1a)) o->b69 = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        if (((D_1F8001F8 + D_1F800198) & 3) == 0) {
            ObjSetFacingToPlayer(o);
            if (near(o, 0xc0)) {
                o->state = 0;
                o->step++;
            }
        }
        break;
    case 3:
        ObjSetFacingToPlayer(o);
        o->state = 2;
        if (*(unsigned short *)&o->wb4) {
            o->wac = 0x1d;
            o->box0 = D_8012F448;
            o->box1 = D_8012F449;
            o->box2 = D_8012F44A;
            o->box3 = D_8012F44B;
            setanim(o, D_80132384[0]);
        } else {
            o->wac = 0x13;
            o->box0 = D_8012F420;
            o->box1 = D_8012F421;
            o->box2 = D_8012F422;
            o->box3 = D_8012F423;
            setanim(o, D_8013235C[0]);
        }
        break;
    }
}

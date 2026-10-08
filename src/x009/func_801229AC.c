// FUNC 801229ac 1372 X009
// MATCHING 801229ac 1372
#include "TOBJ.H"

extern char D_80077D18[];
extern void *D_8012EA14[], *D_8012EA1C[], *D_8012EA20[], *D_8012EA30[];
extern short D_1F80016A;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);
extern void FUN_8001faf4(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern void func_80121964(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern void ObjSetFacingToPlayer(TObj *);
extern void func_801224AC(TObj *);
extern int Rand(void);

static __inline__ int Ground(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_801229AC(TObj *o)
{
    int r;
    unsigned short f;
    int d;

    switch (o->state) {
    case 0:
        o->movetab = D_80077D18;
        o->b9c = 1;
        o->velV = -0x700;
        o->wac = 7;
        o->state++;
        o->anim = D_8012EA14[0];
        AnimLoadDuration(o);
        if (o->visible) playSFX(0x95);
        break;
    case 1:
        FUN_8001faf4(o);
        if (!(o->b9d & 2) || o->animFrame != (o->b9d & 1)) {
            f = o->animFrame;
            d = 0x10;
            if (f & 1) d = -0x10;
            func_8004065C(o, o->h->p.whole + d, o->y.p.whole, f);
        }
        func_80121964(o);
        AnimAdvance(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV >= -0x27f) {
            if (o->animFrame) {
                if (o->h->p.whole <= D_1F80016A) {
                    o->state = 3;
                    break;
                }
            } else {
                if (o->h->p.whole >= D_1F80016A) {
                    o->state = 3;
                    break;
                }
            }
        }
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->wac = 9;
            o->state++;
            o->anim = D_8012EA1C[0];
            AnimLoadDuration(o);
        }
        break;
    case 2:
        FUN_8001faf4(o);
        if (!(o->b9d & 2) || o->animFrame != (o->b9d & 1)) {
            f = o->animFrame;
            d = 0x10;
            if (f & 1) d = -0x10;
            func_8004065C(o, o->h->p.whole + d, o->y.p.whole, f);
        }
        func_80121964(o);
        AnimAdvance(o);
        o->velV += 0x30;
        if (o->velV > 0x600) o->velV = 0x600;
        o->y.raw += o->velV << 8;
        if (o->velV < 0x280) {
            if (o->animFrame) {
                if (o->h->p.whole <= D_1F80016A) {
                    o->state = 3;
                    break;
                }
            } else {
                if (o->h->p.whole >= D_1F80016A) {
                    o->state = 3;
                    break;
                }
            }
        }
        if (Ground(o)) {
            o->state = 5;
            o->b9c = 0;
            o->wac = 0xa;
            o->anim = D_8012EA20[0];
            AnimLoadDuration(o);
        }
        break;
    case 3:
        o->b9c = 2;
        o->velV = 0;
        o->b69 = 0;
        o->wac = 9;
        o->state++;
        o->anim = D_8012EA1C[0];
        AnimLoadDuration(o);
    case 4:
        AnimAdvance(o);
        o->velV += 0x30;
        if (o->velV > 0x600) o->velV = 0x600;
        o->y.raw += o->velV << 8;
        if (Ground(o)) {
            o->state = 5;
            o->b9c = 0;
            o->wac = 0xa;
            o->anim = D_8012EA20[0];
            AnimLoadDuration(o);
        }
        break;
    case 5:
        if (AnimAdvance(o)) {
            o->state++;
            if (o->visible) playSFX(0x95);
            o->wac = 0xe;
            o->anim = D_8012EA30[0];
            AnimLoadDuration(o);
        }
        break;
    case 6:
        if (AnimAdvance(o)) {
            ObjSetFacingToPlayer(o);
            o->state = 0;
            func_801224AC(o);
        }
        break;
    }
    if (o->visible) {
        r = D_1F8001F8 + D_1F800198;
        if ((r & 0x3f) == 0) {
            if (Rand() & 1) playSFX(0x95);
        } else if ((r & 0x1f) == 0) {
            playSFX(0x95);
        }
    }
    if (o->timer) o->timer--;
}

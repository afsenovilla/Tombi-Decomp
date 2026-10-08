// FUNC 80122f08 908 X009
// MATCHING 80122f08 908
#include "TOBJ.H"
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8001fab4(TObj *);
extern void FUN_8001fb20(TObj *);
extern void func_80121964(TObj *);
extern void func_801224AC(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);
extern void playSFX(int);
extern int Rand(void);
extern void *D_8012EA04[];
extern unsigned char D_800A6038;
extern unsigned char D_8009D2B1;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern char D_80077D24[], D_80077D30[];

static __inline__ short blocked(TObj *o)
{
    int d;
    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) {
        return 1;
    }
    d = (o->animFrame & 1) ? -16 : 16;
    return func_8004065C(o, o->h->p.whole + d, o->y.p.whole, o->animFrame) != 0;
}

static __inline__ short leave(TObj *o)
{
    if (o->wb4 == 3) {
        return 0;
    }
    return o->wb4 != D_8009D2B1;
}

void func_80122F08(TObj *o)
{
    short f;
    int t;

    switch (o->state) {
    case 0:
        ObjSetFacingToPlayer(o);
        o->w22 = 200;
        o->movetab = D_80077D24;
        o->wac = 3;
        o->state++;
        o->anim = D_8012EA04[0];
        AnimLoadDuration(o);
        break;
    case 1:
        AnimAdvance(o);
        if (!(D_800A6038 & 2)) {
            FUN_8001fab4(o);
        } else {
            FUN_8001fb20(o);
        }
        func_80121964(o);
        f = o->animFrame;
        ObjSetFacingToPlayer(o);
        if (f != o->animFrame) {
            o->velH = 30;
            o->state = 2;
        }
        if (blocked(o)) {
            o->state = 2;
        }
        if (o->b69 == 1 || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 16)) {
            o->b69 = 0;
        }
        if (o->w22 == 0) {
            o->w22 = 200;
            o->state = 3;
        } else {
            o->w22--;
        }
        if (leave(o)) {
            o->step = 3;
            o->state = 0;
        }
        break;
    case 2:
        if (--o->velH == -1) {
            o->state = 1;
        }
        AnimAdvance(o);
        FUN_8001fb20(o);
        if (o->b69 == 1 || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 16)) {
            o->b69 = 0;
        }
        if (leave(o)) {
            o->step = 3;
            o->state = 0;
        }
        break;
    case 3:
        o->state = 0;
        func_801224AC(o);
        break;
    case 4:
        ObjSetFacingToPlayer(o);
        o->state = 1;
        o->movetab = D_80077D30;
        o->wac = 3;
        o->anim = D_8012EA04[0];
        AnimLoadDuration(o);
        if (o->visible) {
            playSFX(0x96);
        }
        break;
    }
    if (o->visible) {
        t = D_1F8001F8 + D_1F800198;
        if (!(t & 0x7f)) {
            if (Rand() & 1) {
                playSFX(0x96);
            }
        } else if (!(t & 0x3f)) {
            playSFX(0x96);
        }
    }
    if (o->timer) {
        o->timer--;
    }
}

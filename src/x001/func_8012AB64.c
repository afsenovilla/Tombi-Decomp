// FUNC 8012ab64 1568 X001
// MATCHING 8012ab64 1568
#include "TOBJ.H"

extern char D_80077D0C[];
extern void *D_8013FC70[];
extern void *D_8013FCBC[];
extern void *D_8013FCC0[];
extern short D_1F80027E;
extern short D_1F800284;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_8001fb20(TObj *);
extern short func_8004065C(TObj *, short, short, int);
extern short FUN_800408d8(TObj *, short, short);
extern short FUN_80040278(TObj *, short, short);
extern void FUN_800e9f74(int, short, short, short);
extern void FUN_8002b920(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

static __inline__ short wall(TObj *o)
{
    int f;
    short d;

    f = o->w7a & 1;
    if (f) d = -0x10;
    else d = 0x10;
    if (func_8004065C(o, o->h->p.whole + d, o->y.p.whole, f)) {
        o->wba = 0;
        return 1;
    }
    return 0;
}

static __inline__ int land(TObj *o)
{
    if (o->b69 == 1) {
        o->wae = -1;
        o->wb2 = 0;
        o->b69 = 0;
        o->wba = 1;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        o->wba = 0;
        o->wb2 = D_1F80027E;
        o->wae = D_1F800284;
        o->d8c = (-D_1F80027E << 2) & 0xff;
        return 1;
    }
    return 0;
}

void func_8012AB64(TObj *o)
{
    switch (o->state) {
    case 0:
        if (o->w98 == 0) {
            o->active = 2;
            o->step = 2;
            o->animFrame = o->w7a;
            FUN_800e9f74(0x1f4, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            break;
        }
        o->b9c = 1;
        o->d8c = 0;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->animFrame = 1 - o->w7a;
        o->state++;
        if (o->wac == 0xd || o->wac == 0x13) o->wac = 0xf;
        else o->wac = 0xd;
        setAnim(o, D_8013FC70[o->wac]);
        break;
    case 1:
        AnimAdvance(o);
        FUN_8001fa88(o, o->w7a);
        wall(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) o->velV = 0;
        if (o->velV > 0) {
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_8001fa88(o, o->w7a);
        wall(o);
        if (land(o)) {
            o->velH = 0x180;
            o->state++;
        }
        break;
    case 3:
        FUN_8001fb20(o);
        land(o);
        if (o->w7a & 1) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        if (wall(o)) o->velH = 0;
        o->velH -= 0x20;
        if (o->velH < 0) {
            o->velH = 0;
            o->timer = 0x78;
            o->state++;
            if (o->wac == 0xd) {
                o->w50 = 0xc;
                o->active = 1;
                o->w4e = 0;
                o->wac = 0x13;
                setAnim(o, D_8013FCBC[0]);
            } else {
                FUN_8002b920(o);
            }
        }
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->timer = 0x80;
            o->state++;
            if (o->wac == 0x13) {
                o->w4e = 7;
                o->w50 = 0xe;
                o->wac = 0x14;
                o->anim = D_8013FCC0[0];
                AnimLoadDuration(o);
            }
        }
        FUN_8001fb20(o);
        land(o);
        break;
    case 5:
        if (AnimAdvance(o)) {
            o->box0 = 0xc;
            o->box1 = 0x18;
            o->active = 1;
            o->b68 = 0;
            o->b04 = 1;
            o->step = 1;
            o->state = 0;
            o->b9c = 0;
        }
        FUN_8001fb20(o);
        land(o);
        break;
    }
}

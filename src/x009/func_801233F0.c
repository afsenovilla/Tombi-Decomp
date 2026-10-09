// FUNC 801233f0 1080 X009
// MATCHING 801233f0 1080
#include "TOBJ.H"

typedef struct { short w0; } X;
extern char D_80077CDC[];
extern void *D_8012E9F8[];
extern int AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_8001fb20(TObj *);
extern void FUN_8001fe6c(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

static __inline__ short coll(TObj *o)
{
    short v;
    unsigned short f = o->w7a;

    v = 0x10;
    if (f & 1) {
        v = -0x10;
    }
    return func_8004065C(o, o->h->p.whole + v, o->y.p.whole, f);
}
#define COLL() coll(o)

static __inline__ short land(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_801233F0(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->state) {
    case 0:
        if (o->b6a != 0) {
            o->step = 3;
            break;
        }
        o->velV = -0x400;
        o->movetab = D_80077CDC;
        o->animFrame = 1 - o->w7a;
        o->d8c = 0;
        o->b9c = 1;
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        FUN_8001fa88(o, o->w7a);
        COLL();
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b69 = 0;
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->velV += 0x40;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        o->y.raw += o->velV << 8;
        FUN_8001fa88(o, o->w7a);
        COLL();
        if (land(o)) {
            o->b9c = 0;
            o->velH = 0x180;
            o->state++;
        }
        break;
    case 3:
        if (COLL()) {
            o->velH = 0;
        }
        FUN_8001fb20(o);
        if (o->b69 == 1 || FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->b69 = 0;
        }
        if (o->w7a & 1) {
            o->h->raw -= o->velH << 8;
        } else {
            o->h->raw += o->velH << 8;
        }
        o->velH -= 0x20;
        if (o->velH < 0) {
            o->velH = 0;
            o->b9c = 0;
            if (o->subtype & 0x80) {
                o->b6a = 1;
                x->w0 = 3;
                o->wac = 0x12;
                o->state++;
                o->anim = D_8012E9F8[0x12];
                FUN_8001fe6c(o);
            } else if (x->w0 == 2) {
                o->active = 1;
                o->b04 = 1;
                o->step = 6;
                o->state = 4;
            } else {
                o->active = 1;
                o->b04 = 1;
                o->step = x->w0 + 4;
                o->state = 0;
            }
        }
        break;
    case 4:
        if (AnimAdvance(o)) {
            o->active = 1;
            o->b04 = 1;
            o->step = 2;
            o->state = 0;
        }
        FUN_8001fb20(o);
        if (o->b69 == 1 || FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->b69 = 0;
        }
        break;
    }
}

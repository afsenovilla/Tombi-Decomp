// FUNC 80123e24 1800 X009
// MATCHING 80123e24 1800
#include "TOBJ.H"

typedef struct { short w0, w2, w4, w6, w8, wa; } X;
#define XS(o) ((X *)((char *)(o) + 0xb4))
#define WD2(o) (*(short *)((char *)(o) + 0xd2))

extern int D_1F8002E8;
extern unsigned char D_8009C964, D_8009C93A, D_8009C942;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern void *D_8012EA40[], *D_8012E9FC[], *D_8012EA38[];
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int Rand(void);
extern void FUN_80020490(TObj *);
extern void func_80121A58(TObj *);
extern void func_80121D2C(TObj *);
extern void func_80121E5C(TObj *);
extern void func_801220CC(TObj *);
extern void func_801225BC(TObj *);
extern void func_801229AC(TObj *);
extern void func_80122F08(TObj *);
extern void func_80123294(TObj *);
extern void func_801233F0(TObj *);
extern void func_80123828(TObj *);
extern void func_80123CE8(TObj *);

void func_80123E24(TObj *o)
{
    X *x = XS(o);
    TObj *e;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->b0a = 0;
            o->d8c = 0;
            if ((o->subtype & 0x7f) == 0) {
                x->w2 = o->d30;
                x->w4 = o->d34;
                x->w6 = o->d38;
            } else {
                x->w2 = o->h->p.whole;
                x->w4 = o->y.p.whole;
                x->w6 = o->d->p.whole;
            }
            o->b9d = 0;
            o->d3c = D_1F8002E8;
            o->w1e = 2;
            o->box0 = 0xc;
            o->box1 = 0x18;
            o->box2 = 0x10;
            o->b0d = 0;
            o->box3 = 0x20;
            o->step++;
            break;
        case 1:
            if (o->subtype & 0x80) o->b0d = 1;
            else o->b0d = 0;
            o->w08 = 0x7b52;
            o->b6a = 1;
            *(signed char *)&o->b0f = -9;
            o->anim = 0;
            o->step = 0;
            o->b04++;
            if ((o->subtype & 0x7f) == 0) {
                o->active = 7;
            } else {
                x->w8 = o->a.p.whole - 0x40;
                x->wa = o->a.p.whole + 0x40;
            }
            break;
        }
        break;
    case 1:
        if (D_8009C964 == 0x21 && D_8009C93A == 1) break;
        if (D_8009C942 && o->step != 8) {
            ObjCullRegister(o);
            break;
        }
        if (o->step) ObjCullRegister(o);
        if (o->b9e == 0) {
            switch (o->step) {
            case 0:
                func_80121A58(o);
                break;
            case 1:
                func_80121D2C(o);
                break;
            case 2:
                func_80121E5C(o);
                goto chk;
            case 3:
                func_801220CC(o);
                goto chk;
            case 4:
                func_801225BC(o);
                goto chk;
            case 5:
                func_801229AC(o);
                goto chk;
            case 6:
                func_80122F08(o);
                goto chk;
            case 7:
                func_80123294(o);
                goto chk;
            case 8:
                switch (o->state) {
                case 0:
                    o->active = 2;
                    o->wb4 = 3;
                    o->b6a = 1;
                    o->wac = 0x12;
                    o->state++;
                    o->anim = D_8012EA40[0];
                    AnimLoadDuration(o);
                    break;
                case 1:
                    if (AnimAdvance(o)) {
                        o->timer = 0x78;
                        o->state++;
                    }
                    break;
                case 2:
                    if (D_1F8001F8 & 3) o->b0d = 1;
                    else o->b0d = 0;
                    if (--o->timer == 0) {
                        o->active = 1;
                        o->b0d = 1;
                        o->step = 3;
                        o->state = 0;
                    }
                    break;
                }
                goto chk;
            }
        } else {
            if (o->b9f == 0) {
                WD2(o) = o->h->p.whole;
                o->b9f = 0xf;
            } else {
                o->h->p.whole = (short)(WD2(o) - 2) + (Rand() & 3);
                if (--o->b9f == 0) {
                    o->b9e = 0;
                    o->h->p.whole = WD2(o);
                }
            }
        }
        o->b9d = 0;
        break;
    case 2:
        switch (o->step) {
        case 0:
            func_801233F0(o);
            if (ObjCullRegister(o)) break;
        chk:
            if (((D_1F8001F8 + D_1F800198) & 7) == 0) FUN_80020490(o);
            break;
        case 1:
            func_80123828(o);
            ObjCullRegister(o);
            break;
        case 2:
            func_80123CE8(o);
            if (ObjCullRegister(o) == 0) o->b04 = 3;
            break;
        case 3:
            switch (o->state) {
            case 0:
                o->state++;
                o->wac = 1;
                o->anim = D_8012E9FC[0];
                AnimLoadDuration(o);
                break;
            case 1:
                if (AnimAdvance(o)) {
                    o->active = 1;
                    o->b04 = 1;
                    o->b9c = 0;
                    o->step = 3;
                    o->state = 0;
                }
                break;
            }
            ObjCullRegister(o);
            break;
        }
        o->b9d = 0;
        break;
    case 3:
        o->b0d = 0;
        o->subtype &= 0x7f;
        o->timer = ((Rand() & 7) << 4) + 0x384;
        o->active = 7;
        o->d8c = 0;
        *(signed char *)&o->b0f = -9;
        o->b0b = 0;
        o->b04++;
        break;
    case 4:
        if (D_8009C942) break;
        if (--o->timer != -1) break;
        o->b9c = 0;
        o->box2 = 0x10;
        o->box3 = 0x20;
        o->b0a = 0;
        o->b6a = 1;
        o->b0b = 0;
        *(signed char *)&o->b0f = -9;
        o->b04 = 0;
        o->step = 1;
        o->state = 0;
        o->substep = 0;
        o->wac = 0x10;
        o->anim = D_8012EA38[0];
        AnimLoadDuration(o);
        if ((o->subtype & 0x7f) == 0) {
            e = (TObj *)o->d90;
            o->h->p.whole = x->w2 + e->h->p.whole;
            o->y.p.whole = x->w4 + e->y.p.whole;
            o->d->p.whole = x->w6 + e->d->p.whole;
        } else {
            o->h->p.whole = x->w2;
            o->y.p.whole = x->w4;
            o->d->p.whole = x->w6;
        }
        break;
    }
}

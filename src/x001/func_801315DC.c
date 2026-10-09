// FUNC 801315dc 808 X001
// MATCHING 801315dc 808
#include "TOBJ.H"
extern void **D_8013C97C[];
extern void *D_8013E674, *D_8013E680;
extern int D_1F8002D4[];
extern unsigned char D_8009CE55[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_800202b4(TObj *);
extern TObj *FUN_800183b8(void);
extern void FUN_8003e300(int, int, Fix16 *);
extern void FUN_80018790(TObj *);

void func_801315DC(TObj *o)
{
    Fix16 v[3];
    TObj *n;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 8;
        o->b0d = 0x80;
        o->anim = D_8013C97C[o->subtype][0];
        o->d3c = D_1F8002D4[0];
        o->box0 = 0xb;
        o->box1 = 0x16;
        o->box2 = 0x10;
        o->box3 = 0x20;
        AnimLoadDuration(o);
        if (D_8009CE55[0] < 3) break;
        o->subtype = 1;
        o->w1e = 7;
        o->b0d = 0;
        v[0].p.whole = o->a.p.whole;
        v[1].p.whole = o->y.p.whole;
        v[2].p.whole = o->b.p.whole;
        if (D_8009CE55[0] == 3) FUN_8003e300(0xf, 0, v);
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        if (!FUN_800202b4(o)) break;
        AnimAdvance(o);
        if (D_8009CE55[0] != 2) break;
        n = FUN_800183b8();
        if (n) {
            n->active = 1;
            n->type = 0xf;
            n->subtype = 1;
            n->a = o->a;
            n->y = o->y;
            n->b = o->b;
            n->w1e = 7;
            n->b0d = 0;
            n->anim = D_8013C97C[n->subtype][0];
            n->d3c = D_1F8002D4[0];
            n->box0 = 0xb;
            n->box1 = 0x16;
            n->box2 = 0x10;
            n->box3 = 0x20;
            n->b04 = 2;
            n->step = 0;
            n->state = 0;
        }
        D_8009CE55[0] = 3;
        o->w1e = 7;
        o->b0d = 0x80;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        break;
    case 2:
        if (!FUN_800202b4(o)) break;
        switch (o->subtype) {
        case 0:
            switch (o->state) {
            case 0:
                o->anim = D_8013E674;
                AnimLoadDuration(o);
                o->state++;
            case 1:
                if (AnimAdvance(o)) o->b04 = 3;
                break;
            }
            break;
        case 1:
            if (o->state == 0) {
                o->anim = D_8013E680;
                AnimLoadDuration(o);
                o->active = 2;
                o->y.p.whole += 8;
                o->state++;
            }
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

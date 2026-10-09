// FUNC 8012b2d4 2116 X001
/* score 150: full logic. Open: case 0 (step 0) loads the box constants 0xc/0x18 early into a0/v1 (ours
   loads them at the stores); the b9f shake branch computes wd2-2 as li 0xfffe+addu; the despawn range test loads
   D_800A457C/E in a different order. Tried: box setter inlines at several positions, hill-climbed case 0. */
#include "TOBJ.H"

extern int D_1F8002D0[];
extern unsigned char D_8009CEAE, D_8009C942;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_800A4582, D_800A457E;
extern unsigned short D_800A457C;
extern int D_1F80018C, D_1F800190;
extern void *D_8013FC90[], *D_8013FC94[], *D_8013FCB0[];
extern short D_1F80027E;
extern short D_1F800284;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern int Rand(void);
extern int func_800203DC(TObj *);
extern void FUN_80026bfc(int, int);
extern short FUN_80040278(TObj *, short, short);
extern void func_801282D0(TObj *);
extern void func_801285E4(TObj *);
extern void func_80128A2C(TObj *);
extern void func_80129588(TObj *);
extern void func_80129B2C(TObj *);
extern void func_80129DAC(TObj *);
extern void func_8012A290(TObj *);
extern void func_8012A5CC(TObj *);
extern void func_8012A794(TObj *);
extern void func_8012AB64(TObj *);
extern void func_8012B184(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
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

typedef struct { TObj t; char pad[0xd2 - 0xc0]; unsigned short wd2; } BX;
#define o (&b->t)
void func_8012B2D4(BX *b)
{
    short *q = &o->wb4;

    switch (o->b04) {
    case 0:
        if (o->step == 0) {
            q[3] = 0;
            q[0] = o->h->p.whole;
            q[1] = o->y.p.whole;
            q[2] = o->d->p.whole;
            o->w22 = 0;
            o->b0d = 0;
            o->b68 = 0;
            o->ba7 = 0;
            o->d3c = D_1F8002D0[0];
            q[4] = 0;
            o->w4e = 7;
            o->w50 = 0xe;
            o->box2 = 0x10;
            o->w4a = 0x18;
            o->w1e = 1;
            o->box1 = 0x18;
            o->w48 = 0xc;
            o->box0 = 0xc;
            o->box3 = 0x20;
            o->step++;
        } else {
            o->active = 1;
            *(signed char *)&o->b0f = -9;
            o->b6a = 0;
            o->b9e = 0;
            o->b69 = 0;
            o->wb0 = 0;
            o->d8c = 0;
            o->d88 = 0;
            o->d84 = 0;
            o->w22 = 0;
            o->step = 0;
            o->w98 = 3;
            o->w9a = 3;
            o->anim = 0;
            o->b04++;
        }
        break;
    case 1:
        if (o->b0c == 1 && D_8009CEAE == 1) {
            o->b04 = 5;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            break;
        }
        if (D_8009C942) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        if (o->b9e == 0) {
            switch (o->step) {
            case 0: func_801282D0(o); break;
            case 1: func_801285E4(o); break;
            case 2: func_80128A2C(o); break;
            case 3: func_80129588(o); break;
            case 4: func_80129B2C(o); break;
            case 5: func_80129DAC(o); break;
            case 6: func_8012A290(o); break;
            case 7: func_8012A5CC(o); break;
            case 8: func_8012A794(o); break;
            }
            if (((D_1F8001F8 + D_1F800198) & 7) == 0) {
                if (o->y.p.whole > D_800A4582 + 0xa0 ||
                    D_800A457E + 0xb0 < (unsigned short)(o->h->p.whole + 0xaa - D_800A457C)) {
                    o->b04 = 3;
                    o->active = 2;
                    o->visible = 0;
                }
            }
        } else if (o->b9f == 0) {
            o->b9f = 0xf;
            b->wd2 = o->h->p.whole;
        } else {
            { int r = Rand() & 3; o->h->p.whole = b->wd2 - 2 + r; }
            if (--o->b9f == 0) {
                o->b9e = 0;
                o->h->p.whole = b->wd2;
            }
        }
        o->b9d = 0;
        break;
    case 2:
        switch (o->step) {
        case 0:
            func_8012AB64(o);
            ObjCullRegister(o);
            break;
        case 1:
            switch (o->state) {
            case 0:
                o->b9c = 0;
                FUN_80026bfc(1, 6);
                o->box2 = 4;
                o->box3 = 0x14;
                o->wac = 8;
                o->state++;
                o->anim = D_8013FC90[0];
                AnimLoadDuration(o);
                break;
            case 1:
                break;
            case 2:
                o->state++;
                o->wac = 9;
                o->anim = D_8013FC94[0];
                AnimLoadDuration(o);
                break;
            }
            ObjCullRegister(o);
            break;
        case 2:
            func_8012B184(o);
            if (!ObjCullRegister(o)) o->b04 = 3;
            break;
        }
        break;
    case 3:
        *(signed char *)&o->b0f = -9;
        o->b0b = 0;
        o->h->p.whole = q[0];
        o->y.p.whole = q[1];
        o->d->p.whole = q[2];
        o->b04++;
        break;
    case 4:
        if (o->b0c == 1 && D_8009CEAE == 1) {
            o->b04 = 5;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            break;
        }
        if (((D_1F8001F8 + D_1F800198) & 0xf) == 0 && !func_800203DC(o)) {
            o->box2 = 0x10;
            o->box3 = 0x20;
            o->b04 = 0;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    case 5:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->h->p.whole = 0x304;
            o->y.p.whole = -0x1da;
            o->d->p.whole = q[2];
            o->active = 3;
            *(signed char *)&o->b0f = -9;
            o->box2 = 0x10;
            o->box3 = 0x20;
            o->b9e = 0;
            o->b6a = 0;
            o->b69 = 0;
            o->wb0 = 0;
            o->d8c = 0;
            o->step++;
            q[4] = 0;
            o->wac = 0x10;
            setAnim(o, D_8013FCB0[0]);
            break;
        case 1:
            AnimAdvance(o);
            if (D_8009CEAE == 2) o->step++;
            break;
        case 2:
            AnimAdvance(o);
            if (D_8009CEAE == 3) o->step++;
            D_1F80018C = o->h->raw;
            D_1F800190 = o->y.raw;
            break;
        case 3:
            AnimAdvance(o);
            o->y.p.whole -= 4;
            if (o->y.p.whole < -0x3ac) {
                o->velV = -0x400;
                o->step++;
            }
            o->d8c = (o->d8c - 6) & 0xff;
            D_1F80018C = o->h->raw;
            D_1F800190 = o->y.raw;
            break;
        case 4:
            AnimAdvance(o);
            o->velV += 0x10;
            o->y.raw += o->velV << 8;
            if (o->velV > 0) {
                o->b69 = 0;
                o->step++;
            }
            o->d8c = (o->d8c - 6) & 0xff;
            D_1F80018C = o->h->raw;
            D_1F800190 = o->y.raw;
            break;
        case 5:
            AnimAdvance(o);
            o->velV += 0x10;
            if (o->velV > 0x400) o->velV = 0x400;
            o->y.raw += o->velV << 8;
            o->d8c = (o->d8c - 6) & 0xff;
            D_1F80018C = o->h->raw;
            D_1F800190 = o->y.raw;
            if (land(o)) o->step++;
            break;
        case 6:
            AnimAdvance(o);
            break;
        }
        break;
    }
}

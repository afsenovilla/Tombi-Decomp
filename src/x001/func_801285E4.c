// FUNC 801285e4 1096 X001
// MATCHING 801285e4 1096
#include "TOBJ.H"
typedef struct { short w0, w2, w4, w6, w8; } S;
extern unsigned char D_8013C7C8[];
extern unsigned char D_8013C7D8[];
extern char D_80077CE8[];
extern char D_80077CF4[];
extern void *D_8013FC74[];
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_1F80027E;
extern short D_1F800284;
extern short D_8007A5F0[];
extern short D_8007A1F0[];
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void SfxPlay(int);
extern void FUN_8001fab4(TObj *);
extern short FUN_8004065c(TObj *, short, short, int);
extern short FUN_80040278(TObj *, short, short);
extern void func_80128110(TObj *);

static __inline__ short chk(TObj *o)
{
    S *q = (S *)&o->wb4;
    int f;
    short d;

    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) {
        o->wba = 1;
        return 1;
    }
    f = o->animFrame & 1;
    if ((unsigned short)f) d = -0x10;
    else d = 0x10;
    if (FUN_8004065c(o, o->h->p.whole + d, o->y.p.whole, f)) {
        q->w6 = 0;
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

void func_801285E4(TObj *o)
{
    S *p = (S *)&o->wb4;

    switch (o->state) {
    case 0:
        o->b9c = 0;
        if (D_8013C7C8[Rand() & 0xf])
            o->movetab = D_80077CE8;
        else
            o->movetab = D_80077CF4;
        if (D_8013C7D8[Rand() & 7]) {
            o->timer = 300;
            o->ba7 = 0;
        } else {
            o->timer = 200;
            o->ba7 = 1;
        }
        o->wac = 1;
        o->anim = D_8013FC74[0];
        AnimLoadDuration(o);
        o->state++;
        p->w8 = 0;
        break;
    case 1:
        if (--o->timer == -1) {
            o->timer = 0x80;
            o->state++;
            break;
        }
        AnimAdvance(o);
        if (o->visible && ((D_1F8001F8 + D_1F800198) & 0xf) == 0)
            SfxPlay(0x5e);
        if (p->w6) {
            int t = o->d8c;
            unsigned short a;
            short s = *(short *)o->movetab;
            a = t;
            if (o->animFrame) a = t + 0x80;
            a &= 0xff;
            o->h->raw += (D_8007A5F0[a] * s) >> 4;
            o->y.raw += (D_8007A1F0[a] * s) >> 4;
            o->y.p.whole += 2;
            if (chk(o) && p->w6 == 0) {
                p->w8 = 0;
                o->step = 2;
                o->state = 0;
                o->wb0 = 0;
                break;
            }
        } else {
            o->b9c = 1;
            FUN_8001fab4(o);
            if (chk(o)) {
                p->w8 = 0;
                o->step = 2;
                o->state = 0;
                o->wb0 = 0;
                break;
            }
        }
        if (!land(o)) {
            p->w8 = 0;
            o->step = 2;
            o->state = 0;
            o->wb0 = 1;
        }
        break;
    case 2:
        func_80128110(o);
        o->state = 0;
        o->ba7 = 0;
        break;
    }
}

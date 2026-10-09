// FUNC 80127d48 792 X003
// MATCHING 80127d48 792
#include "TOBJ.H"

extern unsigned char D_80135CB0[];
extern unsigned char D_80135D5B[];
extern void *D_80139500[];
extern unsigned short D_1F80027E;
extern unsigned char D_800A603E[], D_800A60E4[];
extern void FUN_80026bfc(int, int);
extern void AnimLoadDuration(TObj *);
extern short FUN_80040278(TObj *, short, short);

static __inline__ void setAnimN(TObj *o, short n)
{
    unsigned char *p;
    o->wac = n;
    o->anim = D_80139500[n];
    p = &D_80135CB0[o->wac * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    AnimLoadDuration(o);
}

static __inline__ void fall(TObj *o)
{
    unsigned int t;
    o->y.p.whole++;
    if (o->b69 == 1) {
        o->b69 = 0;
        return;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0xe)) {
        if (o->wac >= 0xc) {
            o->d8c = 0;
        } else {
            t = ((D_1F80027E << 2) + o->d8c) & 0xff;
            if (t) {
                if (t < 0x80) o->d8c--;
                else o->d8c++;
                o->d8c = *(unsigned char *)&o->d8c;
            }
        }
    }
}

void func_80127D48(TObj *o)
{
    short *w = &o->wb4;
    unsigned char t;

    switch (o->state) {
    case 0:
        o->b9c = 0;
        FUN_80026bfc(2, 6);
        if (w[1] == 0 || w[1] == 5) {
            o->state = 2;
            o->timer = 2;
            setAnimN(o, 9);
        } else {
            o->timer = 7;
            o->state++;
            w[1]++;
            setAnimN(o, D_80135D5B[w[1]]);
        }
        break;
    case 1:
        if (--o->timer == -1) o->state = 0;
        fall(o);
        break;
    case 2:
        if (--o->timer == -1) o->state++;
        fall(o);
        break;
    case 3:
        {
        unsigned char t;
        unsigned char *p;
        D_800A603E[0] = 2;
        D_800A60E4[0] = 3;
        /* debt: volatile load + volatile store create the anti-dependence that keeps lbu before sh wac */
        t = *(volatile unsigned char *)&o->state;
        *(volatile short *)&o->wac = 10;
        o->state = t + 1;
        o->anim = D_80139500[10];
        p = &D_80135CB0[o->wac * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        AnimLoadDuration(o);
        }
        break;
    case 4:
        break;
    }
}

// FUNC 801259ac 844 X003
// MATCHING 801259ac 844
#include "TOBJ.H"
extern unsigned short D_1F80027E;
extern unsigned short D_1F800176, D_1F800186;
extern short FUN_80040278(TObj *, int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int Rand(void);
extern void *D_80139500[];
extern void *D_80139570;
extern unsigned char D_80135CB0[];
extern unsigned char D_80135D54[];

static __inline__ void fall(TObj *o)
{
    short yy;
    unsigned int v;

    yy = o->y.p.whole;
    o->y.p.whole = yy + 1;
    if (o->b69 == 1) {
        o->b69 = 0;
    } else if (FUN_80040278(o, o->h->p.whole, (short)(yy + 0xf))) {
        if (o->wac >= 0xc) {
            o->d8c = 0;
        } else {
            v = ((D_1F80027E << 2) + o->d8c) & 0xff;
            if (v != 0) {
                if (v < 0x80) o->d8c = o->d8c - 1;
                else o->d8c = o->d8c + 1;
                o->d8c = *(unsigned char *)&o->d8c;
            }
        }
    }
}

void func_801259AC(TObj *o)
{
    short *w = &o->wb4;

    switch (o->substep) {
    case 0: {
        unsigned char *p;
        o->substep++;
        o->timer = D_80135D54[Rand() & 7];
        if (o->wb4 != 0) o->wac = 7;
        else o->wac = 0x1e;
        o->anim = D_80139500[o->wac];
        p = &D_80135CB0[o->wac * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        AnimLoadDuration(o);
        break;
    }
    case 1:
        fall(o);
        if (o->visible) {
            if (--o->timer == -1) {
                o->state = 2;
                o->substep = 0;
            }
        } else if (*w == 0 || (unsigned short)(o->h->p.whole - D_1F800176 + 0x70) >= 0x220 ||
                   (unsigned short)(D_1F800186 - o->y.p.whole + 0x70) >= 0x1c0) {
            o->state = 0;
            o->substep = 0;
        }
        break;
    case 2: {
        unsigned char *p;
        o->substep++;
        o->wac = 0x1c;
        o->anim = D_80139570;
        p = &D_80135CB0[o->wac * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        AnimLoadDuration(o);
        break;
    }
    case 3:
        if (AnimAdvance(o)) o->substep = 0;
        fall(o);
        break;
    }
}

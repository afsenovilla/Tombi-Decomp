// FUNC 8002f4e0 1152 MAIN0
// wip: solo falla que gcc reasocia (D + (r-8)) como ((D-8) + r) tras MulNegSinScaled
#include "TOBJ.H"
typedef struct { short w0; short w2; } P;
typedef struct { void **tab; int a; int b; } AT;
extern AT D_80079D58[];
extern unsigned short D_1F8001F8;
extern unsigned char D_800A6104;
extern TObj D_800A6038;
extern short D_800A6118;
extern P *D_800A6078, *D_800A607C;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_80025f40(int, int, int, int);
extern short MulCos(int, int);
extern int MulNegSinScaled(int, int);

void func_8002F4E0(TObj *o)
{
    unsigned char v;
    int t;
    switch (o->state) {
    case 0:
        switch (o->b0c) {
        case 5:
            o->d84 = 0;
            o->anim = D_80079D58[o->subtype].tab[5];
            break;
        case 6:
            o->d84 = 0x56;
            o->anim = D_80079D58[o->subtype].tab[6];
            break;
        case 7:
            o->d84 = 0xab;
            o->anim = D_80079D58[o->subtype].tab[7];
            break;
        case 8:
            o->d84 = 0x2c;
            o->anim = D_80079D58[o->subtype].tab[5];
            break;
        case 9:
            o->d84 = 0x7f;
            o->anim = D_80079D58[o->subtype].tab[6];
            break;
        case 10:
            o->d84 = 0xd4;
            o->anim = D_80079D58[o->subtype].tab[7];
            break;
        }
        AnimLoadDuration(o);
        o->d88 = 0;
        o->timer = 0xb4;
        o->state++;
    case 1:
        if (o->b0c == 0 && (D_1F8001F8 & 3) == 0)
            FUN_80025f40(0, 0, 0xff, 2);
        o->timer--;
        o->visible = D_800A6038.visible;
        if (o->timer >= 0x3d) {
            if (o->b6b < 0x7f)
                v = o->b6b + 8;
            else
                v = o->b6b;
        } else if (o->timer < 0) {
            if (D_800A6118 > 0) D_800A6038.active = 3; else D_800A6038.active = 1;
            D_800A6104 = 3;
            o->b6b = 0;
            o->b04 = 2;
            goto anim;
        } else {
            if (o->b6b)
                v = o->b6b - 2;
            else
                v = 0;
        }
        o->b6b = v;
    anim:
        AnimAdvance(o);
        switch (o->b0c) {
        case 0:
            o->h->p.whole = D_800A6038.h->p.whole;
            o->y.p.whole = D_800A6038.y.p.whole - 8;
            o->d->p.whole = D_800A6038.d->p.whole;
            break;
        case 5:
            o->h->p.whole = D_800A6038.h->p.whole + MulCos(*(short *)&o->d84, 8);
            o->y.p.whole = D_800A6038.y.p.whole + (MulNegSinScaled(*(short *)&o->d84, 28) - 8);
            o->d->p.whole = D_800A6038.d->p.whole + (MulNegSinScaled(*(short *)&o->d84, 4) - 4);
            break;
        case 6:
            o->h->p.whole = D_800A6038.h->p.whole + MulCos(*(short *)&o->d84, 16);
            o->y.p.whole = D_800A6038.y.p.whole + (MulNegSinScaled(*(short *)&o->d84, 16) - 8);
            o->d->p.whole = D_800A6038.d->p.whole + (MulNegSinScaled(*(short *)&o->d84, 4) - 4);
            break;
        case 7:
            o->h->p.whole = D_800A6038.h->p.whole + MulCos(*(short *)&o->d84, 24);
            o->y.p.whole = D_800A6038.y.p.whole + (MulNegSinScaled(*(short *)&o->d84, 8) - 8);
            o->d->p.whole = D_800A6038.d->p.whole + (MulNegSinScaled(*(short *)&o->d84, 4) - 4);
            break;
        case 8:
            o->h->p.whole = D_800A6038.h->p.whole + MulCos(*(short *)&o->d84, 4);
            o->y.p.whole = D_800A6038.y.p.whole + (MulNegSinScaled(*(short *)&o->d84, 24) - 8);
            o->d->p.whole = D_800A6038.d->p.whole + (MulNegSinScaled(*(short *)&o->d84, 4) - 4);
            break;
        case 9:
            o->h->p.whole = D_800A6038.h->p.whole + MulCos(*(short *)&o->d84, 20);
            o->y.p.whole = D_800A6038.y.p.whole + (MulNegSinScaled(*(short *)&o->d84, 20) - 8);
            o->d->p.whole = D_800A6038.d->p.whole + (MulNegSinScaled(*(short *)&o->d84, 4) - 4);
            break;
        case 10:
            o->h->p.whole = D_800A6038.h->p.whole + MulCos(*(short *)&o->d84, 28);
            o->y.p.whole = D_800A6038.y.p.whole + (MulNegSinScaled(*(short *)&o->d84, 12) - 8);
            o->d->p.whole = D_800A6038.d->p.whole + (MulNegSinScaled(*(short *)&o->d84, 4) - 4);
            break;
        }
        o->d84 = (o->d84 + 0x10) & 0xff;
        break;
    }
}

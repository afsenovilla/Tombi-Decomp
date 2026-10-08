// FUNC 80132174 1856 X000
/* score 24: only the b04==0 init tail differs (game loads DAT_1f8002dc and o->b04 right after the AnimLoadDuration call,
   stores stay in source order); tried statement permutations (random + positional), locals for b04/h/DAT_1f8002dc, E* alias. */
#include "TOBJ.H"
typedef struct { TObj o; unsigned short c0, c2, c4, c6, c8, ca, cc, ce, d0; short d2; } E;
#define EX(o) ((E *)(o))
extern unsigned char DAT_8009c942;
extern Fix16 *DAT_800a607c;
extern int DAT_1f8002dc;
extern int DAT_1f800350;
extern char DAT_800d7e28[];
extern char DAT_80077cdc[];
extern char DAT_80077d90[];
extern void *PTR_8013ad38[];
extern void *PTR_8013ad3c[];
extern void *PTR_8013ad44[];
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern unsigned FUN_8001f9e0(void);
extern int FUN_800202b4(TObj *);
extern short FUN_8005e420(int, int);
extern void FUN_80018790(TObj *);
extern void FUN_801307a0(TObj *);
extern void FUN_801308d8(TObj *);
extern void FUN_80130a38(TObj *);
extern void FUN_80130bb0(TObj *);
extern void FUN_801317e4(TObj *);
extern void FUN_801319d0(TObj *);
extern void FUN_801320a8(TObj *);
extern short FUN_800411cc(TObj *, short, short);
extern void FUN_8001fa20(TObj *, unsigned short);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_8005f290(short *, int, int, int);
extern void FUN_800371c0(int, int, char *, int);
extern void FUN_800174fc(char *, int, int, int, int);

static __inline__ void draw(int a, int b)
{
    short r[4];
    r[0] = b * 0x20 + 0x1c0;
    r[1] = 0xa0;
    r[2] = 0x20;
    r[3] = 0x60;
    FUN_8005f290(r, 0, 0, 0);
    FUN_800371c0(DAT_1f800350, (short)a, DAT_800d7e28, 0xa001c0);
    FUN_800174fc(DAT_800d7e28, (short)(b * 0x20 + 0x1c1), 0xa0, 0xe0, 0x1f0);
}

void FUN_80132174(TObj *o)
{
    int d;
    int t;
    unsigned char b;
    Fix16 *h;

    switch (o->b04) {
    case 0:
        o->box0 = 0x14;
        o->box1 = 0x28;
        o->box2 = 10;
        o->box3 = 0x1e;
        o->active = 1;
        o->w1e = 7;
        o->b0d = 1;
        o->w08 = FUN_8005e420(0xe0, 0x1f0);
        o->b0a = 6;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->wac = 2;
        o->anim = PTR_8013ad38[0];
        FUN_8001fe6c(o);
        t = DAT_1f8002dc;
        b = o->b04;
        o->movetab = DAT_80077cdc;
        o->timer = 1;
        *(signed char *)&o->b0f = -9;
        EX(o)->c4 = 0xffff;
        h = o->h;
        o->b04 = b + 1;
        o->b6a = 0;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
        o->wb4 = 0;
        EX(o)->c6 = 0;
        EX(o)->ca = 0;
        o->d3c = t;
        EX(o)->c0 = o->y.p.whole;
        *(short *)&o->bbe = h->p.whole;
        o->d64 = 0x1000;
        EX(o)->c2 = o->d->p.whole;
        break;
    case 1:
        if (DAT_8009c942 != 0) {
            FUN_800202b4(o);
            break;
        }
        if (o->b9e == 0) {
            switch (o->step) {
            case 0:
                switch (o->state) {
                case 0:
                    FUN_801307a0(o);
                    break;
                case 1:
                    FUN_801308d8(o);
                    break;
                case 2:
                    FUN_80130a38(o);
                    break;
                case 3:
                    FUN_80130bb0(o);
                    break;
                case 4:
                    FUN_801317e4(o);
                    break;
                }
                break;
            case 1:
                FUN_801319d0(o);
                break;
            }
        } else if (o->b9f == 0) {
            short t = o->h->p.whole;
            o->b9f = 0xf;
            EX(o)->d2 = t;
        } else {
            { int r = FUN_8001f9e0(); o->h->p.whole = (short)(EX(o)->d2 - 2) + (r & 3); }
            if (--o->b9f == 0) {
                o->b9e = 0;
                o->h->p.whole = EX(o)->d2;
            }
        }
        if (FUN_800202b4(o))
            EX(o)->ca = 1;
        else
            EX(o)->ca = 0;
        if (DAT_800a607c->p.whole >= o->d->p.whole)
            o->w08 = FUN_8005e420(0xe0, 0x1f0);
        else
            o->w08 = FUN_8005e420(0xe0, 0x1f1);
        break;
    case 2:
        if (DAT_8009c942 != 0) {
            FUN_800202b4(o);
            break;
        }
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0:
                o->timer = 0x78;
                o->w22 = 1;
                o->wac = 3;
                o->anim = PTR_8013ad3c[0];
                FUN_8001fe6c(o);
                o->movetab = DAT_80077d90;
                o->state++;
                break;
            case 1:
                if (--o->timer == 0) {
                    o->wac = 2;
                    o->anim = PTR_8013ad38[0];
                    FUN_8001fe6c(o);
                    o->state++;
                }
                d = 8;
                if (o->animFrame)
                    d = -8;
                if (FUN_800411cc(o, o->h->p.whole + d, o->y.p.whole + 0x18) == 0) {
                    FUN_8001fa20(o, 0);
                    o->w22++;
                }
                break;
            case 2:
                if (*(unsigned short *)&o->wb4 != 0) {
                    if (--o->w22 == 0) {
                        o->active = 1;
                        o->b04 = 1;
                        o->step = 0;
                        o->state = 3;
                        o->substep = 0;
                    }
                    FUN_8001fa20(o, 1);
                } else {
                    if (--o->w22 == 0) {
                        o->active = 1;
                        o->b04 = 1;
                        o->step = 0;
                        o->state = 0;
                        o->substep = 0;
                    }
                }
                break;
            }
            if (FUN_800202b4(o)) {
                FUN_8001fec0(o);
                EX(o)->ca = 1;
            } else
                EX(o)->ca = 0;
            break;
        case 1:
            FUN_801320a8(o);
            if (FUN_800202b4(o)) {
                FUN_8001fec0(o);
                EX(o)->ca = 1;
            } else
                EX(o)->ca = 0;
            break;
        case 2:
            switch (o->state) {
            case 0:
                o->b0b = 1;
                o->b0f = 4;
                o->velV = -0x400;
                o->movetab = DAT_80077cdc;
                o->wac = 5;
                o->state++;
                o->anim = PTR_8013ad44[0];
                FUN_8001fe6c(o);
                break;
            case 1:
                FUN_8001fa88(o, 1 - o->animFrame);
                if ((o->velV += 0x40) > 0x400)
                    o->velV = 0x400;
                o->y.raw += o->velV << 8;
                break;
            }
            if (o->animFrame & 1)
                o->d8c = (o->d8c + 0x14) & 0xff;
            else
                o->d8c = (o->d8c - 0x14) & 0xff;
            if (FUN_800202b4(o) == 0)
                o->b04 = 3;
            FUN_8001fec0(o);
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
    if (EX(o)->ca == 1 && EX(o)->c4 != *(unsigned short *)o->anim) {
        EX(o)->c4 = *(unsigned short *)o->anim;
        EX(o)->c6 ^= 1;
        draw(EX(o)->c4, EX(o)->c6);
    }
}

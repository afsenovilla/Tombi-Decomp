// FUNC 80129dac 1252 X001
// MATCHING 80129dac 1252
#include "TOBJ.H"

extern unsigned char D_8013C7C8[];
extern char D_80077D00[], D_80077CF4[], D_80077CDC[];
extern void *D_8013FC74[];
extern short D_1F80016A, D_1F80016E;
extern unsigned short D_1F80027E;
extern unsigned int FUN_8001f9e0(void);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001faf4(TObj *);
extern short FUN_800408d8(TObj *, short, short);
extern short FUN_8004065c(TObj *, short, short, short);

void func_80129DAC(TObj *o)
{
    short *q = &o->wb4;
    short *r;
    int f, n;
    int t;
    int af;
    short d;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->b9c = 1;
        if (o->w22 != 0) {
            o->movetab = D_80077D00;
            o->timer = 500;
        } else {
            if (D_8013C7C8[FUN_8001f9e0() & 0xf])
                o->movetab = D_80077CF4;
            else
                o->movetab = D_80077D00;
            if (D_8013C7C8[FUN_8001f9e0() & 0xf])
                o->timer = 0x78;
            else
                o->timer = 0x3c;
        }
        o->state++;
        if (o->w22)
            o->animFrame = o->h->p.whole > o->wb4;
        else
            o->animFrame = D_1F80016A < o->h->p.whole;
        break;
    case 1:
        FUN_8001fec0(o);
        FUN_8001faf4(o);
        if (o->w22 == 0) {
            if (o->animFrame) {
                if (!(o->h->p.whole > D_1F80016A)) {
                    o->state = 0;
                    o->step = 4;
                    break;
                }
            } else {
                if (!(o->h->p.whole < D_1F80016A)) {
                    o->state = 0;
                    o->step = 4;
                    break;
                }
            }
        } else {
            t = o->h->p.whole > o->wb4;
            if (t != o->animFrame) {
            o->state = 0;
            o->step = 4;
            break;
            }
        }
        if (o->b69 == 4 || FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
            o->velV = 0;
            if (o->b69 == 4) {
                q[3] = 1;
                o->wb2 = 0x80;
            } else {
                o->wb2 = (-D_1F80027E * 4 + 0x80) & 0xff;
                q[3] = 0;
            }
            o->step = 3;
            n = o->d8c;
            if ((unsigned int)((n - (unsigned short)o->wb2) & 0xff) > 0x80)
                o->state = 8;
            else
                o->state = 9;
            break;
        }
        if (--o->timer == -1) {
            o->timer = 0x80;
            o->state = 0;
            o->step = 4;
            break;
        }
        r = &o->wb4;
        if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) {
            o->wba = 1;
            f = 1;
        } else {
            af = o->animFrame & 1;
            if ((unsigned short)af)
                d = -0x10;
            else
                d = 0x10;
            if (FUN_8004065c(o, o->h->p.whole + d, o->y.p.whole, af)) {
                r[3] = 0;
                f = 1;
            } else
                f = 0;
        }
        if (f) {
            if (o->animFrame)
                o->state = 3;
            else
                o->state = 2;
        }
        break;
    case 2:
        o->d8c = (o->d8c + 2) & 0xff;
        if (o->d8c > 0x40) {
            o->d8c = 0x40;
            o->step = 2;
            o->state = 2;
            o->b9c = 0;
            o->wbc = 0;
            if (o->w22 ? (o->y.p.whole > o->wb6) : (o->y.p.whole > D_1F80016E))
                o->wb0 = 0;
            else
                o->wb0 = 1;
            o->movetab = D_80077CDC;
            o->wac = 1;
            o->anim = D_8013FC74[0];
            FUN_8001fe6c(o);
        }
        break;
    case 3:
        o->d8c = (o->d8c - 2) & 0xff;
        if (o->d8c < 0xc0) {
            o->d8c = 0xc0;
            o->step = 2;
            o->state = 2;
            o->b9c = 0;
            if (o->w22 ? (o->y.p.whole > o->wb6) : (o->y.p.whole > D_1F80016E))
                o->wb0 = 0;
            else
                o->wb0 = 1;
            q[4] = 0;
            o->movetab = D_80077CDC;
            o->wac = 1;
            o->anim = D_8013FC74[0];
            FUN_8001fe6c(o);
        }
        break;
    }
}

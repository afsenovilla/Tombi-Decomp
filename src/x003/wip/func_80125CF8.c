// FUNC 80125cf8 1588 X003
/* score ~231: control flow decoded (sibling of func_801259AC: slope/fall inline, off-screen check). Left: the
   near() range checks (operand/register order, sltiu vs slti), shared SET tails between substeps and register
   choice in the timer setups. */
#include "TOBJ.H"
extern unsigned short D_1F80027E;
extern unsigned short D_1F800176, D_1F800186;
extern unsigned short D_1F80016A, D_1F80016E, D_1F800172;
extern short FUN_80040278(TObj *, int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int Rand(void);
extern short isObjectBelowGround(TObj *);
extern void FUN_8001fab4(TObj *);
extern int func_801256F4(TObj *, short);
extern void *D_80139500[];
extern unsigned char D_80135CB0[];
extern unsigned char D_80135D54[], D_80135D4C[];
extern unsigned short D_80135D2C[];
extern char D_80077D24[], D_80077CE8[];

#define SET() \
    { unsigned char *p; \
    o->anim = D_80139500[o->wac]; \
    p = &D_80135CB0[o->wac * 4]; \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p++; \
    o->box3 = *p++; \
    AnimLoadDuration(o); }

static __inline__ void slope(TObj *o, short y)
{
    unsigned int v;

    if (o->b69 == 1) {
        o->b69 = 0;
    } else if (FUN_80040278(o, o->h->p.whole, y)) {
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

static __inline__ short near(TObj *o)
{
    int a = (unsigned short)(-o->h->p.whole + D_1F80016A + 0x60);
    if ((unsigned short)(-o->d->p.whole + D_1F800172 + 0x2d) >= 0x5b) return 0;
    if (a > 0xc0) return 0;
    return (unsigned short)(D_1F80016E - o->y.p.whole + 0x40) <= 0x80;
}

void func_80125CF8(TObj *o)
{
    short *w = &o->wb4;
    int t;

    switch (o->substep) {
    case 0:
        if (isObjectBelowGround(o) != o->animFrame) {
            o->substep = 3;
            if (*w) {
                o->movetab = D_80077D24;
                o->wac = 0xf;
            } else {
                o->wac = 0x1e;
                o->movetab = D_80077CE8;
            }
            SET();
        } else {
            o->substep++;
        }
        break;
    case 1:
        o->substep++;
        if (*w) {
            t = D_80135D54[Rand() & 7];
            o->movetab = D_80077D24;
            o->wac = 8;
        } else {
            t = D_80135D4C[Rand() & 7];
            o->wac = 1;
            o->movetab = D_80077CE8;
        }
        o->timer = t;
        SET();
        break;
    case 2:
        if (--o->timer == -1) {
            if (near(o) && (Rand() & 1)) {
                o->state++;
                o->substep = 0;
                break;
            }
            o->substep = 5;
            if (*w) {
                t = D_80135D4C[Rand() & 7];
                o->wac = 7;
                o->movetab = D_80077D24;
            } else {
                t = D_80135D2C[Rand() & 7];
                o->wac = 0;
                o->movetab = D_80077CE8;
            }
            o->timer = t;
            SET();
            break;
        }
        FUN_8001fab4(o);
        AnimAdvance(o);
        slope(o, o->y.p.whole + 0xe);
        if (func_801256F4(o, o->animFrame)) {
            o->substep++;
            if (*w) o->wac = 0xf;
            else o->wac = 0x1e;
            SET();
            break;
        }
        if (!o->visible) goto off;
        break;
    case 3:
        if (AnimAdvance(o)) {
            o->w22 = 0x1e;
            o->animFrame = 1 - o->animFrame;
            o->substep++;
            if (*w) o->wac = 8;
            else o->wac = 0;
            SET();
        }
        break;
    case 4:
        if (--o->w22 == -1) {
            if (*w) {
                o->wac = 8;
                o->timer = D_80135D54[Rand() & 7];
            } else {
                o->wac = 1;
                o->timer = D_80135D4C[Rand() & 7];
            }
            o->substep = 2;
        }
        break;
    case 5:
        {
            short yy = o->y.p.whole;
            o->y.p.whole = yy + 1;
            slope(o, yy + 0xf);
        }
        if (o->visible) {
            if (--o->timer == -1) {
                if ((Rand() & 1) == o->animFrame) {
                    o->substep = 1;
                } else {
                    o->substep = 3;
                    if (*w) o->wac = 0xf;
                    else o->wac = 0x1e;
                    SET();
                }
            }
        } else {
        off:
            if (*w == 0 || (unsigned short)(o->h->p.whole - D_1F800176 + 0x70) >= 0x220 ||
                (unsigned short)(D_1F800186 - o->y.p.whole + 0x70) >= 0x1c0) {
                o->state = 0;
                o->substep = 0;
            }
        }
        break;
    }
}

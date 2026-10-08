// FUNC 80130bb0 3124 X000
// MATCHING 80130bb0 3124
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short x, y; } P;
typedef struct { int a, b, c; } V3;
extern int FUN_8002078c(P a, P b);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001e4f0(int);
extern int FUN_8001f9e0(void);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001fa60(TObj *, int);
extern int FUN_8001fddc(int, int);
extern int FUN_8001fdac(int, int);
extern short FUN_800411cc(TObj *, short, short);
extern Fix16 *DAT_800a6078;
extern Fix16 *DAT_800a607c;
extern unsigned short DAT_800a604e;
extern void *DAT_8013ad38[];
extern void *DAT_8013ad5c[];
extern void *DAT_8013ad6c[];
extern short DAT_80139260[];
extern unsigned short DAT_80139240[];
extern short *DAT_80139234[];
extern char D_80077cdc[];
extern char D_80077d0c[];
extern char D_80077d84[];

#define WBC U16(o, 0xbc)
#define C8 U16(o, 0xc8)
#define CA U16(o, 0xca)

#define TICK                         \
    if (--WBC == 0) {                \
        WBC = 0x38;                  \
        if (CA)                      \
            FUN_8001e4f0(0x12);      \
    }

#define MOD90                                              \
    {                                                      \
        int m = (short)(o->d->p.whole % 90);                      \
        if (m >= 0x50)                                     \
            o->d->p.whole = 0x5a - m + o->d->p.whole;    \
        if (m < 0xb)                                       \
            o->d->p.whole -= m;                            \
    }

static __inline__ int check(TObj *o)
{
    short r;
    unsigned short d;
    int x;
    if (o->d->p.whole != DAT_800a607c->p.whole) {
        x = 0;
    } else {
        r = 0;
        if ((unsigned short)(DAT_800a6078->p.whole - o->h->p.whole + 0x40) < 0x80 && (FUN_8001f9e0() & 0xf) < 12) {
            r = 1;
        } else if ((unsigned short)(DAT_800a6078->p.whole - o->h->p.whole + 0x80) < 0x100 && (FUN_8001f9e0() & 0xf) < 6) {
            r = 1;
        }
        x = 0;
        d = DAT_800a604e - o->y.p.whole + 0x30;
        if (r == 1)
            x = d < 0xf8;
        else
            x = 0;
    }
    return x;
}

void FUN_80130bb0(TObj *o)
{
    P a, b;
    int dx;
    short *t;
    int ang;
    unsigned short s;
    unsigned char n;

    switch (o->substep) {
    case 0:
        if (U16(o, 0xb4)) {
            o->w22 = 0xf0;
            WBC = 1;
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            n = 4;
            goto set;
        } else {
            o->w22 = 0x38;
            o->wb4 = 1;
            WBC = 1;
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            o->movetab = D_80077cdc;
            o->substep++;
        }
        break;
    case 1:
        if (--o->w22 == 0) {
            o->velH = 0;
            o->velV = 0;
            WBC = 1;
            o->substep++;
        }
        goto tick;
    case 2:
        o->y.raw += o->velV << 8;
        o->velV -= 0x10;
        if (o->velV < -0x1ff || FUN_800411cc(o, o->h->p.whole, o->y.p.whole - 0x18)) {
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            WBC = 1;
            C8 = 0;
            o->substep++;
        }
        goto tick;
    case 3:
        o->y.raw += o->velV << 8;
        o->velV += 0x10;
        if (o->velV >= 0 || FUN_800411cc(o, o->h->p.whole, o->y.p.whole - 0x18)) {
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            WBC = 1;
            C8 = 0;
            o->w22 = 0xf0;
            o->substep++;
        }
        goto tick;
    case 4:
        if (--WBC == 0) {
            WBC = 0x38;
            C8 ^= 1;
            if (CA)
                FUN_8001e4f0(0x12);
        }
        if (--o->w22 == 0) {
            o->timer = 0xf0;
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            o->substep = 9;
        }
        if (o->w22 == 0xb4 || o->w22 == 0x3c) {
            if (check(o)) {
                o->step = 1;
                o->state = 0;
                o->substep = 0;
                break;
            }
            *(V3 *)&o->d30 = *(V3 *)&o->a;
            switch (DAT_80139260[FUN_8001f9e0() & 0xf]) {
            case 0:
                o->animFrame = 1;
                break;
            case 1:
                o->animFrame = 0;
                break;
            case 2:
            default:
                goto out;
            }
            o->wba = ((FUN_8001f9e0() & 1) << 5) + 0x20;
            WBC = 0x38;
            o->movetab = D_80077d0c;
            o->wac = 0xb;
            o->anim = DAT_8013ad5c[0];
            FUN_8001fe6c(o);
            o->w22 = 0xb4;
            o->substep++;
        }
    out:
        break;
    case 5:
        dx = 0x20;
        if (o->animFrame)
            dx = -0x20;
        s = o->wba + ((o->h->raw - o->d30) >> 16);
        if (--o->w22 == 0
            || (unsigned short)o->wba * 2 < s
            || FUN_800411cc(o, o->h->p.whole + dx, o->y.p.whole + 0x18)) {
            o->w22 = 0x3c;
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            o->substep++;
        } else {
            t = DAT_80139234[o->subtype];
            if (o->h->p.whole < t[0] || t[1] < o->h->p.whole) {
                o->wac = 2;
                o->anim = DAT_8013ad38[0];
                FUN_8001fe6c(o);
                o->substep = 9;
            }
        }
        FUN_8001fa88(o, o->animFrame);
        goto tick;
    case 6:
        if (--o->w22 == 0) {
            o->w22 = 0xa8;
            WBC = 1;
            o->movetab = D_80077d84;
            o->wac = 0xf;
            o->anim = DAT_8013ad6c[0];
            FUN_8001fe6c(o);
            n = o->substep;
            C8 = 0;
            n++;
        set:
            o->substep = n;
        }
        break;
    case 7:
        if (--o->w22 == 0) {
            if (check(o)) {
                o->step = 1;
                o->state = 0;
                o->substep = 0;
            } else {
                o->movetab = D_80077d0c;
                o->wac = 0xb;
                o->animFrame ^= 1;
                o->anim = DAT_8013ad5c[0];
                FUN_8001fe6c(o);
                o->substep++;
            }
        }
        if (--WBC == 0) {
            WBC = 0x38;
            o->wac = 0xf;
            C8 ^= 1;
            o->anim = DAT_8013ad6c[0];
            FUN_8001fe6c(o);
            if (CA)
                FUN_8001e4f0(0x12);
        }
        FUN_8001fa60(o, C8);
        break;
    case 8:
        dx = 0x20;
        if (o->animFrame)
            dx = -0x20;
        s = ((o->a.raw - o->d30) >> 16) + 4;
        if (s < 8
            || FUN_800411cc(o, o->h->p.whole + dx, o->y.p.whole + 0x18)) {
            o->w22 = 0x3c;
            o->wac = 2;
            o->anim = DAT_8013ad38[0];
            FUN_8001fe6c(o);
            o->substep++;
        }
        FUN_8001fa88(o, o->animFrame);
        break;
    case 9:
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        b.x = S16(o, 0xbe);
        b.y = U16(o, 0xc0) - 0x38;
        if (b.x < a.x)
            o->animFrame = 1;
        else
            o->animFrame = 0;
        ang = (FUN_8002078c(a, b) + 0x100) & 0xf8;
        o->h->raw += (short)FUN_8001fddc(ang, 0x200) << 8;
        o->y.raw += (short)FUN_8001fdac(ang, 0x200) << 8;
        MOD90;
        dx = U16(o, 0xbe) - (unsigned short)o->h->p.whole + 4;
        if ((unsigned short)dx < 8) {
            o->h->p.whole = S16(o, 0xbe);
            o->timer = 0xb4;
            o->substep++;
        }
    tick:
        TICK;
        break;
    case 10:
        o->y.raw += ((U16(o, 0xc0) << 16) - o->y.raw) >> 5;
        MOD90;
        TICK;
        if ((unsigned short)(U16(o, 0xc0) - o->y.p.whole + 4) < 8) {
            o->wb4 = 0;
            o->state = DAT_80139240[FUN_8001f9e0() & 0xf];
            o->substep = 0;
            if (o->d->p.whole == DAT_800a607c->p.whole) {
                unsigned short d = DAT_800a604e - o->y.p.whole + 0x30;
                if ((unsigned short)(DAT_800a6078->p.whole - o->h->p.whole + 0x80) < 0x100 && d < 0xf8) {
                    o->state = 4;
                    o->substep = 0;
                }
            }
        } else if (--o->timer == 0) {
            o->state = 3;
            o->substep = 0;
        }
        break;
    default:
        return;
    }
    FUN_8001fec0(o);
}

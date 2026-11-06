// VERSION SLPS_01144 x04 80124270 2904
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short x, y; } P;
typedef struct { int a, b, c; } V3;
extern int angleBetweenPoints(P a, P b);
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern int Rand(void);
extern void applyAnimVelocityX(TObj *, int);
extern void applyAnimVelocityY(TObj *, int);
extern int MulCos(int, int);
extern int MulNegSinScaled(int, int);
extern short probeCollisionAtDepthB(TObj *, short, short);
extern short probeFloorPlaneAt(TObj *, int, int);
extern Fix16 *PLAYER_H;
extern Fix16 *D_800A7BBC;
#define PLAYER_D D_800A7BBC
extern unsigned short PLAYER_Y;
extern void *D_801305D4[];
extern void *D_80130628[];
extern void *D_8013062C[];
extern void *D_801305E8[];
extern void *D_801305EC[];
extern short D_8012F5F0[];
extern unsigned short D_8012F5D0[];
extern char MOVE_TABLE_0[];
extern char D_800716F4[];
extern char D_8007176C[];

#define WBC (*(unsigned short*)&o->wbc)
#define C8 U16(o, 0xcc)
#define CA U16(o, 0xca)

#define TICK                         \
    if (--WBC == 0) {                \
        WBC = 0x28;                  \
        if (CA)                      \
            playSFX(0x12);           \
    }

static __inline__ int check(TObj *o)
{
    short r;
    unsigned short d;
    int x;
    if (o->d->p.whole != PLAYER_D->p.whole) {
        x = 0;
    } else {
        r = 0;
        if ((unsigned short)(PLAYER_H->p.whole - o->h->p.whole + 0x40) < 0x80 && (Rand() & 0xf) < 12) {
            r = 1;
        } else if ((unsigned short)(PLAYER_H->p.whole - o->h->p.whole + 0x80) < 0x100 && (Rand() & 0xf) < 6) {
            r = 1;
        }
        x = 0;
        d = PLAYER_Y - o->y.p.whole + 0x30;
        if (r == 1)
            x = d < 0xf8;
        else
            x = 0;
    }
    return x;
}

void func_80124270_slps01144(TObj *o)
{
    P a, b;
    int dx;
    int ang;
    unsigned short s;
    unsigned char n;

    switch (o->substep) {
    case 0:
        if ((*(unsigned short*)&o->wb4)) {
            o->w22 = 0xf0;
            WBC = 1;
            o->wac = 1;
            o->anim = D_801305D4[0];
            AnimLoadDuration(o);
            o->substep = 4;
        } else {
            o->w22 = 0x30;
            o->wb4 = 1;
            WBC = 1;
            o->wac = 0x16;
            o->anim = D_80130628[0];
            AnimLoadDuration(o);
            o->movetab = MOVE_TABLE_0;
            o->substep++;
        }
        break;
    case 1:
        if (--o->w22 == 0) {
            WBC = 1;
            o->velH = 0;
            o->velV = 0;
            o->wac = 0x17;
            o->anim = D_8013062C[0];
            AnimLoadDuration(o);
            o->substep++;
        }
        goto tick;
    case 2:
        o->y.raw += o->velV << 8;
        o->velV -= 0x10;
        if (o->velV < -0x1ff || probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole - 0x18)) {
            WBC = 1;
            o->substep++;
        }
        goto tick;
    case 3:
        o->y.raw += o->velV << 8;
        o->velV += 0x10;
        if (o->velV >= 0 || probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole - 0x18)) {
            o->wac = 1;
            o->anim = D_801305D4[0];
            AnimLoadDuration(o);
            WBC = 1;
            o->w22 = 0xf0;
            o->substep++;
        }
        goto tick;
    case 4:
        if (--WBC == 0) {
            WBC = 0x38;
            C8 ^= 1;
            if (CA)
                playSFX(0x12);
        }
        if (--o->w22 == 0) {
            o->timer = 0xf0;
            o->wac = 1;
            o->anim = D_801305D4[0];
            AnimLoadDuration(o);
            if ((unsigned short)U16(o, 0xc4) < o->h->p.whole)
                o->animFrame = 0;
            else
                o->animFrame = 1;
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
            switch (D_8012F5F0[Rand() & 0xf]) {
            case 0:
                o->animFrame = 1;
                o->wba = ((Rand() & 1) << 5) + 0x20;
                WBC = 0x38;
                o->movetab = D_800716F4;
                o->wac = 6;
                o->anim = D_801305E8[0];
                AnimLoadDuration(o);
                o->w22 = 0xb4;
                o->substep++;
                break;
            case 1:
                o->animFrame = 0;
                o->wba = ((Rand() & 1) << 5) + 0x20;
                WBC = 0x38;
                o->movetab = D_800716F4;
                o->wac = 6;
                o->anim = D_801305E8[0];
                AnimLoadDuration(o);
                o->w22 = 0xb4;
                o->substep++;
                break;
            case 2:
            default:
                break;
            }
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
            || probeCollisionAtDepthB(o, o->h->p.whole + dx, o->y.p.whole + 0x18)) {
            o->w22 = 0x28;
            o->wac = 1;
            o->anim = D_801305D4[0];
            AnimLoadDuration(o);
            o->substep++;
        }
        applyAnimVelocityX(o, o->animFrame);
        goto tick;
    case 6:
        if (--o->w22 == 0) {
            o->w22 = 0x78;
            WBC = 1;
            o->movetab = D_8007176C;
            o->wac = 1;
            o->anim = D_801305D4[0];
            AnimLoadDuration(o);
            n = o->substep;
            C8 = 0;
            n++;
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
                o->movetab = D_800716F4;
                o->wac = 6;
                o->animFrame ^= 1;
                o->anim = D_801305E8[0];
                AnimLoadDuration(o);
                o->substep++;
            }
        }
        if (--WBC == 0) {
            WBC = 0x28;
            o->wac = 7;
            C8 ^= 1;
            o->anim = D_801305EC[0];
            AnimLoadDuration(o);
            if (CA)
                playSFX(0x12);
        }
        applyAnimVelocityY(o, C8);
        break;
    case 8:
        dx = 0x20;
        if (o->animFrame)
            dx = -0x20;
        s = ((o->a.raw - o->d30) >> 16) + 4;
        if (s < 8
            || probeCollisionAtDepthB(o, o->h->p.whole + dx, o->y.p.whole + 0x18)) {
            o->w22 = 0x3c;
            o->wac = 1;
            o->anim = D_801305D4[0];
            AnimLoadDuration(o);
            if ((unsigned short)U16(o, 0xc4) < o->h->p.whole)
                o->animFrame = 0;
            else
                o->animFrame = 1;
            o->substep++;
        }
        applyAnimVelocityX(o, o->animFrame);
        break;
    case 9:
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        b.x = U16(o, 0xc4);
        b.y = U16(o, 0xc6) - 0x38;
        if (b.x < a.x)
            o->animFrame = 1;
        else
            o->animFrame = 0;
        ang = (angleBetweenPoints(a, b) + 0x100) & 0xf8;
        o->h->raw += (short)MulCos(ang, 0x200) << 8;
        o->y.raw += (short)MulNegSinScaled(ang, 0x200) << 8;
        dx = U16(o, 0xc4) - (unsigned short)o->h->p.whole + 4;
        if ((unsigned short)dx < 8) {
            o->h->p.whole = U16(o, 0xc4);
            o->timer = 300;
            o->substep++;
        }
    tick:
        TICK;
        break;
    case 10:
        o->y.raw += ((U16(o, 0xc6) << 16) - o->y.raw) >> 6;
        TICK;
        if ((unsigned short)(U16(o, 0xc6) - o->y.p.whole + 4 - 2) < 4) {
            if (probeFloorPlaneAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x18))) {
                o->wb4 = 0;
                {
                    unsigned short st = D_8012F5D0[Rand() & 0xf];
                    o->substep = 0;
                    o->state = st;
                }
                if (o->d->p.whole == PLAYER_D->p.whole) {
                    unsigned short d = PLAYER_Y - o->y.p.whole + 0x30;
                    if ((unsigned short)(PLAYER_H->p.whole - o->h->p.whole + 0x80) < 0x100 && d < 0xf8) {
                        o->state = 4;
                        o->substep = 0;
                    }
                }
            } else {
                o->state = 3;
                o->substep = 0;
            }
        } else if (--o->timer == 0) {
            o->state = 3;
            o->substep = 0;
        }
        break;
    default:
        break;
    }
}
#endif

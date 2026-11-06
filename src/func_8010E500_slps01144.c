// VERSION SLPS_01144 x00 8010E500 1080
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gv2.h"
#include "gcd.h"
#include "gv6.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"

typedef struct { short f0, f2; unsigned short f4; short f6, f8; short pad[5]; } T8010EAF8;

extern T8010EAF8 D_80114748[];

static __inline__ int getU(TObj *o)
{
    int b = U8(o, 0xc1);
    int u = GV2.bb3 + b * 4;
    if ((GAME.displayExpBar & 3) != 0)
        u = (unsigned char)b << 2 | 3;
    if (GCD.bcf06 != 0)
        u = 8;
    if (GCD.bd006 != 0)
        u = 8;
    return u;
}

static __inline__ void decel(TObj *o, int u)
{
    unsigned short x = o->velX;
    int xs;
    int r;
    if ((unsigned short)(x + D_80114748[u].f6) <= D_80114748[u].f6 * 2) {
        o->velX = 0;
        return;
    }
    if (o->animFrame & 1) {
        xs = (short)x;
        if (xs < 0)
            r = xs + D_80114748[U8(o, 0xc1)].f6;
        else
            r = xs - D_80114748[u].f8;
    } else {
        xs = (short)x;
        if (xs > 0)
            r = xs - D_80114748[U8(o, 0xc1)].f6;
        else
            r = xs + D_80114748[u].f8;
    }
    o->velX = r;
}

void func_8010E500_slps01144(TObj *o)
{
    int u = getU(o);
    unsigned char b;

    if (o->velY > 0) {
        switch (o->animFrame) {
        case 0:
        case 4:
            b = o->ba6;
            if (b & 2)
                if (!(b & 1))
                    return;
            if (o->velX >= D_80114748[u].f0)
                o->velX = o->velX - D_80114748[u].f4;
            else
                o->velX = o->velX + D_80114748[u].f4;
            break;
        case 1:
        case 5:
            b = o->ba6;
            if (b & 2)
                if (b & 1)
                    return;
            if (o->velX <= -D_80114748[u].f0)
                o->velX = o->velX + D_80114748[u].f4;
            else
                o->velX = o->velX - D_80114748[u].f4;
            break;
        default:
            decel(o, u);
            break;
        }
    } else {
        switch (o->animFrame) {
        case 0:
        case 4:
            b = o->ba6;
            if (b & 2)
                if (!(b & 1))
                    return;
            o->velX += D_80114748[u].f4;
            if (o->velX > D_80114748[u].f2)
                o->velX = D_80114748[u].f2;
            break;
        case 1:
        case 5:
            b = o->ba6;
            if (b & 2)
                if (b & 1)
                    return;
            o->velX -= D_80114748[u].f4;
            if (o->velX < -D_80114748[u].f2)
                o->velX = -D_80114748[u].f2;
            break;
        default:
            decel(o, u);
            break;
        }
    }
}
#endif

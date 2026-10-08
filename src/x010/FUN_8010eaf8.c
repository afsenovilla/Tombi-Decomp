// FUNC 8010eaf8 1532 X010
// MATCHING 8010eaf8 1532
#include "TOBJ.H"
#include "raw7.h"

typedef struct { short f0, f2, f4, f6, f8; short pad[5]; } T8010EAF8;

extern unsigned char D_8009D2B3, D_8009C990, D_8009CF06, D_8009D006;
extern unsigned short D_8009D610;
extern T8010EAF8 D_8011545C[];

static __inline__ int getU(TObj *o)
{
    int b = U8(o, 0xc1);
    int u = D_8009D2B3 + b * 4;
    if ((D_8009C990 & 3) != 0)
        u = (unsigned char)b << 2 | 3;
    if (D_8009CF06 != 0)
        u = 8;
    if (D_8009D006 != 0)
        u = 8;
    return u;
}

static __inline__ void decel(TObj *o, int u)
{
    unsigned short x = o->velX;
    int xs;
    int r;
    if ((unsigned short)(x + D_8011545C[u].f6) <= D_8011545C[u].f6 * 2) {
        o->velX = 0;
        return;
    }
    if (o->animFrame & 1) {
        xs = (short)x;
        if (xs < 0)
            r = xs + D_8011545C[U8(o, 0xc1)].f6;
        else
            r = xs - D_8011545C[u].f8;
    } else {
        xs = (short)x;
        if (xs > 0)
            r = xs - D_8011545C[U8(o, 0xc1)].f6;
        else
            r = xs + D_8011545C[u].f8;
    }
    o->velX = r;
}

void FUN_8010eaf8(TObj *o)
{
    short u = getU(o);
    unsigned short acc = D_8011545C[u].f4;
    unsigned short lim = D_8011545C[u].f0;
    unsigned short max = D_8011545C[u].f2;
    unsigned char b;

    if (D_8009D610 == 7) {
        if (o->velY > 0) {
            switch (o->animFrame) {
            case 0:
            case 4:
                b = o->ba6;
                if (b & 2)
                    if (!(b & 1))
                        return;
                if (o->velX >= (short)lim)
                    o->velX = o->velX - acc;
                else
                    o->velX = o->velX + acc;
                break;
            case 1:
            case 5:
                b = o->ba6;
                if (b & 2)
                    if (b & 1)
                        return;
                if (o->velX <= -(short)lim)
                    o->velX = o->velX + acc;
                else
                    o->velX = o->velX - acc;
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
                o->velX += acc;
                if (o->velX > (short)max)
                    o->velX = max;
                break;
            case 1:
            case 5:
                b = o->ba6;
                if (b & 2)
                    if (b & 1)
                        return;
                o->velX -= acc;
                if (o->velX < -(short)max)
                    o->velX = -max;
                break;
            default:
                decel(o, u);
                break;
            }
        }
    } else {
        if (o->velY > 0) {
            switch (o->animFrame) {
            case 0:
            case 4:
                b = o->ba6;
                if (b & 2)
                    if (!(b & 1))
                        return;
                if (o->velX >= (short)lim)
                    o->velX = o->velX - acc;
                else
                    o->velX = o->velX + acc;
                break;
            case 1:
            case 5:
                b = o->ba6;
                if (b & 2)
                    if (b & 1)
                        return;
                if (o->velX <= -(short)lim)
                    o->velX = o->velX + acc;
                else
                    o->velX = o->velX - acc;
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
                o->velX += acc;
                if (o->velX > (short)max)
                    o->velX = max;
                break;
            case 1:
            case 5:
                b = o->ba6;
                if (b & 2)
                    if (b & 1)
                        return;
                o->velX -= acc;
                if (o->velX < -(short)max)
                    o->velX = -max;
                break;
            default:
                decel(o, u);
                break;
            }
        }
    }
}

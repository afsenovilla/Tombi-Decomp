// FUNC 800402ec 880 MAIN0
// wip score 155 (was 201): s0..s7 pinned to the game's ret/x/o/f1/cnt/ys/fl/y and f/b/c to a3/t1/a0; fl branches swapped (getCollisionPlaneAt in the else), (short)r >= 0 -> goto hit. sx = xm << 24 computed once, `sx >> 24` per use (xm asm-laundered no longer needed). left: gcc folds x - 1 - sx into x - (sx + 1) and x + 8 - xm into x - (xm - 8) (game keeps (x-1)-sx, (x+8)-t1 with xm copied to t1); bs/d swapped (a1/v1); game computes c & 0xf before xm; loop-end n load is lh + move t0 in the game
#include "spad.h"
#include "TOBJ.H"

extern unsigned short *getCollisionPlaneAt(int, int);
extern unsigned short *D_1F800278;
extern unsigned short D_1F800282;
extern unsigned char *D_8007B5AC;
#define D D_1F800278

int func_800402EC(TObj *o, int x_, int y_, int fl_)
{
    register int x asm("$17") = x_;
    register int y asm("$23") = y_;
    register int fl asm("$22") = fl_;
    register int ret asm("$16");
    register int ys asm("$21");
    register int f1 asm("$19");
    register short cnt asm("$20");
    short n;
    unsigned short a;
    register unsigned short f asm("$7");
    register unsigned short b asm("$9");
    register unsigned short c asm("$4");
    short bs, d;
    int r, k;
    unsigned int xm;
    unsigned char *t;
    Fix16 *h;
    unsigned short w;
    int sx;
    char pad[24];

    n = *D++;
    if (n == 0)
        return 0;
    if (0) {
    hit:
        (*(unsigned short*)&SPAD.s284) = (f & 0xe00) >> 9;
        D_1F800282 = f;
        o->h->p.whole -= r;
        return k;
    }
    cnt = 0;
    ret = 0;
    ys = (short)y_;
    f1 = fl_ & 1;
    do {
        f = *D++;
        n--;
        if ((f & 0xc) == 0) {
            D += 3;
            continue;
        }
        a = *D++;
        b = *D++;
        c = *D++;
        bs = b;
        if (bs == 0)
            continue;
        d = y - a;
        if (bs < 0) {
            if (d < bs)
                continue;
            if (d > 0)
                return ret;
        } else {
            if (bs < d)
                return ret;
            if (d < 0)
                continue;
        }
        xm = x & 7;
        sx = xm << 24;
        r = xm - (c & 0xf) - ((c >> 4) & 0xf) * (ys - (short)a) / (short)b;
        if (f & 0x10) {
            if (!f1 && (f & 4)) {
                x = x - 1 - (sx >> 24);
                ret = 1;
                h = o->h; w = h->p.whole - 1; h->p.whole = w - (sx >> 24);
            } else if (f & 8) {
                x = x + 8 - xm;
                ret = 2;
                h = o->h; h->p.whole = h->p.whole + 8 - xm;
            }
            (*(unsigned short*)&SPAD.s284) = (f & 0xe00) >> 9;
            cnt++;
            if (cnt >= 8)
                return ret;
            if ((fl & 0xff00) != 0xff00) {
                t = D_8007B5AC;
                t += *(unsigned short *)(t + 8);
                D = (unsigned short *)(t + *(unsigned short *)(t + ((short)x / 8) * 2));
            } else {
                D = getCollisionPlaneAt((short)x, o->d->p.whole);
            }
            n = *D++;
            continue;
        }
        if (!f1) {
            if (!(f & 4))
                continue;
            if ((short)r >= 0) {
                k = 1;
                goto hit;
            }
            return ret;
        } else {
            if (!(f & 8))
                continue;
            if ((short)r <= 0) {
                k = 2;
                goto hit;
            }
            return ret;
        }
    } while (n != 0);
    return ret;
}

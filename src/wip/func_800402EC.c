// FUNC 800402ec 880 MAIN0
// wip score 207: layout ok (hit block before loop via goto, pad[24]); left: gcc reassociates x-1-(signed char)xm and x+8-xm, fl&1 hoist order, regalloc
#include "TOBJ.H"

extern unsigned short *func_8003F200(int, int);
extern unsigned short *D_1F800278;
extern unsigned short D_1F800282;
extern unsigned short D_1F800284;
extern unsigned char *D_8007B5AC;
#define D D_1F800278

int func_800402EC(TObj *o, int x, int y, int fl)
{
    short n;
    short cnt;
    int ret;
    unsigned short f, a, b, c;
    short bs, d;
    int r, k;
    unsigned int xm;
    unsigned char *t;
    int f1;
    short ys;
    Fix16 *h;
    char pad[24];

    n = *D++;
    if (n == 0)
        return 0;
    if (0) {
    hit:
        D_1F800284 = (f & 0xe00) >> 9;
        D_1F800282 = f;
        o->h->p.whole -= r;
        return k;
    }
    cnt = 0;
    ret = 0;
    f1 = fl & 1;
    ys = y;
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
        if (bs < 0) {
            d = y - a;
            if (d < bs)
                continue;
            if (d > 0)
                return ret;
        } else {
            d = y - a;
            if (bs < d)
                return ret;
            if (d < 0)
                continue;
        }
        xm = x & 7;
        r = xm - (c & 0xf) - ((c >> 4) & 0xf) * (ys - (short)a) / (short)b;
        if (f & 0x10) {
            if (!f1 && (f & 4)) {
                x = x - 1 - (signed char)xm;
                ret = 1;
                h = o->h; h->p.whole = h->p.whole - 1 - (signed char)xm;
            } else if (f & 8) {
                x = x + 8 - xm;
                ret = 2;
                h = o->h; h->p.whole = h->p.whole + 8 - xm;
            }
            D_1F800284 = (f & 0xe00) >> 9;
            cnt++;
            if (cnt >= 8)
                return ret;
            if ((fl & 0xff00) == 0xff00) {
                D = func_8003F200((short)x, o->d->p.whole);
            } else {
                t = D_8007B5AC;
                t += *(unsigned short *)(t + 8);
                D = (unsigned short *)(t + *(unsigned short *)(t + ((short)x / 8) * 2));
            }
            n = *D++;
            continue;
        }
        if (!f1) {
            if (!(f & 4))
                continue;
            if ((short)r < 0)
                return ret;
            k = 1;
            goto hit;
        } else {
            if (!(f & 8))
                continue;
            if ((short)r > 0)
                return ret;
            k = 2;
            goto hit;
        }
    } while (n != 0);
    return ret;
}

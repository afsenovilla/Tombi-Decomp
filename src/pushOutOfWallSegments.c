// FUNC 800402ec 880 MAIN0
// MATCHING 800402ec 880
#include "spad.h"
#include "TOBJ.H"

extern unsigned short *getCollisionPlaneAt(int, int);
extern unsigned short *D_1F800278;
extern unsigned short D_1F800282;
extern unsigned char *D_8007B5AC;
#define D D_1F800278

int pushOutOfWallSegments(TObj *o, int x_, int y_, int fl_)
{
    int x = x_;
    register int y asm("$23") = y_;
    register int fl asm("$22") = fl_;
    register int ret asm("$16");
    int ys;
    register int f1 asm("$19");
    short cnt;
    short n;
    unsigned short a;
    register unsigned short f asm("$7");
    register unsigned short b asm("$9");
    register unsigned short c asm("$4");
    short bs, d;
    int r, k;
    register unsigned int xm asm("$9");
    unsigned char *t;
    register Fix16 *h asm("$3");
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
    while (n != 0) {
    top:
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
asm("" : "=r"(a) : "0"(a));
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
{ int q = ((c >> 4) & 0xf) * (ys - (short)a) / (short)b; register int tt asm("$2") = x & 7; xm = tt; asm("" : "=r"(xm) : "0"(xm)); sx = tt << 24; r = tt - (c & 0xf) - q; }
        if (f & 0x10) {
asm("" : "=r"(sx) : "0"(sx));
            if (!f1 && (f & 4)) {
                { register int t asm("$2"); int s1; s1 = sx >> 24; asm("" : "=r"(s1) : "0"(s1)); t = x - 1; x = t - s1; }
                h = o->h; asm("" : "=r"(h) : "0"(h)); { int s2; unsigned short t; t = h->p.whole; s2 = sx >> 24; asm("" : "=r"(s2) : "0"(s2)); t = t - 1; h->p.whole = t - s2; }
                ret = 1;
            } else if (f & 8) {
                { int t; t = x + 8; x = t - xm; }
                ret = 2;
                h = o->h; h->p.whole = h->p.whole + 8 - xm;
            }
            (*(unsigned short*)&SPAD.s284) = (f & 0xe00) >> 9;
            asm("" :: "r"(f));
            cnt++;
            if (cnt >= 8)
                return ret;
            { int w3 = fl & 0xff00; register int w5 asm("$5") = w3; if (w3 != 0xff00) { asm("" :: "r"(w5));
asm("" : "=r"(x) : "0"(x));
                t = D_8007B5AC;
                t += *(unsigned short *)(t + 8);
                { int i2 = ((short)x / 8) * 2; register unsigned int o2 asm("$3") = *(unsigned short *)(t + i2); register unsigned short *p asm("$2") = (unsigned short *)(t + o2); D = p; }
            } else {
                D = getCollisionPlaneAt((short)x, o->d->p.whole);
            } }
            { register int nn asm("$2"); register short *q asm("$2") = (short *)D; D++; nn = *q; n = nn; if (nn != 0) goto top; }
            break;
        }
        if (!f1) {
            if (!(f & 4))
                continue;
asm("" : "=r"(r) : "0"(r));
            if ((short)r >= 0) {
                k = 1;
                goto hit;
            }
            return ret;
        } else {
            if (!(f & 8))
                continue;
asm("" : "=r"(r) : "0"(r));
            if ((short)r <= 0) {
                k = 2;
                goto hit;
            }
            return ret;
        }
    }
    return ret;
}

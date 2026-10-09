// FUNC 8011b588 364 X003
// MATCHING 8011b588 364
#include "TOBJ.H"

extern short D_1F80016A, D_1F80016E;
extern TObj D_800A6038;
extern unsigned char D_80135A78[];
extern int FUN_800205d8(int, int);

int func_8011B588(TObj *o, int mode, int target)
{
    int t;
    int cur;
    unsigned char d;
    unsigned char thr;

    if (mode == 0)
        t = FUN_800205d8(D_1F80016A - o->h->p.whole, D_1F80016E - o->y.p.whole);
    else if (mode == 1) {
        if (o->subtype & 2)
            t = (o->subtype & 1) << 7;
        else
            t = (D_800A6038.h->p.whole < o->h->p.whole) << 7;
    } else
        t = target;
    cur = o->d38;
    d = t - cur;
    thr = D_80135A78[(cur & 0xff) >> 4];
    if (d != 0) {
        if (thr >= d) {
            if (d >= 2)
                d = 2;
            if ((unsigned char)cur >= 0xc1)
                o->d38 = cur + d;
            else {
                o->d38 = cur + d;
                if ((unsigned char)o->d38 >= 0xa0)
                    o->d38 = 0xa0;
            }
        } else {
            if (d < 0xff)
                d = 0xfe;
            if ((unsigned char)cur < 0xc0)
                o->d38 = cur + d;
            else {
                o->d38 = cur + d;
                if ((unsigned char)o->d38 < 0xe1)
                    o->d38 = 0xe0;
            }
        }
        o->d38 = (unsigned char)o->d38;
        return 0;
    }
    return 1;
}

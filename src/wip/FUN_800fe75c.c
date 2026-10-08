// FUNC 800fe75c 304 X000
#include "tobj.h"
extern unsigned char c2b1, cf06, c938, c942;
extern unsigned short c960, c962;
extern char *f0ec;
extern void fa(int), fb(int);
void FUN_800fe75c(TObj *o)
{
    unsigned char b = o->state;
    unsigned char s;
    short t;
    if (b == 1) {
        t = o->timer;
        o->timer = t - 1;
        if (t != 1) return;
        c2b1 = 0;
        cf06 = 0;
        if (c960 != 10 || c962 != 0) c938 = 2;
        s = o->state;
    } else {
        if (1 < b) return;
        if (b != 0) return;
        s = o->b9e;
        if (s != 0 && (s == 4 || s == 7)) *f0ec = 1;
        if (c938 != 1) {
            c942 = 1;
            c938 = 1;
            fa(0);
            fb(3);
        }
        o->timer = 0xb4;
        o->b0f = 0xf8;
        s = o->state;
        o->visible = 1;
    }
    o->state = s + 1;
}

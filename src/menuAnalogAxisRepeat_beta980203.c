// VERSION BETA_980203 beta980203 80028014 196
#if defined(VERSION_BETA_980203)
// Beta version: dead zone 0x50..0xb0 inclusive, falls through to the common return of 0
short menuAnalogAxisRepeat(unsigned char *p, short mode)
{
    unsigned char v = *p;
    short hi, lo, r;
    unsigned short t;
    unsigned short w;

    r = 0;
    switch (mode) {
    case 0:
        hi = 0x80;
        lo = 0x20;
        break;
    case 1:
        hi = 0x10;
        lo = 0x40;
        break;
    }
    w = v;
    *p = 0;
    if ((unsigned)(w - 0x50) >= 0x61) {
        t = w - 0x30;
        if (t < 0xa0) {
            *p = 1;
            r = lo;
            if (t < 0x50) r = hi;
        } else {
            t = w - 0x10;
            if (t < 0xe0) {
                *p = 2;
                r = lo;
                if (t < 0x70) r = hi;
            } else {
                *p = 3;
                r = lo;
                if (v < 0x80) r = hi;
            }
        }
    }
    return r;
}
#endif

// VERSION SLPS_01144 slps01144 80022B00 32
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
unsigned char func_80022B00_slps01144(unsigned char a, unsigned char b)
{
    unsigned char r;
    int d = a - b;

    r = d;
    if (d < 0) {
        r = 0;
    }
    return r;
}
#endif

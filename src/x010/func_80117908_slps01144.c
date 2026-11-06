// VERSION SLPS_01144 x10 80117908 324
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

extern unsigned char D_8009EAAE, D_8009E4E7, D_8009E4E0, D_8009E4E6;
extern void playSFX(int);
#define S16A (*(unsigned short *)0x1F80016A)
#define S16E (*(short *)0x1F80016E)
#define S172 (*(short *)0x1F800172)

void func_80117908_slps01144(TObj *o)
{
    unsigned char st = o->state;

    switch (st) {
    case 0:
        if (S172 == 90 && S16E >= -84) {
            unsigned int x;
            if (o->subtype == 0)
                x = S16A - 1675;
            else
                x = S16A - 2739;
            if (x < 69) {
                o->state = st + 1;
                o->timer = 16;
                D_8009EAAE = 0;
                D_8009E4E7 = 1;
                D_8009E4E0 = 1;
                D_8009E4E6 = 1;
            }
        }
        break;
    case 1:
        o->y.p.whole += 3;
        if (--o->timer == -1) o->state++;
        break;
    case 2:
        o->state = st + 1;
        playSFX(163);
        break;
    }
}
#endif

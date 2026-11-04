// VERSION BETA_980203 beta980203 80044BBC 172
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/isQuadOnScreen.c (func_80043348_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
typedef struct {
    char pad[8];
    unsigned short w8, wa, pad1[2], w10, w12, pad2[2], w18, w1a, pad3[2], w20, w22;
} Q;

int func_80044BBC_beta980203(Q *o)
{
    if (o->wa < 0xf1 || o->w12 < 0xf1 || o->w1a < 0xf1 || o->w22 < 0xf1) {
        if (o->w8 < 0x141 || o->w10 < 0x141 || o->w18 < 0x141 || o->w20 < 0x141)
            return 1;
    }
    return 0;
}
#endif

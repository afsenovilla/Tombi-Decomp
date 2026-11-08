// VERSION BETA_980203 x00 801363D8 536
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_80131858_slps01144.c (func_80131858_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
typedef struct O {
    char p0[5];
    unsigned char state;
    char p6[0x20 - 6];
    short h20;
    char p22[0x69 - 0x22];
    unsigned char b69;
    char p6a;
    unsigned char b6b;
    char p6c[0x90 - 0x6c];
    struct O *p90, *p94;
    char p98[0xb6 - 0x98];
    unsigned short hb6;
    char pb8[0xbe - 0xb8];
    unsigned short hbe;
    char pc0[0xc6 - 0xc0];
    unsigned short hc6;
    char pc8[0xce - 0xc8];
    unsigned short hce;
} O;

void func_801363D8_beta980203(O *o)
{
    O *q, *r, *q2;
    unsigned char m;
    short t;
    switch (o->state) {
    case 0:
        if (o->b69) {
            o->b6b = o->b69;
            o->h20 = 0;
            o->state++;
        }
        break;
    case 1:
        if (o->b69 == 0) {
            o->state++;
        } else if ((t = o->h20) < 32) {
            o->h20 = t + 4;
            r = o->p94;
            m = o->b69;
            q = o->p90;
            if (m == 2) {
                q->hc6++;
                o->hb6++;
                o->hc6++;
                r->hb6++;
                r->hc6++;
            } else if (m == 3) {
                q->hce++;
                o->hbe++;
                o->hce++;
                r->hbe++;
                r->hce++;
            }
        }
        break;
    case 2:
        o->state++;
        break;
    case 3:
        if (o->h20 != 0) {
            if ((--o->h20 & 3) == 0) {
                r = o->p94;
                m = o->b6b;
                q = o->p90;
                q2 = q;
                if (m == 2) {
                    q->hc6--;
                    o->hb6--;
                    o->hc6--;
                    r->hb6--;
                    r->hc6--;
                } else if (m == 3) {
                    q2->hce--;
                    o->hbe--;
                    o->hce--;
                    r->hbe--;
                    r->hce--;
                }
            }
        } else
            o->state = 0;
        break;
    }
}
#endif

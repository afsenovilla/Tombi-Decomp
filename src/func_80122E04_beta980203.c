// VERSION BETA_980203 x00 80122E04 136
#if defined(VERSION_BETA_980203)
// Beta version: SLPM_80165 C of src/func_80118240_slpm80165.c (func_80118240_slpm80165, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
// Demo-only helper: waits for player mode 5, then spins and rises for 128 frames before state 2.
#include "TOBJ.H"
extern unsigned char D_800A57BD;
void func_80122E04_beta980203(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_800A57BD == 5) {
            o->timer = 128;
            o->step++;
        }
        break;
    case 1:
        o->d8c += 32;
        o->y.p.whole += 2;
        if (--o->timer == 0) {
            o->b04 = 2;
            o->step = 0;
        }
        break;
    }
}
#endif

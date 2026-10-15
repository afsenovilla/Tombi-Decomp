// VERSION SLPM_80165 x00 80118240 136
#if defined(VERSION_SLPM_80165)
// Demo-only helper: waits for player mode 5, then spins and rises for 128 frames before state 2.
#include "TOBJ.H"
extern unsigned char D_800A57BD;
void func_80118240_slpm80165(TObj *o)
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

// VERSION SLPM_80165 x00 8011BFDC 224
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only weapon hit on an object not already knocked out (+0x00 bit 1): +0x00 3 and +0x68 1; a type 0 weapon
// loses 0x200 of +0xA8 (at 0x500 it drops: +0xA5 0, +0x00 2, sound 7), other weapons call func_80049594. Impact
// effect 0 at the weapon, scratchpad word 0x1F80019E cleared.
#include "TOBJ.H"
extern short func_8004ACAC(TObj *, TObj *);
extern void func_80049594(TObj *, TObj *);
extern void func_80020054(int, int, int, int);
extern void func_8001E3D8(int);
extern short D_1F80019E;

void func_8011BFDC_slpm80165(TObj *w, TObj *o)
{
    if (o->active & 2)
        return;
    if (func_8004ACAC(w, o) == -1)
        return;
    o->active = 3;
    o->b68 = 1;
    if (w->type == 0) {
        if ((w->wa8 -= 0x200) < 0x500) {
            w->wa8 = 0x500;
            w->ba5 = 0;
            w->active = 2;
            func_8001E3D8(7);
        }
    } else {
        func_80049594(w, o);
    }
    func_80020054(0, w->a.p.whole, w->y.p.whole, w->b.p.whole);
    D_1F80019E = 0;
}
#endif

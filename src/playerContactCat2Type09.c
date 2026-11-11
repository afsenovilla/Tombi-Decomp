// FUNC 80126698 216 X000
// MATCHING 80126698 216
// VERSION PCPX_96084 tomba 8006D5A4 208
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x000 80126698 (playerContactCat2Type09.c): SLPM_80165 C of src/playerContactCat2Type09.c (func_8011C7DC_slpm80165, twin score 0.914), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
void playerContactCat2Type09(TObj *a, TObj *b)
{
    short dx;
    b->b69 = 0;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) < 0x5b) {
        dx = a->h->p.whole - b->h->p.whole;
        if ((unsigned short)(dx + (a->box0 + b->box0)) > a->box1 + b->box1) return;
        if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) return;
        if (dx > 0) b->b69 = 3;
        else b->b69 = 2;
        a->b69 = 2;
    }
}
#else
#include "TOBJ.H"
void playerContactCat2Type09(TObj *a, TObj *b)
{
    short dx;
    b->b69 = 0;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) < 0x5b) {
        dx = a->h->p.whole - b->h->p.whole;
        if ((unsigned short)(dx + (a->box0 + b->box0)) > a->box1 + b->box1) return;
        if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) return;
        if (dx > 0) b->b69 = 3;
        else b->b69 = 2;
        a->b69 = 2;
        *(short *)0x1F80019E = 0;
    }
}
#endif

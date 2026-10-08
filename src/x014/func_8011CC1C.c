// FUNC 8011cc1c 336 X014
// MATCHING 8011cc1c 336
#include "TOBJ.H"
extern unsigned short D_8009C962;
extern short D_1F800284;
extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short FUN_800411cc(TObj *, short, short);
extern short FUN_80042fbc(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);

void func_8011CC1C(TObj *o, TObj *e)
{
    if (*(unsigned char *)&o->wac == 2) return;
    if (o->active & 2) return;
    if (e->subtype == 1 && D_8009C962 != 7) {
        if (FUN_800411cc(o, o->h->p.whole, o->y.p.whole + o->box2) == 0) return;
        if (D_1F800284 != ((e->wac + 1) & 3)) return;
        o->active = 2;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame &= 1;
        FUN_8004258c(o, 1);
    } else {
        if (FUN_80042fbc(o, e) == 0) return;
        if (D_1F8001A4 != 0) return;
        {
            int ah, bh;
            o->active = 2;
            bh = e->h->p.whole;
            ah = o->h->p.whole;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            o->animFrame = ah < bh;
            FUN_8004258c(o, 3);
        }
    }
    D_1F80019E = 0;
}

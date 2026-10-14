// FUNC 8011cc1c 336 X014
// MATCHING 8011cc1c 336
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"
extern short FUN_800411cc(TObj *, short, short);
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void func_8011CC1C(TObj *o, TObj *e)
{
    if (*(unsigned char *)&o->wac == 2) return;
    if (o->active & 2) return;
    if (e->subtype == 1 && GAME.selectedSection != 7) {
        if (FUN_800411cc(o, o->h->p.whole, o->y.p.whole + o->box2) == 0) return;
        if (SPAD.s284 != ((e->wac + 1) & 3)) return;
        o->active = 2;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame &= 1;
        decreaseObjectTimer(o, 1);
    } else {
        if (boxesOverlap(o, e) == 0) return;
        if (SPAD.b1a4 != 0) return;
        {
            int ah, bh;
            o->active = 2;
            bh = e->h->p.whole;
            ah = o->h->p.whole;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            o->animFrame = ah < bh;
            decreaseObjectTimer(o, 3);
        }
    }
    SPAD.s19e = 0;
}

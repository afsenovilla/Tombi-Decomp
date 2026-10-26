// FUNC 8011ca60 320 X014
// MATCHING 8011ca60 320
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned char *PLAYER_EXT[];
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void flowerForestPigShotContact(TObj *a, TObj *b)
{
    short v;

    if (*(unsigned char *)&a->wac == 2) return;
    if (a->active & 2) return;
    if (!boxesOverlap(a, b)) return;
    if (SPAD.b1a4) return;
    if (GAME.selectedSection == 7) {
        a->active = 2;
        a->animFrame = b->h->p.whole > a->h->p.whole;
        a->b04 = 2;
        a->step = 0;
        a->state = 0;
        decreaseObjectTimer(a, 1);
    } else {
        if (a->h->p.whole > b->h->p.whole) {
            a->animFrame = 1;
            v = 0x200;
        } else {
            v = -0x200;
            a->animFrame = 0;
        }
        a->velX = v;
        PLAYER_EXT[0][9] = 10;
        a->b04 = 6;
        a->step = 7;
        a->state = 0;
    }
    b->active = 2;
    b->b04 = 2;
    b->step = 0;
    b->state = 0;
    SPAD.s19e = 0;
}

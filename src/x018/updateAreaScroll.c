// FUNC 801162d8 676 X018
// MATCHING 801162d8 676
#include "gcd.h"
#include "gvars.h"
/* csv entry 801162b8 (708 B) starts with 8 data words (jump-table/pointer data); code starts at 801162d8.
   debt: volatile read of pl->a in the last range test stops gcc threading the first test's failure jump past it. */
#include "TOBJ.H"
#include "gamestate.h"

typedef void (*Fn)(TObj *);
extern TObj PLAYER;
extern Fn D_80079AD0[];
extern unsigned char D_800A60E0;
extern short D_800A604A, D_800A604E;
extern void cameraModeHandler6B(TObj *);
extern void tryStartItemInteraction(TObj *, int, int);
extern void stepPlayerXToward(int);
extern void cameraEventScrollHandler(TObj *);

void updateAreaScroll(TObj *o)
{
    TObj *pl = &PLAYER;

    switch (o->step) {
    case 0:
        if (GVARS.b3a) o->step++;
        *(short *)((char *)o->d38 + 2) = 0;
        cameraModeHandler6B(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (GAME.selectedSection) {
        case 1:
            if (D_800A604A < 0x14 && D_800A604E >= -0x3b) {
                if (GCD.bcded == 0) goto set3;
                tryStartItemInteraction(o, 1, 0);
            } else if (pl->a.p.whole >= 0x12d && pl->y.p.whole >= -0x3b) {
                tryStartItemInteraction(o, 1, 2);
                break;
            }
            if (GCD.bcded != 0) {
                if ((unsigned short)(pl->a.p.whole - 0x70) < 0x60 && pl->b.p.whole > 0) {
                    stepPlayerXToward(0xa0);
                    if (pl->b.p.whole >= 0x5b) tryStartItemInteraction(o, 1, 1);
                }
            } else {
            set3:
                D_800A60E0 = 3;
            }
            if ((unsigned short)(*(volatile short *)&pl->a.p.whole - 0x70) < 0x60 && pl->b.p.whole < 0) {
                stepPlayerXToward(0xa0);
                if (pl->b.p.whole < -0x5a) tryStartItemInteraction(o, 1, 3);
            }
            break;
        case 2:
            if (D_800A604A < 0x14 && D_800A604E >= -0x3b) tryStartItemInteraction(o, 1, 0);
            break;
        }
        break;
    case 2:
        cameraEventScrollHandler(o);
        break;
    }
}

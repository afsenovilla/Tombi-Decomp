// FUNC 80115fc0 444 X011
// MATCHING 80115fc0 444
#include "spad.h"
#include "gvars.h"
#include "gamestate.h"
typedef struct { unsigned short frac; short whole; } FP;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x38 - 8];
    FP *p38;
} O;
typedef void (*Fn)(O *);

extern Fn D_80079AD0[];
extern void func_80027810(O *);
extern int tryStartItemInteraction(O *, int, int);
extern void FUN_8002a4d0(O *);

void func_80115FC0(O *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.b3a != 0) o->step++;
        o->p38->whole = 0;
        func_80027810(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (GAME.selectedSection) {
        case 1:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x14 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x3b) {
                tryStartItemInteraction(o, 1, 0);
                break;
            }
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x123) break;
            if ((*(short*)((char*)&SPAD.i16c + 2)) < -0x3b) break;
            if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x5b) break;
            tryStartItemInteraction(o, 1, 1);
            break;
        case 2:
            if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0x67) break;
            if ((*(short*)((char*)&SPAD.i16c + 2)) < -0x3b) break;
            tryStartItemInteraction(o, 1, 0);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}

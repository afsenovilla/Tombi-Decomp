// FUNC 80117f60 276 X016
// MATCHING 80117f60 276
#include "player.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;
extern void func_80117E04(TObj *);
extern void clockTowerRestoredTalk(TObj *);
extern int func_800202B4(TObj *);
extern void freeObjectLayer2(TObj *);

void func_80117F60(TObj *o)
{

    switch (o->b04) {
    case 0:
        func_80117E04(o);
        break;
    case 1:
        func_800202B4(o);
        switch (o->step) {
        case 0:
            if (o->h->p.whole < (*(S2 **)&PLAYER.obj.h)->s2) (*(S2 **)&PLAYER.obj.h)->s2 = o->h->p.whole;
            if (o->b68) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            clockTowerRestoredTalk(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}

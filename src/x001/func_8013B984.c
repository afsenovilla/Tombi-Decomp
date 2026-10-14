// FUNC 8013b984 300 X001
// MATCHING 8013b984 300
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"

extern TObj *allocObjectLayer2(void);
extern short func_80139B2C(TObj *);
extern void lostSeedKidTalk(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8013B984(TObj *o)
{
    TObj *e;

    switch (o->b04) {
    case 0:
        if (GCD.bcdb2 >= 2 || (GAME.area00_eventControl & 0x100)) {
            o->b04 = 3;
            break;
        }
        e = allocObjectLayer2();
        if (e) {
            e->active = 1;
            e->type = 0x16;
            e->b0a = 2;
            e->a.p.whole = 0x105b;
            e->y.p.whole = -0x506;
            e->animFrame = 0;
            e->subtype = 0;
            e->b0c = 0;
            e->b.p.whole = 0x47a;
            *(TObj **)&o->category = e;
        }
        o->b04 = 1;
        break;
    case 1:
        if (func_80139B2C(o)) lostSeedKidTalk(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}

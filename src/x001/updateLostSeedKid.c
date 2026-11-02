// FUNC 8013b984 300 X001
// MATCHING 8013b984 300
// VERSION SLPS_01144 x00 80136650 284
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned char D_8009EA65;
extern TObj *allocObjectLayer2(void);
extern short checkLostSeedKidTalk(TObj *);
extern void lostSeedKidTalk(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80136650(TObj *o)
{
    TObj *e;

    switch (o->b04) {
    case 0:
        if (GAME.area00_eventControl & 0x40)
            o->b04 = 3;
        if (D_8009EA65 >= 6) {
            o->b04 = 3;
            break;
        }
        e = allocObjectLayer2();
        if (e) {
            e->active = 1;
            e->type = 0x15;
            e->a.p.whole = 0x138;
            e->animFrame = 0;
            e->y.p.whole = -0x11c;
            e->b.p.whole = 0;
            *(TObj **)&o->category = e;
        }
        o->b04 = 1;
        break;
    case 1:
        if (checkLostSeedKidTalk(o)) lostSeedKidTalk(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
#endif
#else
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"

extern TObj *allocObjectLayer2(void);
extern short checkLostSeedKidTalk(TObj *);
extern void lostSeedKidTalk(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateLostSeedKid(TObj *o)
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
        if (checkLostSeedKidTalk(o)) lostSeedKidTalk(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
#endif

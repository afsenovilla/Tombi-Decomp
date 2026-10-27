// FUNC 800e9410 156 I0NFO2
// MATCHING 800e9410 156
#include "TOBJ.H"
extern void func_800202B4(TObj *o);
extern signed char GAME_PLAYER_MODE[];
extern unsigned char PLAYER_VISIBLE;

void topdownShadowFollowOwner(TObj *o)
{
    TObj *p;
    func_800202B4(o);
    p = (TObj *)o->d90;
    o->a.raw = p->a.raw;
    if (GAME_PLAYER_MODE[0] != 3) {
        o->y.raw = p->y.raw;
    }
    o->b.raw = p->b.raw;
    if (p->category == 0) o->visible = PLAYER_VISIBLE;
    else o->visible = p->visible;
    o->b0f = 0x10;
}

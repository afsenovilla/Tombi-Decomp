// FUNC 800eda48 140 I0NFO
// MATCHING 800eda48 140
#include "TOBJ.H"
extern void func_800202B4(TObj *o);
extern signed char GAME_PLAYER_MODE[];

void overheadShadowFollowOwner(TObj *o)
{
    TObj *p;
    unsigned char v;
    func_800202B4(o);
    p = (TObj *)o->d90;
    o->a.raw = p->a.raw;
    if (GAME_PLAYER_MODE[0] != 3) {
        o->y.raw = p->y.raw;
    }
    o->b.raw = p->b.raw;
    if (p->category == 0) v = 1;
    else v = p->visible;
    o->visible = v;
    o->b0f = 0x10;
}

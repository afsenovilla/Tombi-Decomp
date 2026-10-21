// FUNC 800ecc2c 68 I0NFO
// MATCHING 800ecc2c 68
#include "TOBJ.H"
extern int AnimAdvance(TObj *o);
extern void overheadCollideMapTile(TObj *o, int x, int y, int z);

void overheadActorAdvanceAndCollide(TObj *o)
{
    AnimAdvance(o);
    overheadCollideMapTile(o, o->h->p.whole, o->y.p.whole, o->animFrame * 2);
}

// FUNC 8012441c 180 X014
// MATCHING 8012441c 180
#include "TOBJ.H"

extern TObj *allocObjectLayer2(void);

int spawnTrickVillagePigShotB(TObj *o, unsigned char k)
{
    TObj *n = allocObjectLayer2();
    int x;

    if (n != 0) {
        n->active = 1;
        n->type = 0x4f;
        n->subtype = 0;
        n->b0c = 0;
        if (o->animFrame & 1) {
            x = o->a.p.whole - 0x10;
        } else {
            x = o->a.p.whole + 0x10;
        }
        n->a.raw = x << 16;
        n->y.raw = (o->y.p.whole - 4) << 16;
        n->b.raw = o->b.p.whole << 16;
        n->d8c = k;
    }
}

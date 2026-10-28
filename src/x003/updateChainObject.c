// FUNC 8011a544 384 X003
// MATCHING 8011a544 384
#include "TOBJ.H"
#include "gamestate.h"

extern void ObjFreeDup(TObj *);
extern void chainInitLinks(TObj *);
extern void chainSwingStep(TObj *);
extern void chainDrawLinks(TObj *);

void updateChainObject(TObj *o)
{
    TObj *e;
    int v;
    char pad[0x48];

    unsigned char s = o->b04;

    v = s;
    switch (v) {
    case 0:
        o->ba4 = 0;
        v = GAME.selectedSection;
        if (v < 0) goto d0;
        if (v < 2) {
            o->active = 2;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->velX = 0;
            e = (TObj *)o->d90;
            if (e) {
                o->d30 = o->a.raw - e->a.raw;
                o->d34 = o->y.raw - e->y.raw;
                o->d38 = o->b.raw - e->b.raw;
                if (o->d94 == 0) {
                    o->box0 = 0x14;
                    o->box1 = 0x28;
                    o->box2 = 0xf;
                    o->box3 = 0x1e;
                    o->active = 1;
                }
            }
        } else {
        d0:
            chainInitLinks(o);
        }
        o->b04++;
        break;
    case 1:
        v = GAME.selectedSection;
        if (v < 0) goto d1;
        if (v < 2) chainSwingStep(o);
        else { d1: chainDrawLinks(o); }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

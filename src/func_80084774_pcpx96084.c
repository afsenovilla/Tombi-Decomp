// VERSION PCPX_96084 tomba 80084774 244
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Area-0 event set-up by the event state D_800DE222: spawns the type-0x14 actor at one of two places (subtype
// 0 or 1); state 2 ends the controller (+4 = 3).
#include "tobj.h"
extern unsigned char D_800DE222;
extern TObj *func_8001A36C_pcpx96084(void);
void func_80084774_pcpx96084(TObj *o)
{
    TObj *n;
    switch (D_800DE222) {
    case 0:
        n = func_8001A36C_pcpx96084();
        if (n != 0) {
            n->active = 1;
            n->type = 0x14;
            n->a.p.whole = 0xa38;
            n->y.p.whole = -308;
            n->animFrame = 0;
            n->subtype = 0;
            n->b.p.whole = -40;
        }
        o->b04 = 1;
        break;
    case 1:
        n = func_8001A36C_pcpx96084();
        if (n != 0) {
            n->active = 1;
            n->type = 0x14;
            n->a.p.whole = 0xc29;
            n->y.p.whole = -735;
            n->animFrame = 0;
            n->subtype = 1;
            n->b.p.whole = 0x5a;
        }
        o->b04 = 1;
        break;
    case 2:
        o->b04 = 3;
        break;
    }
}

#endif

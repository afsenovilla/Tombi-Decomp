// VERSION PCPX_96084 tomba 80052160 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Player event step: clears the motion state, selects player animation 0x1E (loaded once) and then advances it.
#include "TOBJ.H"
typedef struct { char p0[0xc]; unsigned char b0c; char p1[0x2c - 0xd]; unsigned short cur, prev; } PX;
extern PX *D_800CCCCC;
extern void func_8004239C(TObj *);
extern void AnimLoadDuration(TObj *);
extern void func_800180D8_pcpx96084(TObj *);

void func_80052160_pcpx96084(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->wa8 = 0;
        D_800CCCCC->b0c = 0;
        o->b69 = 0;
        o->ba4 = 0;
        o->velY = 0;
        o->velV = 0;
        o->animFrame &= 1;
        D_800CCCCC->cur = 0x1e;
        if (D_800CCCCC->prev != 0x1e) {
            func_8004239C(o);
            AnimLoadDuration(o);
            D_800CCCCC->prev = D_800CCCCC->cur;
        }
        o->state++;
    case 1:
        func_800180D8_pcpx96084(o);
        break;
    }
}
#endif

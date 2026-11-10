// VERSION PCPX_96084 tomba 8003C178 236
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Proximity step: waits until the player is within 128 px horizontally, then winds +0x88 up to 2048 (by 8)
// until the player has left again.
#include "TOBJ.H"
typedef struct { short frac, whole; } F16;
extern F16 *D_800F5018;

void func_8003C178_pcpx96084(TObj *o)
{
    unsigned char s = o->state;
    switch (s) {
    case 0:
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->state++;
        break;
    case 1:
        if ((unsigned short)(o->h->p.whole - D_800F5018->whole + 128) < 256)
            o->state = s + 1;
        break;
    case 2:
        if (o->d88 < 2048) o->d88 += 8;
        if ((unsigned short)(o->h->p.whole - D_800F5018->whole + 128) > 256)
            o->state++;
        break;
    }
}
#endif

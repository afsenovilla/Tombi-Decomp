// FUNC 80116ae0 300 X004
// MATCHING 80116ae0 300
#include "TOBJ.H"
#include "raw7.h"

extern TObj PLAYER;
extern short D_1F80016A, D_1F800172;
extern unsigned short D_1F8001FC;
extern unsigned char D_8009C940;
extern unsigned char D_8009D2B0, D_8009C93F, D_8009C942;
extern int tryStartItemInteraction(TObj *, int, int);

static __inline__ short busy(void)
{
    if (U8(&PLAYER, 0xac) >= 2) return 1;
    if (PLAYER.b69 == 0) return 1;
    if (PLAYER.b9c | PLAYER.b9e | U8(&PLAYER, 0xaa) | D_8009C940) return 1;
    if (D_1F8001FC & 0x10) return 0;
    return 1;
}

void func_80116AE0(TObj *o)
{
    if (D_1F80016A >= 0x6d0 && D_1F800172 == 0x168) {
        if (tryStartItemInteraction(o, 1, 7)) {
            D_8009D2B0 = 0;
            D_8009C93F = 1;
            D_8009C942 = 1;
        }
        return;
    }
    if (busy()) return;
    if (D_1F80016A < 0x28a && D_1F800172 == 0x21c) {
        if (tryStartItemInteraction(o, 1, 8)) {
            D_8009D2B0 = 0;
            D_8009C93F = 1;
            D_8009C942 = 1;
        }
    }
}

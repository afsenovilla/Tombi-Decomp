// FUNC 80020b64 160 MAIN0
// MATCHING 80020b64 160
#include "gamestate.h"
typedef struct { unsigned short a; unsigned short pad[0x118]; unsigned short bits[2]; } S;
extern unsigned short D_80078790[];
extern S D_8009C962;

int testSectionFlagBit(int n)
{
    int i;
    int s = 0;
    int m;
    for (i = 0; i < GAME.selectedArea; i++) s += D_80078790[i];
    s += D_8009C962.a;
    if (n >= 32) s++;
    m = n % 32;
    return *(int *)&D_8009C962.bits[s * 2] & (1 << m);
}

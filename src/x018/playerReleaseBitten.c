// FUNC 800eee40 80 X018
// MATCHING 800eee40 80
#include "plx.h"
typedef struct { char pad[4]; unsigned char b4, b5, b6; } E;
extern E *D_8009D2E8;
typedef struct { char pad[0xac]; unsigned char bac; } O;

void playerReleaseBitten(O *o)
{
    if (o->bac >= 2) {
        D_8009D2E8 = (*(E **)&PLX.p611c);
        D_8009D2E8->b4 = 2;
        D_8009D2E8->b5 = 2;
        D_8009D2E8->b6 = 0;
    }
    o->bac = 0;
}

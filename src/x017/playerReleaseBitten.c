// FUNC 800eee40 80 X017
// MATCHING 800eee40 80
#include "plx.h"
typedef struct { char pad[4]; unsigned char b4, b5, b6; } E;
extern E *BITTEN_OBJECT;
typedef struct { char pad[0xac]; unsigned char bac; } O;

void playerReleaseBitten(O *o)
{
    if (o->bac >= 2) {
        BITTEN_OBJECT = (*(E **)&PLX.p611c);
        BITTEN_OBJECT->b4 = 2;
        BITTEN_OBJECT->b5 = 2;
        BITTEN_OBJECT->b6 = 0;
    }
    o->bac = 0;
}

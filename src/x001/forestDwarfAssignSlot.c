// FUNC 80135a14 308 X001
// MATCHING 80135a14 308
#include "gcd.h"
#include "TOBJ.H"

extern int Rand(void);

void forestDwarfAssignSlot(TObj *o)
{
    short i;
    char pad[16];

    i = 0;
    while (((unsigned char*)&GCD.bcec1)[i] && i < 7) i++;
    if (i >= 7) {
        ((unsigned char*)&GCD.bcec1)[o->b0c + 11] = (Rand() & 1) - 0x64;
    } else {
        ((unsigned char*)&GCD.bcec1)[o->b0c + 11] = i - 0x6c;
        ((unsigned char*)&GCD.bcec1)[i] = 1;
    }
    for (i = 0; i < 7; i++) {
        if (((unsigned char*)&GCD.bcec1)[i + 12] == o->b0c + 0x93) ((unsigned char*)&GCD.bcec1)[i + 12] = (Rand() & 1) - 0x64;
    }
}

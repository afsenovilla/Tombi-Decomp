// FUNC 800312e8 224 MAIN0
// MATCHING 800312e8 224
#include "raw7.h"
extern void *p605c;
extern char a0[], a1[], a2[], b0[], b1[], b2[];
void FUN_800312e8(char *o)
{
    int s = U8(o, 5);
    if (s != 1) {
        if (s < 2 && s == 0) {
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: p605c = a0; break;
        case 4: case 5: p605c = a1; break;
        case 6: case 7: p605c = a2;
        }
        }
    } else {
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: p605c = b0; break;
        case 4: case 5: p605c = b1; break;
        case 6: case 7: p605c = b2;
        }
    }
}

// FUNC 8011ef98 196 X010
// MATCHING 8011ef98 196
#include "gv2.h"
#include "TOBJ.H"
extern int probeTiltedBarContact(TObj *, TObj *, int, int);

int probeTiltedBarObject(TObj *a, TObj *b)
{
    if (!(GV2.bc3 & 0x40)) return 0;
    switch (b->subtype) {
    case 0:
        return probeTiltedBarContact(a, b, 0, b->d30);
    case 1:
    case 2:
        if (probeTiltedBarContact(a, b, 1, b->d30)) return 1;
        return probeTiltedBarContact(a, b, 2, (b->d30 + 0x400) & 0xfff);
    default:
        return probeTiltedBarContact(a, b, 2, b->d30);
    }
}

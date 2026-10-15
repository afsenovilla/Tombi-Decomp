// FUNC 8011f518 156 X010
// MATCHING 8011f518 156
#include "TOBJ.H"

extern int collideObjectWithTiltedBar(TObj *, TObj *, int, int);

void turningPlankObjectCollision(TObj *a, TObj *b)
{
    switch (b->subtype) {
    case 0:
        collideObjectWithTiltedBar(a, b, 0, b->d30);
        break;
    case 1:
    case 2:
        if (!collideObjectWithTiltedBar(a, b, b->subtype, b->d30))
            collideObjectWithTiltedBar(a, b, 2, (b->d30 + 0x400) & 0xfff);
        break;
    default:
        collideObjectWithTiltedBar(a, b, 2, b->d30);
        break;
    }
}

// FUNC 80125868 124 X001
// MATCHING 80125868 124
#include "TOBJ.H"

extern int collideObjectWithTiltedBar(TObj *, TObj *, int, int);

void seesawPlankObjectCollision(TObj *a, TObj *b)
{
    if (b->subtype == 0) {
        collideObjectWithTiltedBar(a, b, 0, b->d8c);
    } else if (!collideObjectWithTiltedBar(a, b, b->subtype, b->d8c)) {
        collideObjectWithTiltedBar(a, b, 2, (b->d8c + 0x400) & 0xfff);
    }
}

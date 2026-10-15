// FUNC 80124ba8 120 X001
// MATCHING 80124ba8 120
#include "TOBJ.H"
extern void collidePlayerWithTiltedBar(TObj *, TObj *, int, int);

void seesawPlankPlayerCollision(TObj *a, TObj *b)
{
    if (b->subtype == 0) {
        collidePlayerWithTiltedBar(a, b, 0, b->d8c);
    } else {
        collidePlayerWithTiltedBar(a, b, b->subtype, b->d8c);
        collidePlayerWithTiltedBar(a, b, 2, (b->d8c + 0x400) & 0xfff);
    }
}

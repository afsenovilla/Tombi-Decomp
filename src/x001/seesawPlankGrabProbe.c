// FUNC 8012767c 132 X001
// MATCHING 8012767c 132
#include "TOBJ.H"

extern int seesawPlankProbeFace(TObj *, TObj *, int, int);

int seesawPlankGrabProbe(TObj *a, TObj *b)
{
    if (b->subtype == 0) return seesawPlankProbeFace(a, b, 0, b->d8c);
    if (seesawPlankProbeFace(a, b, b->subtype, b->d8c)) return 1;
    return seesawPlankProbeFace(a, b, 2, (b->d8c + 0x400) & 0xfff);
}

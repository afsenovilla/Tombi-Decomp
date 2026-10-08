// FUNC 8001fbac 104 MAIN0
// MATCHING 8001fbac 104
#include "TOBJ.H"
extern short MulCosDup(short a, short b);
extern short MulNegSin(short a, short b);

void func_8001FBAC(TObj *o, short a, short b)
{
    short c, s;
    c = MulCosDup(a, b);
    s = MulNegSin(a, b);
    o->velH = c;
    o->velV = s;
}

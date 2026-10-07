// FUNC 8001fc14 104 MAIN0
// MATCHING 8001fc14 104
#include "TOBJ.H"
extern int MulCos(int a, int b);
extern int MulNegSinScaled(int a, int b);

void FUN_8001fc14(TObj *o, short a, short b)
{
    int c = MulCos(a, b);
    int s = MulNegSinScaled(a, b);
    o->velH = c;
    o->velV = s;
}

// FUNC 8001fc14 104 MAIN0
// MATCHING 8001fc14 104
#include "TOBJ.H"
extern int MulCos(int a, int b);
extern int MulNegSinScaled(int a, int b);

void setObjectSpeedPolar2(TObj *o, short a, short b)
{
    int c = MulCos(a, b);
    int s = MulNegSinScaled(a, b);
#if defined(VERSION_PCPX_96084) /* PrePre 8: the speed pair sits 4 bytes lower */
    *(short *)((char *)o + 0x7c) = c;
    *(short *)((char *)o + 0x7e) = s;
#else
    o->velH = c;
    o->velV = s;
#endif
}

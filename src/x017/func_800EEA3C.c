// FUNC 800eea3c 64 X017
// MATCHING 800eea3c 64
#include "TOBJ.H"

void func_800EEA3C(TObj *o)
{
    short v = o->wb0;
    if (v < 0) o->wb6 = (short)((v << 2) + 0x100) & 0xff;
    else if (v > 0) o->wb6 = (short)(v << 2) & 0xff;
    else o->wb6 = 0;
}

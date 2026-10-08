// FUNC 800eea3c 64 X000
#include "TOBJ.H"

void func_800EEA3C(TObj *o)
{
    short v;
    short r;
    char pad[4];
    v = o->wb0;
    r = v;
    if (v < 0) {
        r = r * 4 + 0x100;
        o->wb6 = r & 0xff;
    } else if (v > 0) {
        r = r * 4;
        o->wb6 = r & 0xff;
    } else {
        o->wb6 = 0;
    }
}

// FUNC 80110eec 240 X000
#include "TOBJ.H"

void FUN_80110eec(TObj *o)
{
    int v; short w;
    short n;
    char pad[16];
    switch (o->bbe & 1) {
    case 0:
        v = o->wb2; w = v;
        if (v < -0x144) n = w + 0x20;
        else {
            n = w + 0x22;
            if (v > -0x85) {
                n = w + 0x26;
                if (v >= 0) {
                    if (v == 0) n = 0x88;
                    else {
                        n = w + 0x10;
                        if (v > 0x143) {
                            n = w + 0x20;
                            if (v > 0x4ff) n = 0x500;
                        }
                    }
                }
            }
        }
        break;
    case 1:
        v = o->wb2; w = v;
        if (v >= 0x145) n = w - 0x20;
        else {
            n = w - 0x22;
            if (v < 0x85) {
                n = w - 0x26;
                if (v <= 0) {
                    if (v == 0) n = -0x88;
                    else if (v >= -0x143) n = w - 0x10;
                    else {
                        n = -0x500;
                        if (v > -0x500) n = w - 0x20;
                    }
                }
            }
        }
        break;
    default: return;
    }
    o->wb2 = n;
}

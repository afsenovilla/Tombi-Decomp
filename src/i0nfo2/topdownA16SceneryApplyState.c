// FUNC 800f0a88 136 I0NFO2
// MATCHING 800f0a88 136
#include "TOBJ.H"
extern unsigned char D_8009CFA8;

void topdownA16SceneryApplyState(TObj *o)
{
    switch (D_8009CFA8) {
    case 0:
    case 1:
    case 4:
        break;
    case 2:
        if ((unsigned short)o->wb4 != 0xB) {
            o->_pad0e[0] = 2;
            o->wb4 = 0xB;
            o->a.p.whole = -4;
            o->y.p.whole = 0x193;
        }
        break;
    case 3:
        if ((unsigned short)o->wb4 != 0xC) {
            o->_pad0e[0] = 2;
            o->wb4 = 0xC;
            o->a.p.whole = -3;
            o->y.p.whole = 0x193;
        }
        break;
    }
}

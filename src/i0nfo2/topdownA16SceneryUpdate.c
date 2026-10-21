// FUNC 800f0b10 252 I0NFO2
// MATCHING 800f0b10 252
#include "TOBJ.H"
extern void func_80018CA4(TObj *o);
extern unsigned char D_8009CFA8, D_800A60A3;

void topdownA16SceneryUpdate(TObj *o)
{
    if ((unsigned short)o->wb8 == 5) {
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
    if (D_800A60A3 != 0) {
        if (D_800A60A3 == *(unsigned char *)&o->wb6) o->b6b = 0x80;
        else o->b6b = 0;
    } else {
        if ((unsigned short)o->wb6 != 0) o->b6b = 0;
        else o->b6b = 0x80;
    }
    o->visible = 1;
    func_80018CA4(o);
}

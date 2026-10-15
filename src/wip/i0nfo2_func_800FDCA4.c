// FUNC 800fdca4 396 I0NFO2
#include "TOBJ.H"
#include "raw7.h"
extern TObj *func_800183B8(void);
extern TObj *D_8009F2D8[];
typedef struct { short s[14]; } EntDCA4;
extern EntDCA4 *D_801008D8;

void func_800FDCA4(int arg)
{
    EntDCA4 *base = D_801008D8;
    short *e;
    short i;
    TObj *n;
    for (i = 0; *(e = base[i].s) != 0xFF; i++) {
        n = func_800183B8();
        if (n != 0) {
            n->active = 1;
            n->type = 0x30;
            n->b0a = 0xD;
            n->animFrame = e[2];
            n->subtype = e[0xC];
            n->b0c = e[0xD];
            n->_pad0e[0] = e[0];
            n->a.p.whole = e[0xA];
            n->b.p.whole = 0;
            n->y.p.whole = e[0xB];
            S16(n, 0xB4) = e[3];
            S16(n, 0xB6) = e[1];
            S16(n, 0xB8) = e[9];
            S16(n, 0xBA) = e[8];
            S16(n, 0x74) = e[4];
            S16(n, 0x76) = e[5];
            S16(n, 0x78) = e[6];
            S16(n, 0x7A) = e[7];
            D_8009F2D8[i] = n;
            n->d90 = arg;
        }
    }
}

// FUNC 800fa670 476 I0NFO2
#include "TOBJ.H"
#include "raw7.h"
extern TObj *func_800183B8(void);
extern unsigned char D_8009CF01;
extern unsigned char D_8009CFC9;
extern TObj *D_8009F2D8[];
typedef struct { short s[14]; } EntA670;
extern EntA670 *D_800FFE20;

void func_800FA670(TObj *o)
{
    EntA670 *base = D_800FFE20;
    short *e;
    short i;
    TObj *n;
    o->b04 = 1;
    if (D_8009CFC9 != 0) {
        o->b04 = 3;
        return;
    }
    e = base->s;
    i = 0;
    while (e[0] != 0xFF) {
        if (i == 1 && D_8009CF01 != 0) {
        } else if ((n = func_800183B8()) != 0) {
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
            n->d90 = (int)o;
        }
        i++;
        e = base[i].s;
    }
    o->step = 1;
}

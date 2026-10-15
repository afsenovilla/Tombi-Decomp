// FUNC 800ee260 436 I0NFO3
#include "TOBJ.H"
#include "raw7.h"
extern TObj *func_800183B8(void);
extern unsigned char D_8009CE34;
extern TObj *D_8009F2D8[];
typedef struct { short s[14]; } EntE260;
extern EntE260 *D_800EEF30;

void func_800EE260(TObj *o)
{
    EntE260 *base = D_800EEF30;
    short *e;
    short i;
    TObj *n;
    e = base->s;
    i = 0;
    if (e[0] != 0xFF) do {
        if (e[8] < 6 && e[8] >= 4 && D_8009CE34 != 0) goto next;
        n = func_800183B8();
        if (n != 0) {
            n->active = 1;
            n->type = 0x45;
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
    next:
        i++;
        e = base[i].s;
    } while (e[0] != 0xFF);
}

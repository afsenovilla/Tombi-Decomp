// FUNC 800ea44c 236 I0NFO2
#include "TOBJ.H"
extern volatile unsigned short D_8009D670[];

void func_800EA44C(TObj *o)
{
    volatile unsigned short *pad;
    unsigned short d, f;
    if (*(unsigned char *)&o->wac < 2) {
        o->animFrame = f = o->animFrame & 7;
        pad = D_8009D670;
        if (*pad & 0x10) {
            d = 1;
            if (!(*pad & 0x20)) {
                d = 3;
                if (!(*pad & 0x80)) d = 2;
            }
        } else if (*pad & 0x40) {
            d = 7;
            if (!(*pad & 0x20)) {
                d = 5;
                if (!(*pad & 0x80)) d = 6;
            }
        } else {
            d = 4;
            if (!(*pad & 0x80)) {
                d = f | 8;
                if (*pad & 0x20) {
                    o->animFrame = 0;
                    return;
                }
            }
        }
        o->animFrame = d;
    }
}

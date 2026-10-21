// FUNC 800e8c60 324 I0NFO3
// MATCHING 800e8c60 324
#include "TOBJ.H"
extern unsigned char D_8009CE21;

void tunnelGateStepEvent7D(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CE21 == 0xFF) {
            o->timer = 0x10;
            o->step++;
        }
        break;
    case 1:
        if (o->b0c == 0) {
            if ((unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 0x20) < 0x41
                && (unsigned short)(o->y.p.whole - *(unsigned short *)0x1F80016E + 0x30) < 0x61) {
                o->timer = 0x10;
                o->step++;
            }
        } else {
            o->timer = 0x10;
            o->step = ((TObj *)o->d90)->step;
        }
        break;
    case 2:
        if (o->b0c == 0) o->d8c += 0x40;
        else o->d8c -= 0x40;
        if (--o->timer == -1) o->step++;
        break;
    case 3:
        break;
    }
}

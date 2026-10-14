// FUNC 80135b0c 676 X000
// MATCHING 80135b0c 676
#include "spad.h"
#include "TOBJ.H"
typedef struct { short x, y; } P;
extern int angleBetweenPoints(P a, P b);
extern short D_8007A5F0[];
extern short D_8007A3F0[];

void func_80135B0C(TObj *o)
{
    P a, b;
    unsigned d;
    switch (o->step) {
    case 0:
        o->d84 = 0x80;
        o->b0a = 2;
        o->velH = 0x80;
        o->d88 = 0;
        o->d8c = 0;
        o->animFrame = 1;
        o->step++;
        break;
    case 1:
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        b.x = (*(unsigned short*)((char*)&SPAD.i168 + 2));
        b.y = (*(unsigned short*)((char*)&SPAD.i16c + 2));
        o->d88 = angleBetweenPoints(a, b);
        o->timer = 0x78;
        o->step++;
        break;
    case 2:
        d = (o->d84 - o->d88) & 0xff;
        if (d != 0) {
            if (d < 0x80) o->d84 = (o->d84 - 1) & 0xff;
            else o->d84 = (o->d84 + 1) & 0xff;
        }
        if ((unsigned)((o->d84 - 0x40) & 0xff) < 0x80) {
            o->animFrame = 0;
            o->d8c = (o->d84 + 0x80) & 0xff;
        } else {
            o->animFrame = 1;
            o->d8c = o->d84;
        }
        o->h->raw += (D_8007A5F0[(unsigned char)o->d84] * o->velH) >> 4;
        o->y.raw += (D_8007A3F0[(unsigned char)o->d84] * o->velH) >> 4;
        if (--o->timer == -1) o->step = 1;
        if ((unsigned)((unsigned short)o->h->p.whole - 0x14d) >= 0x10c) o->step++;
        else if ((unsigned short)(o->y.p.whole + 0xc8) >= 0x74) o->step++;
        break;
    case 3:
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        b.x = 0x1ec;
        b.y = -0x87;
        o->d88 = angleBetweenPoints(a, b);
        o->timer = 0x78;
        o->step--;
        break;
    }
}

// FUNC 8011b460 296 X003
// MATCHING 8011b460 296
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"

extern unsigned char PLAYER;
extern unsigned char D_800A60E4;
extern int SquareRoot0(int);
extern int angleFromDelta(int, int);

int func_8011B460(TObj *o)
{
    int dx, dy;
    int a;

    if (SPAD.b1a4 != 0) goto ret0;
    if (PLAYER != 1) return 0;
    if (D_800A60E4 == 2) return 0;
    if (GVARS.b40 != 0) return 0;
    dx = (*(short*)((char*)&SPAD.i170 + 2)) - o->d->p.whole + 0x2d;
    if ((unsigned short)dx >= 0x5b) return 0;
    dx = (*(short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole;
    dy = (*(short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole;
    a = angleFromDelta(dx, dy);
    o->wb0 = (unsigned char)a;
    if ((unsigned char)(a - o->d38 + 0x30) >= 0x61) {
    ret0:
        return 0;
    }
    return SquareRoot0(dx * dx + dy * dy) < 0x29;
}

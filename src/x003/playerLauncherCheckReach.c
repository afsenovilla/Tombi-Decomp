// FUNC 8011b324 316 X003
// MATCHING 8011b324 316
#include "spad.h"
#include "player.h"
#include "TOBJ.H"

extern short D_8007A1F0[], D_8007A5F0[];
extern int *D_800A6078;
extern int SquareRoot0(int);
extern int angleFromDelta(int, int);

int playerLauncherCheckReach(TObj *o)
{
    int dx, dy;
    int a;

    dx = (*(short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole;
    dy = (*(short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole;
    if (SquareRoot0(dx * dx + dy * dy) > 0x10) {
    a = (unsigned char)angleFromDelta(dx, dy);
    o->wb0 = a;
    a += 0x80;
    a &= 0xff;
    dy = (o->velH * D_8007A1F0[a]) >> 12;
    dx = (o->velH * D_8007A5F0[a]) >> 12;
    *D_800A6078 += dx << 8;
    ((int*)&PLAYER.obj.y.raw)[0] += dy << 8;
    o->velH += 0x80;
    if (o->velH > 0x800) o->velH = 0x800;
    return 0;
    }
    return 1;
}

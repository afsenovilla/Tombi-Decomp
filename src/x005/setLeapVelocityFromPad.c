// FUNC 800fbcec 200 X005
// MATCHING 800fbcec 200
#include "gv6.h"
#include "TOBJ.H"
#define PAD (*(volatile unsigned short *)&GV6.s70)

void setLeapVelocityFromPad(TObj *o)
{
    if (o->animFrame & 1) {
        o->velX = -0x300;
        o->velY = -0x600;
        if (PAD & 0x80) {
            o->velX = -0x480;
            o->velY = -0x800;
        }
        if (PAD & 0x20) {
            o->velX = -0x200;
            o->velY = -0x400;
        }
    } else {
        o->velX = 0x300;
        o->velY = -0x600;
        if (PAD & 0x80) {
            o->velX = 0x200;
            o->velY = -0x400;
        }
        if (PAD & 0x20) {
            o->velX = 0x480;
            o->velY = -0x800;
        }
    }
}

// FUNC 800fbcec 200 X013
// MATCHING 800fbcec 200
#include "TOBJ.H"
extern unsigned short D_8009D670;
#define PAD (*(volatile unsigned short *)&D_8009D670)

void func_800FBCEC(TObj *o)
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

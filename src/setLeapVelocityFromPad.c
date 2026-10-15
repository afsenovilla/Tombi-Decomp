// FUNC 800fbcec 200 X000
// MATCHING 800fbcec 200
// VERSION SLPS_01144 x00 800FB95C 200
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 800FBCEC (setLeapVelocityFromPad.c): 128 -> 32768, 32 -> 8192.
#include "gv6.h"
#include "TOBJ.H"
#define PAD (*(volatile unsigned short *)&GV6.s70)

void setLeapVelocityFromPad(TObj *o)
{
    if (o->animFrame & 1) {
        o->velX = -0x300;
        o->velY = -0x600;
        if (PAD & 0x8000) {
            o->velX = -0x480;
            o->velY = -0x800;
        }
        if (PAD & 0x2000) {
            o->velX = -0x200;
            o->velY = -0x400;
        }
    } else {
        o->velX = 0x300;
        o->velY = -0x600;
        if (PAD & 0x8000) {
            o->velX = 0x200;
            o->velY = -0x400;
        }
        if (PAD & 0x2000) {
            o->velX = 0x480;
            o->velY = -0x800;
        }
    }
}
#else
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
#endif

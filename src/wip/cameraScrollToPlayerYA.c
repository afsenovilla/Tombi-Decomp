// FUNC 80028754 368 MAIN0
#include "spad.h"
#include "player.h"
/* w1: score 97 (was 114) with the body as an inline taking (o, short d), which creates the 0x18 frame.
   left: subu sp lands in the bne delay slot (game: sp first, move t0,a0 in the slot) and o is not copied to t0.
   old diff: game has a 0x18 frame with no saves (sp first), o moved to t0, d copied a1; scratchpad accesses via symbols */
#define F2 (*(short*)((char*)&SPAD.if0 + 2))
#define E6 SPAD.se6
static __inline__ void inl(unsigned char *o, short d)
{
    short e;
    short s;
    short *p;
    p = &SPAD.se6;
    e = *p;
    s = d + e;
    if (s != 0x3a) {
        if (s < 0x3a) {
            if (s < -6) {
                F2 += 2;
                if (*(short *)(o + 0x32) < F2) F2 = *(short *)(o + 0x32);
                else d += 2;
            } else {
                *p = e + 2;
            }
            p = &SPAD.se6;
            if (d + *p > 0x3a) *p = 0x3a - d;
        } else {
            e -= 2;
            *p = e;
            if (d + e < 0x3a) *p = 0x3a - d;
        }
    }
    if (*(short *)(o + 0x32) < F2 + E6) E6 = *(short *)(o + 0x32) - F2;
}
void cameraScrollToPlayerYA(unsigned char *o)
{
    int t;
    if (PLAYER.obj.b9e == 3) { t = (unsigned short)F2 - 0x14; t = t - PLAYER.obj.d34; }
    else t = (unsigned short)F2 - (unsigned short)(*(short*)((char*)&SPAD.i16c + 2));
    inl(o, t);
}

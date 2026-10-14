// FUNC 80116758 192 X000
// MATCHING 80116758 192
#include "gcd.h"
#include "gamestate.h"
typedef struct { char p[2]; unsigned short s2; } P;
typedef struct { char p[0x32]; short w32; P *p34; } S;
void func_80116758(S *o)
{
    short t;
    if (GAME.selectedSection == 1) {
        t = o->p34->s2 - 0x948;
        if ((unsigned short)t >= 0x69) {
            if (t >= 0) o->w32 = -0x154;
        } else if (t != 0) {
            o->w32 += -(t * 190) / 104;
        }
    } else if (GAME.selectedSection == 5) {
        if (GCD.bce3d == 0xff) o->w32 = -0x54;
    }
}

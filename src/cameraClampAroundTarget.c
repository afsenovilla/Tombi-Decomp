// FUNC 8002771c 220 MAIN0
// MATCHING 8002771c 220
#include "gamestate.h"
typedef struct { unsigned char b0; char p[0x15]; short w16; } S;
typedef struct { short w0; short w2; } P;
typedef struct { char p0[0x30]; short w30; short w32; P *d34; } O;
extern S PLAYER;
extern P *D_800A6078;

void cameraClampAroundTarget(O *o)
{
    S *s = &PLAYER;
    char pad;
    if (s->b0 < 4 || s->b0 == 7) {
        unsigned short x = o->d34->w2;
        short t;
        t = x - 0x90;
        if (D_800A6078->w2 < t) {
            D_800A6078->w2 = t;
        } else {
            t = x + 0x90;
            if (t < D_800A6078->w2)
                D_800A6078->w2 = t;
        }
        if ((*(int*)&GAME.selectedArea) == 0x2000E) {
            if (s->w16 < o->w30 - 0x5c)
                s->w16 = o->w30 - 0x5c;
        } else {
            if (s->w16 < o->w30 - 0x90)
                s->w16 = o->w30 - 0x90;
        }
    }
}

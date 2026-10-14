// FUNC 801196a4 624 X014
// MATCHING 801196a4 624
#include "spad.h"
#include "TOBJ.H"

extern int Rand(void);
extern void playSFX(int);

static __inline__ int add(int a, int b)
{
    return a + b;
}

unsigned char func_801196A4(TObj *o, int d)
{
    int t;
    int r;

    switch ((unsigned char)d) {
    case 0:
        if ((*(short*)((char*)&SPAD.i174 + 2)) + 300 < o->h->p.whole) {
            t = 8;
            t -= d;
            t += Rand() & 3;
            d = t & 0xf;
            playSFX(7);
        }
        break;
    case 1:
    case 2:
    case 3:
        if ((*(short*)((char*)&SPAD.i174 + 2)) + 300 < o->h->p.whole) {
            t = 8;
            t -= d;
            r = (Rand() & 3) - 2;
            t += r;
            d = t & 0xf;
            playSFX(7);
        } else if (o->y.p.whole < (*(short*)&SPAD.s186) - 200) {
            d = -d & 0xf;
            playSFX(7);
        }
        break;
    case 4:
        if (o->y.p.whole < (*(short*)&SPAD.s186) - 200) {
            d = add(-d, Rand() & 1) & 0xf;
            playSFX(7);
        }
        break;
    case 5:
    case 6:
    case 7:
        if ((*(short*)((char*)&SPAD.i174 + 2)) + 20 > o->h->p.whole) {
            d = (8 - d) & 0xf;
            playSFX(7);
        } else if (o->y.p.whole < (*(short*)&SPAD.s186) - 200) {
            d = -d & 0xf;
            playSFX(7);
        }
        break;
    case 8:
        if ((*(short*)((char*)&SPAD.i174 + 2)) + 20 > o->h->p.whole) {
            t = 8;
            t -= d;
            t += Rand() & 1;
            d = t & 0xf;
            playSFX(7);
        }
        break;
    case 9:
    case 10:
    case 11:
        if ((*(short*)((char*)&SPAD.i174 + 2)) + 20 > o->h->p.whole) {
            t = 8;
            t -= d;
            r = (Rand() & 3) - 2;
            t += r;
            d = t & 0xf;
            playSFX(7);
        } else if ((*(short*)&SPAD.s186) - 40 < o->y.p.whole) {
            d = -d & 0xf;
            playSFX(7);
        }
        break;
    case 12:
        if ((*(short*)&SPAD.s186) - 40 < o->y.p.whole) {
            d = add(-d, Rand() & 0x10) & 0xf;
            playSFX(7);
        }
        break;
    case 13:
    case 14:
    case 15:
        if ((*(short*)((char*)&SPAD.i174 + 2)) + 300 < o->h->p.whole) {
            d = (8 - d) & 0xf;
            playSFX(7);
        } else if ((*(short*)&SPAD.s186) - 40 < o->y.p.whole) {
            d = -d & 0xf;
            playSFX(7);
        }
        break;
    }
    return d;
}

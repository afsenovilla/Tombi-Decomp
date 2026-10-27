// FUNC 8010c8d4 324 X019
// MATCHING 8010c8d4 324
#include "TOBJ.H"
extern unsigned char *GRABBED_OBJECT;
extern int GAME;
extern int GAME_AREA00_EVENT_CONTROL;
extern unsigned short PAD_PRESSED;
extern unsigned short BUTTON_MASK_A;

static __inline__ void turn(TObj *o)
{
    Fix16 *p = o->h; int t = o->animFrame & 1; int h = p->p.whole; if (t) t = h + 0x10; else t = h - 0x10; p->p.whole = t;
}

void playerClimbJumpOff(TObj *o)
{
    volatile unsigned short *k;
    Fix16 *p;
    int h, t;
    char *c = (char *)o;
    *(signed char *)(c + 0xf) = -8;
    c[0xa9] = 0;
    *GRABBED_OBJECT = 1;
    *(short *)(c + 0x20) = 6;
    c[0x9c] = 1;
    *(short *)(c + 0x80) = 0;
    *(short *)(c + 0x82) = 0;
    *(short *)(c + 0x7c) = 0;
    *(short *)(c + 0x7e) = 0;
    if (GAME == 0x30000) {
        turn(o);
    } else {
        k = &PAD_PRESSED;
        if (*k & 0x80) {
            h = o->h->p.whole; o->h->p.whole = h - 0x10;
        } else if (*k & 0x20) {
            h = o->h->p.whole; o->h->p.whole = h + 0x10;
        } else {
            turn(o);
        }
    }
    if ((GAME_AREA00_EVENT_CONTROL & 0x40) && (*(volatile unsigned short *)&PAD_PRESSED & BUTTON_MASK_A))
        o->ba7 = 1;
    o->step = 2;
    o->state = 0;
}

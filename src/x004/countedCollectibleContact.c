// FUNC 8011f9e0 148 X004
// MATCHING 8011f9e0 148
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    TObj o;
    char pc0[0xe4 - 0xc0];
    TObj *de4;
} PL;
#define BAC(o) (*(unsigned char *)&(o)->o.wac)
extern short playerPushOutOfObjectBox(PL *, TObj *);

void countedCollectibleContact(PL *o, TObj *p)
{
    short r;
    if (p->b0c != 0) return;
    r = playerPushOutOfObjectBox(o, p);
    if (r == 0) return;
    if (BAC(o) == 1 && r == 1) {
        p->active = 2;
        p->b04 = 2;
        p->step = 1;
        p->state = 0;
        p->b69 = 0;
        o->de4 = p;
        BAC(o) = 2;
    }
    SPAD.s19e = 0;
}

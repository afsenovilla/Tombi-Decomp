// JP x04 8011E4E8 148
// Japanese version of PAL x004 8011F9E0 (x004/func_8011F9E0.c): 228 -> 212.
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    TObj o;
    char pc0[0xd4 - 0xc0];
    TObj *de4;
} PL;
#define BAC(o) (*(unsigned char *)&(o)->o.wac)
extern short func_8004306C(PL *, TObj *);

void func_8011F9E0(PL *o, TObj *p)
{
    short r;
    if (p->b0c != 0) return;
    r = func_8004306C(o, p);
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

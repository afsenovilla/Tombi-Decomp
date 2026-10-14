// FUNC 801167f0 412 X005
// MATCHING 801167f0 412
#include "spad.h"
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern int showMessageBoxDirect(int, int, V6 *);

void townGuideTalk(TObj *o)
{
    V6 v;
    TObj *q;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        if (GV2.bc3 & 0x10) {
            o->d90 = showMessageBoxDirect(2, 0x58, &v);
        } else if (GCD.bcdd5 == 0xff) {
            o->d90 = showMessageBoxDirect(2, 0x4e, &v);
        } else {
            o->d90 = showMessageBoxDirect(2, 0x3c, &v);
        }
        GVARS.b42 = 1;
        GV2.bb0 = 2;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->state++;
        break;
    case 1:
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        SPAD.s1c6 = 0;
        GVARS.b42 = 0;
        PLAYER.obj.wb2 = 0;
        if (GVARS.b3f == 0) {
            PLAYER.obj.b04 = 1;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
        }
        o->animFrame = o->wbc;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

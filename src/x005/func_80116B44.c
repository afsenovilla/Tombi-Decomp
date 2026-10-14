// FUNC 80116b44 436 X005
// MATCHING 80116b44 436
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern short D_1F8001C6;
extern int showMessageBoxDirect(int, int, V6 *);
extern void FUN_80026c50(int, int, int);

void func_80116B44(TObj *o)
{
    V6 v;
    TObj *p;
    unsigned char *f;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        if (GCD.bd008) {
            o->d90 = showMessageBoxDirect(2, 0x66, &v);
        } else {
            o->d90 = showMessageBoxDirect(2, 0x65, &v);
        }
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GV2.bb0)[0] = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        f = &GCD.bd008;
        if (*f == 0) {
            FUN_80026c50(0x41, 1, 1);
            *f = 1;
        }
        D_1F8001C6 = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        if (((unsigned char*)&GVARS.b3f)[0] == 0) {
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        }
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        o->animFrame = o->wbc;
        break;
    }
}

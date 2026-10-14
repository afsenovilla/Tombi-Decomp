// FUNC 80116690 352 X005
// MATCHING 80116690 352
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009D2B0;
extern short D_1F8001C6;
extern int showMessageBoxDirect(int, int, V6 *);

void func_80116690(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 0x33, &v);
        ((unsigned char*)&GVARS.b42)[0] = 1;
        D_8009D2B0 = 2;
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
        D_1F8001C6 = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        if (GVARS.b3f == 0) {
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

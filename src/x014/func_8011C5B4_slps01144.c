// VERSION SLPS_01144 x14 8011C5B4 684
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

typedef struct { short v[6]; } V6;
static short D_80123048[8] = {
    0, 6, 5, 2, 4, 3, 1, 7,
};
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern TObj *showMessageBoxDirect(int, int, V6 *);

static __inline__ void SetAnim(TObj *o, short n)
{
    unsigned char *p;
    p = (unsigned char *)o->d90;
    p += n * 4;
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    o->wac = n;
    o->anim = (*(void ***)&o->wa8)[n];
    AnimLoadDuration(o);
}

void func_8011C5B4_slps01144(TObj *o)
{
    V6 v;
    TObj *e;

    ObjCullRegister(o);
    AnimAdvance(o);
    switch (o->state) {
    case 0:
        ObjSetFacingToPlayer(o);
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        o->timer = 0x3c;
        if (((unsigned char*)&GCD.bcf0b)[((unsigned short*)&GAME.selectedSection)[0]] == 2) {
            o->state = 4;
        } else {
            ((unsigned char*)&GCD.bcf0b)[((unsigned short*)&GAME.selectedSection)[0]] = 2;
            o->state++;
        }
        break;
    case 1:
        if (--o->timer == -1) {
            o->state++;
            SetAnim(o, 0x13);
        }
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->da0 = (int)showMessageBoxDirect(2, D_80123048[((unsigned short*)&GAME.selectedSection)[0]], &v);
        o->state++;
        break;
    case 3:
        e = (TObj *)o->da0;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->timer = 0x3c;
            o->state++;
        }
        break;
    case 4:
        if (--o->timer == -1) {
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            o->b04++;
            o->step = 0;
            o->state = 0;
            if (o->subtype == 0) SetAnim(o, 0);
            else SetAnim(o, 2);
        }
        break;
    }
}
#endif

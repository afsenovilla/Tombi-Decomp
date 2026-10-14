// FUNC 8011f218 712 X014
// MATCHING 8011f218 712
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

typedef struct { short v[6]; } V6;
extern unsigned char TALK_POSE;
extern unsigned char D_8009CF0B[];
extern short D_80125E20[];
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

void func_8011F218(TObj *o)
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
        if (D_8009CF0B[((unsigned short*)&GAME.selectedSection)[0]] == 2) {
            o->state = 4;
        } else {
            TALK_POSE = 2;
            D_8009CF0B[((unsigned short*)&GAME.selectedSection)[0]] = 2;
            o->state++;
        }
        break;
    case 1:
        if (--o->timer == -1) {
            o->state++;
            SetAnim(o, 0x13);
            TALK_POSE = 0;
        }
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->da0 = (int)showMessageBoxDirect(2, D_80125E20[((unsigned short*)&GAME.selectedSection)[0]], &v);
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

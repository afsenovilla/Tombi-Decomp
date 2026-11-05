// VERSION SLPS_01144 x04 8011BF1C 560
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern struct { unsigned short pt; } D_8009E52A; /* selected spawn point, kept in a struct so the load is not hoisted */
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
void func_8011BF1C_slps01144(TObj *o)
{
    unsigned char t = o->b04;
    TObj *p = &PLAYER.obj;
    int st;
    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->box0 = 24;
        o->box1 = 48;
        o->box2 = 8;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->box3 = 16;
        o->b69 = 0;
        o->velH = 0;
        if (D_8009E52A.pt != 11) o->b04 = 3;
        break;
    case 1:
        ObjCullRegister(o);
        st = o->step;
        switch (st) {
        case 0:
            if (o->b69 && GVARS.playerLanded == t) {
                o->step++;
                o->velH = 0x200;
                GVARS.b3f = 1;
            }
            {
                short oh = o->h->p.whole;
                if (p->h->p.whole < oh - 8) p->h->p.whole = oh - 8;
            }
            {
                short oy = o->y.p.whole;
                if (oy - 4 < p->y.p.whole) p->y.p.whole = oy - 4;
            }
            break;
        case 1:
            o->velH -= 8;
            if (o->velH < 128) o->velH = 128;
            o->h->raw += o->velH << 8;
            if (o->h->p.whole >= 177) {
                o->step++;
                p->b04 = 1;
                GVARS.b3f = 0;
                p->step = 18;
                p->state = 0;
            }
            break;
        case 2:
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif

// FUNC 80117c84 536 X017
// MATCHING 80117c84 536
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern void func_80117224(TObj *);
extern void func_8011750C(TObj *);
extern void func_80117864(TObj *);
extern void func_801179EC(TObj *);
extern void setEventComplete(int, int);
extern unsigned char D_8009CE0A;
extern unsigned char D_8009CFD6;
extern unsigned char D_8009D2B0;

void func_80117C84(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.b40 != 0) {
            if (GVARS.b41 == 0x3c && D_8009CE0A == 1) {
                o->step = 3;
                o->state = 0;
            }
            GVARS.b40 = 0;
        } else {
            if (o->b68 != 0 && (unsigned short)o->wba == 1) {
                o->step = 5;
                o->state = 0;
            }
            if (D_8009CE0A != 0xff) {
                if (D_8009CFD6 == 1) o->step = 4;
                else o->step = 1;
                o->state = 0;
            }
        }
        break;
    case 1:
        func_80117224(o);
        break;
    case 2:
        func_8011750C(o);
        break;
    case 3:
        func_80117864(o);
        break;
    case 4:
        switch (o->state) {
        case 0:
            if (GVARS.b3a != 0) {
                GVARS.b42 = 1;
                GVARS.b3f = 1;
                D_8009D2B0 = 2;
                PLAYER.obj.b04 = 5;
                PLAYER.obj.step = 0;
                PLAYER.obj.state = 0;
                setEventComplete(0x66, 0);
                o->timer = 300;
                o->state++;
            }
            break;
        case 1:
            if (--o->timer <= 0) {
                D_8009CFD6 = 1;
                PLAYER.obj.b04 = 1;
                PLAYER.obj.step = 0x20;
                PLAYER.obj.state = 0;
                o->state++;
            }
            break;
        }
        break;
    case 5:
        func_801179EC(o);
        break;
    }
}

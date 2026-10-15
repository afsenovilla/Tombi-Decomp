// FUNC 8012e728 416 X010
// MATCHING 8012e728 416
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void func_8012E550(TObj *);
extern void setEventComplete(int, int);
extern void PoolFree_1F800210(TObj *);

void updateJunglePigBagComplete(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012E550(o);
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            break;
        case 1:
            switch (o->state) {
            case 0:
            case 1:
                if (--o->w08 <= 0) o->state = 15;
                break;
            case 2:
                PLAYER.obj.b04 = 5;
                PLAYER.obj.step = 100;
                PLAYER.obj.state = 0;
                GVARS.b3f = 1;
                GVARS.b42 = 1;
                GVARS.b3e = 1;
                setEventComplete(0xb7, 0);
                o->w08 = 200;
                o->state = 1;
                break;
            case 15:
                PLAYER.obj.b04 = 1;
                PLAYER.obj.step = 0;
                PLAYER.obj.state = 0;
                GVARS.b3f = 0;
                GVARS.b42 = 0;
                GVARS.b3e = 0;
                o->step = 0;
                o->state = 0;
                break;
            }
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}

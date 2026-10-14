// FUNC 8012e328 352 X010
// MATCHING 8012e328 352
#include "player.h"
#include "TOBJ.H"

extern unsigned char D_8009CE5B;
extern unsigned char D_8009C93E[], D_8009C93F[], D_8009C942[];
extern void setEventStarted(int, int, int);

void func_8012E328(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CE5B == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        switch (o->state) {
        case 0:
            ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
            ((unsigned char*)&PLAYER.obj.step)[0] = 100;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_8009C93E[0] = 1;
            setEventStarted(0xb7, 0, 0);
            o->w08 = 200;
            o->state++;
            break;
        case 1:
            if (--o->w08 > 0)
                break;
            o->state = 0xf;
            break;
        case 2:
            break;
        case 0xf:
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_8009C93E[0] = 0;
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    }
}

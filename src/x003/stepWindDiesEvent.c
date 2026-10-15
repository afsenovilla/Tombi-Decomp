// FUNC 80133938 232 X003
// MATCHING 80133938 232
#include "gcd.h"
#include "gv2.h"
#include "player.h"
#include "TOBJ.H"
extern void windDiesEventScene(TObj *);

void stepWindDiesEvent(TObj *o)
{
    unsigned char c;

    switch (o->step) {
    case 0:
        c = (*(unsigned char*)((char*)&PLAYER.obj.da0 + 2));
        if (c == 1 && (unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - 0xc8e) < 0x28 && PLAYER.obj.y.p.whole == -0x7bb) {
            switch (GCD.bce4e) {
            case 0:
                o->step = 1;
                o->state = 0;
                break;
            case 1:
                if (GV2.bc3 & 2) {
                    o->step = 1;
                    o->state = 2;
                }
                break;
            }
        }
        break;
    case 1:
        windDiesEventScene(o);
        break;
    }
}

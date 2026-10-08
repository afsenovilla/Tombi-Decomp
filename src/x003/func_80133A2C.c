// FUNC 80133a2c 360 X003
// MATCHING 80133a2c 360
#include "TOBJ.H"
#include "raw7.h"

extern unsigned char D_8009C93A;
extern TObj D_800A6038;
extern unsigned char D_8009CE4E;
extern unsigned char D_8009D2C3;
extern void func_801337B4(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80133A2C(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        if (D_8009C93A == 0) break;
        switch (o->step) {
        case 0:
            if (U8(&D_800A6038, 0xa2) != 1) break;
            if ((unsigned short)(D_800A6038.h->p.whole - 0xc8e) >= 0x28) break;
            if (D_800A6038.y.p.whole != -0x7bb) break;
            switch (D_8009CE4E) {
            case 0:
                o->step = 1;
                o->state = 0;
                break;
            case 1:
                if (D_8009D2C3 & 2) {
                    o->step = 1;
                    o->state = 2;
                }
                break;
            }
            break;
        case 1:
            func_801337B4(o);
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

// FUNC 800ec84c 372 X011
// MATCHING 800ec84c 372
#include "TOBJ.H"
#include "raw7.h"
extern unsigned short D_1f8001c8;
extern TObj *FUN_80018448(void);

void FUN_800ec84c(int type)
{
    TObj *o;
    int i;
    unsigned char r, g, b;

    switch (type) {
    case 0:
        r = 0xff;
        g = 0x30;
        b = 0x30;
        break;
    case 1:
        r = 0x30;
        g = 0x30;
        b = 0xff;
        break;
    case 2:
        r = 0x30;
        g = 0xff;
        b = 0x30;
        break;
    }
    for (i = 0; i < 8; i++) {
        o = FUN_80018448();
        if (o) {
            o->type = 0x5f;
            o->b0a = 0x13;
            o->timer = 0x5a;
            o->active = 1;
            o->subtype = type;
            o->b0c = i;
            o->b6b = 0;
            o->ba5 = 0xc;
            o->animFrame = i << 5;
            o->d84 = 0xb0;
            switch (D_1f8001c8 & 1) {
            case 0:
                o->d88 = ((o->animFrame - 0x40) & 0xff) << 4;
                break;
            case 1:
                o->d88 = ((o->animFrame - 0x80) & 0xff) << 4;
                break;
            }
            o->d30 = r;
            o->d34 = g;
            o->d8c = 0;
            o->d38 = b;
        }
    }
}

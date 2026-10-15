// FUNC 8011d17c 440 X009
// MATCHING 8011d17c 440
#include "spad.h"
#include "TOBJ.H"
extern void *D_8012E03C[];
extern void FUN_80018da4(TObj *);
extern void freeObjectLayer7(TObj *);

void updateParallaxBackdropSprite(TObj *o)
{
    unsigned short v;
    unsigned short t;

    switch (o->b04) {
    case 0:
        o->w1e = 9;
        o->y.p.whole += 0x180;
        o->b0d = 0;
        o->anim = D_8012E03C[o->b0c];
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->b0f = 0;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->b.p.whole = 0;
        o->d38 = 0x980;
        o->b04++;
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            o->visible = 1;
            FUN_80018da4(o);
            break;
        case 1:
            t = o->d30;
            t -= ((unsigned short*)((char*)&SPAD.i174 + 2))[0];
            if ((short)t < -0x100)
                t = (t & 0xfff) >> 3;
            else
                t = (short)t >> 3;
            if ((short)t < 0x160) {
                o->a.p.whole = t;
                t = o->d34 - ((unsigned short*)&SPAD.s186)[0];
                if ((short)t < o->d38)
                    t = o->d38;
                t = (t & 0xfff) >> 4;
                o->y.p.whole = t;
                o->visible = 1;
                FUN_80018da4(o);
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}

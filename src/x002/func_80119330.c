// FUNC 80119330 380 X002
// MATCHING 80119330 380
#include "TOBJ.H"

extern unsigned char D_8009CE41;
extern int DAT_1f8002d0[];
extern void *D_8011FB7C[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_80119178(TObj *);
extern void FUN_80018790(TObj *);

void func_80119330(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009CE41 != 0) {
            o->b04 = 2;
            break;
        }
        o->w1e = 7;
        o->b0d = 0;
        o->d3c = DAT_1f8002d0[0];
        o->anim = D_8011FB7C[o->subtype];
        FUN_8001fe6c(o);
        if (o->subtype >= 4)
            o->animFrame = 1;
        switch (o->subtype) {
        case 0:
            o->timer = 120;
            break;
        case 1:
            o->timer = 180;
            break;
        case 2:
            o->timer = 60;
            break;
        case 3:
            o->timer = 240;
            break;
        case 4:
            o->timer = 30;
            break;
        case 5:
            o->timer = 0;
            break;
        case 6:
            o->timer = 150;
            break;
        }
        o->b04++;
        break;
    case 1:
        FUN_800202b4(o);
        func_80119178(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

// FUNC 8011ad54 460 X010
// MATCHING 8011ad54 460
#include "TOBJ.H"

extern unsigned char D_8009D078;
extern short D_1F80016A, D_1F80016E;
extern int FUN_800202b4(TObj *);
extern void func_8011AB70(TObj *);
extern void FUN_80018838(TObj *);

void func_8011AD54(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D078 != 0) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->b69 = 0;
        o->b6b = 0;
        o->w98 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d34 = o->y.p.whole;
        o->d8c = o->animFrame << 5;
        if (o->subtype == 0) {
            o->active = 2;
        } else if (o->subtype == 1) {
            o->box0 = 0xe;
            o->box1 = 0x16;
            o->box2 = 4;
            o->box3 = 8;
        } else {
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 4;
            o->box3 = 8;
        }
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b69 == 1) {
            o->w98 = 1;
            o->b6b = 1;
        }
        o->b69 = 0;
        if (o->w98 != 0)
            func_8011AB70(o);
        if (o->subtype == 1 && o->w98 == 0 && D_1F80016A >= 0xc81 && D_1F80016E >= -0x27) {
            o->w98 = 1;
            o->b6b = 1;
            func_8011AB70(o);
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}

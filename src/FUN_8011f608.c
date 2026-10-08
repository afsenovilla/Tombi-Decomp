// FUNC 8011f608 404 X000
// MATCHING 8011f608 404
#include "TOBJ.H"
extern void FUN_80020078(TObj *, int);
extern void FUN_80018838(TObj *);
extern unsigned char DAT_800a603d;
extern unsigned char DAT_800a6047[];
extern short *DAT_800a607c[];

void FUN_8011f608(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (o->subtype == 0) {
            o->box0 = 0x58;
            o->box1 = 0x60;
            o->box2 = 4;
            o->box3 = 8;
            o->b0a = 0x11;
            o->b0f = DAT_800a6047[0] + 1;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->d->p.whole = DAT_800a607c[0][1];
            o->b04++;
        }
        break;
    case 1:
        FUN_80020078(o, 0x5a);
        switch (o->step) {
        case 0:
            if (DAT_800a603d == 5) {
                o->timer = 0x80;
                o->step++;
            }
            break;
        case 1:
            o->d8c += 0x20;
            o->y.p.whole += 2;
            if (--o->timer == 0) {
                o->b04 = 2;
                o->step = 0;
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}

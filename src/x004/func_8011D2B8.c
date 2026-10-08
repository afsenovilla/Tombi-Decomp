// FUNC 8011d2b8 652 X004
// MATCHING 8011d2b8 652
#include "TOBJ.H"
extern TObj D_800A6038;
extern unsigned short D_8009C982[];
extern unsigned char D_8009C93F, D_8009C93A;
extern void FUN_800202b4(TObj *);
extern void FUN_80018838(TObj *);

void func_8011D2B8(TObj *o)
{
    TObj *pl = &D_800A6038;
    unsigned char b = o->b04;
    unsigned char s;

    switch (b) {
    case 0:
        o->b04 = b + 1;
        o->box0 = 0x18;
        o->box1 = 0x30;
        o->box2 = 8;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->box3 = 0x10;
        o->b69 = 0;
        o->velH = 0;
        if (D_8009C982[0] != 0xb)
            o->b04 = 3;
        else
            D_8009C93F = 1;
        break;
    case 1:
        FUN_800202b4(o);
        s = o->step;
        switch (s) {
        case 0:
            if (o->b69 && D_8009C93A == 1) {
                o->step = s + 1;
                o->velH = 0x200;
            }
            if (pl->h->p.whole < o->h->p.whole - 8)
                pl->h->p.whole = o->h->p.whole - 8;
            if (o->y.p.whole - 4 < pl->y.p.whole)
                pl->y.p.whole = o->y.p.whole - 4;
            break;
        case 1:
            o->velH -= 8;
            if (o->velH < 0x80)
                o->velH = 0x80;
            o->h->raw += o->velH << 8;
            if (o->h->p.whole > 0xb0) {
                o->timer = 0x5a;
                o->step++;
                D_800A6038.b04 = 1;
                D_800A6038.step = 0x12;
                D_800A6038.state = 0;
            }
            break;
        case 2:
            if (--o->timer == -1) {
                o->step++;
                D_8009C93F = 0;
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}

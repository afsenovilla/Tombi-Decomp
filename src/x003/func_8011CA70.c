// FUNC 8011ca70 540 X003
// MATCHING 8011ca70 540
#include "TOBJ.H"

extern void *D_80139878[];
extern unsigned short D_800A604E;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void func_8011B6F4(TObj *);
extern void func_8011B870(TObj *);
extern void func_8011BA34(TObj *);
extern void func_8011BCC8(TObj *);
extern void func_8011C050(TObj *);
extern void func_8011C3D8(TObj *);

void func_8011CA70(TObj *o)
{
    switch (o->step) {
    case 0:
        func_8011B6F4(o);
        break;
    case 1:
        func_8011B870(o);
        break;
    case 2:
        func_8011BA34(o);
        break;
    case 3:
        func_8011BCC8(o);
        break;
    case 4:
        func_8011C050(o);
        break;
    case 5:
        switch (o->state) {
        case 0:
            o->timer = 15;
            o->d38 = 0x40;
            o->wac = 5;
            o->w74 = 0;
            o->b0a = 0;
            o->y.p.whole += 0x30;
            o->state++;
            o->anim = D_80139878[0];
            AnimLoadDuration(o);
            break;
        case 1:
            if (o->timer != 0) {
                o->timer--;
            } else if (AnimAdvance(o)) {
                o->state++;
                D_800A604E++;
            }
            break;
        case 2:
            o->state = 0;
            o->step++;
            break;
        }
        break;
    case 6:
        func_8011C3D8(o);
        break;
    }
    if (o->w74 & 2) {
        if (o->w74 & 1)
            o->d8c = (o->d38 + 0x80) & 0xff;
        else
            o->d8c = o->d38;
    } else if ((unsigned)((o->d38 - 0x40) & 0xff) < 0x80) {
        o->animFrame = 1;
        o->d8c = (o->d38 + 0x80) & 0xff;
    } else {
        o->animFrame = 0;
        o->d8c = o->d38;
    }
}

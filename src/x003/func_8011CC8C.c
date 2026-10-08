// FUNC 8011cc8c 408 X003
// MATCHING 8011cc8c 408
#include "TOBJ.H"
extern TObj D_800A6038;
extern void *D_80139870[];
extern void func_8011B6F4(TObj *), func_8011B870(TObj *), func_8011BA34(TObj *), func_8011BCC8(TObj *);
extern void func_8011C050(TObj *), func_8011C7FC(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);

void func_8011CC8C(TObj *o)
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
    case 6:
        break;
    case 7:
        switch (o->state) {
        case 0:
            break;
        case 1:
            if (AnimAdvance(o)) {
                { TObj *pl = &D_800A6038; pl->y.p.whole--; }
                o->step = 3;
                o->state = 0;
                o->wac = 3;
                o->y.p.whole -= 0x30;
                o->anim = D_80139870[0];
                AnimLoadDuration(o);
            }
            break;
        }
        break;
    case 8:
        func_8011C7FC(o);
        break;
    }
    if (o->w74 & 2) {
        if (o->w74 & 1) o->d8c = (o->d38 + 0x80) & 0xff;
        else o->d8c = o->d38;
    } else {
        if ((unsigned char)(o->d38 - 0x40) < 0x80) {
            o->animFrame = 1;
            o->d8c = (o->d38 + 0x80) & 0xff;
        } else {
            o->animFrame = 0;
            o->d8c = o->d38;
        }
    }
}

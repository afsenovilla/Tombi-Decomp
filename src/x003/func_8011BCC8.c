// FUNC 8011bcc8 904 X003
// MATCHING 8011bcc8 904
#include "TOBJ.H"

extern void *D_80139870[];
extern void AnimLoadDuration(TObj *);

void func_8011BCC8(TObj *o)
{
    unsigned char c;

    switch (o->state) {
    case 0:
        c = o->d38 - 0x40;
        o->velH = 0x600;
        o->velV = 7;
        o->b0a = 2;
        o->w74 = o->animFrame + 2;
        o->d34 = (o->d38 << 8) & 0xff00;
        if (c != 0) {
            if (c > 0x80) o->state = 1;
            else o->state = 2;
        } else o->state = 2;
        o->wac = 3;
        o->anim = D_80139870[0];
        AnimLoadDuration(o);
        return;
    case 1:
        o->d34 = (o->d34 + o->velH) & 0xffff;
        if (o->d34 > 0xbfff) break;
        if (o->d34 < 0x4001) break;
        o->velH -= 0x80;
        if (o->velH < 0x300) {
            o->velH = 0x300;
            o->state = 3;
        }
        break;
    case 2:
        o->d34 = (o->d34 - o->velH) & 0xffff;
        if (o->d34 < 0x4000) {
            o->velH -= 0x80;
            if (o->velH < 0) {
                o->state = 6;
                o->velH = 0;
            }
        }
        break;
    case 3:
        o->d34 = (o->d34 + o->velH) & 0xffff;
        if (o->d34 > 0x4000) {
            o->velH -= 0x80;
            if (o->velH < 0) o->velH = 0;
        }
        if (o->d34 > 0x5000) {
            o->state++;
            o->velV--;
            o->d34 = 0x5000;
            if (o->subtype & 4) {
                if (o->subtype & 1) {
                    if (o->velV <= 0) {
                        o->state = 0;
                        o->step++;
                    }
                }
            }
        }
        break;
    case 4:
        o->d34 = (o->d34 - o->velH) & 0xffff;
        if (o->d34 < 0x4001) {
            o->state++;
            if (!(o->subtype & 4)) {
                if (o->velV <= 0) {
                    o->step = 5;
                    o->state = 0;
                }
            }
        } else {
            o->velH += 0x80;
            if (o->velH > 0x600) o->velH = 0x600;
        }
        break;
    case 5:
        o->d34 = (o->d34 - o->velH) & 0xffff;
        if (o->d34 < 0x4000) {
            o->velH -= 0x80;
            if (o->velH < 0) o->velH = 0;
        }
        if (o->d34 < 0x3000) {
            o->state++;
            o->velV--;
            o->d34 = 0x3000;
            if (o->subtype & 4) {
                if (!(o->subtype & 1)) {
                    if (o->velV <= 0) {
                        o->state = 0;
                        o->step++;
                    }
                }
            }
        }
        break;
    case 6:
        o->d34 = (o->d34 + o->velH) & 0xffff;
        if (o->d34 < 0x4000) {
            o->velH += 0x80;
            if (o->velH > 0x600) o->velH = 0x600;
        } else {
            o->state = 3;
            if (!(o->subtype & 4)) {
                if (o->velV <= 0) {
                    o->step = 5;
                    o->state = 0;
                }
            }
        }
        break;
    default:
        return;
    }
    o->d38 = ((unsigned char *)&o->d34)[1];
}

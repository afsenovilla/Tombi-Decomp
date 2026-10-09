// FUNC 8011992c 800 X003
// MATCHING 8011992c 800
#include "TOBJ.H"

extern unsigned char D_8009D2C3;
extern int D_1F8002D4;
extern void *D_80138E84[];
void AnimLoadDuration(TObj *o);
void AnimAdvance(TObj *o);
int ObjCullRegister(TObj *o);
void FUN_80018ca4(TObj *o);
void FUN_800187e4(TObj *o);

void func_8011992C(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (!(D_8009D2C3 & 4)) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        if (o->b0c == 0) {
            o->b0d = 0x80;
            o->w1e = 8;
            o->d64 = 0x2000;
        } else {
            o->b0d = 0x81;
            o->w08 = 0x7811;
            o->w1e = 9;
            o->d64 = 0x1800;
        }
        o->d3c = D_1F8002D4;
        o->anim = D_80138E84[o->b0c];
        AnimLoadDuration(o);
        break;
    case 1:
        switch (o->b0c) {
        case 0:
            o->visible = 1;
            FUN_80018ca4(o);
            switch (o->step) {
            case 0:
                o->step++;
                o->timer = 0x30;
                break;
            case 1:
                if (--o->timer == -1) {
                    o->timer = 0x30;
                    o->step++;
                } else {
                    o->d64 -= 0x10;
                }
                break;
            case 2:
                if (--o->timer == -1) {
                    o->timer = 0x30;
                    o->step--;
                } else {
                    o->d64 += 0x10;
                }
                break;
            }
            switch (o->state) {
            case 0:
                o->state++;
                o->w22 = 0x3c;
                break;
            case 1:
                if (--o->w22 == -1) {
                    o->w22 = 0x3c;
                    o->state++;
                } else {
                    o->y.raw -= 0x2000;
                }
                break;
            case 2:
                if (--o->w22 == -1) {
                    o->w22 = 0x3c;
                    o->state--;
                } else {
                    o->y.raw += 0x2000;
                }
                break;
            }
            break;
        case 2:
            if (ObjCullRegister(o)) {
                AnimAdvance(o);
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}

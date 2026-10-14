// FUNC 8011dd94 832 X001
// MATCHING 8011dd94 832
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { Fix16 x, y, z; } V3;
extern char D_8013C564[];
extern void cullObjectToDrawList(TObj *, int);
extern void func_8011DB5C(TObj *);
extern void applyItemEffect(char *, V3 *, int, int, int);
extern void FUN_80018838(TObj *);

void func_8011DD94(TObj *o)
{
    unsigned char b = o->b04;
    V3 v;
    unsigned char *p;

    switch (b) {
    case 0:
        switch (o->subtype) {
        case 0:
            {
            int f = *(signed char *)&o->animFrame;
            o->box0 = 0x76;
            o->box1 = 0xee;
            o->box2 = 4;
            o->box3 = 8;
            o->b0a = 0x11;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->w7a = 0;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->animFrame = 2;
            o->b69 = 0;
            o->d38 = f << 12;
            o->b04++;
            }
            break;
        case 1:
            {
            int f = o->animFrame;
            o->box0 = 0x76;
            o->box1 = 0xec;
            o->box2 = 4;
            o->box3 = 8;
            o->b0a = 0x11;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->animFrame = 2;
            o->b69 = 0;
            o->d38 = f << 12;
            o->b04++;
            }
            break;
        }
        if (o->b0c != 0 && ((unsigned char*)&GCD.bceae)[0] == 4) {
            v.x.p.whole = o->h->p.whole;
            v.y.p.whole = o->y.p.whole - 0x18;
            v.z.p.whole = o->d->p.whole;
            applyItemEffect(D_8013C564, &v, 1, 0, 0);
        }
        break;
    case 1:
        cullObjectToDrawList(o, 0x78);
        if (o->b0c == 0) {
            if (GVARS.b42 == 0)
                func_8011DB5C(o);
            break;
        }
        switch (o->step) {
        case 0:
            func_8011DB5C(o);
            if (((unsigned char*)&GCD.bceae)[0] == 1) {
                o->d8c = 0;
                o->velH = 0;
                o->d38 = 0;
                o->b69 = 0;
                o->step++;
            }
            break;
        case 1:
            p = ((unsigned char*)&GCD.bceae);
            if (*p < 2)
                break;
            o->d38 += 0x40;
            if (o->d38 > 0x38e)
                o->d38 = 0x38e;
            o->d8c = o->d38 & 0xfff;
            if (o->d8c >= 0x200) {
                *p = 3;
                o->step++;
            }
            break;
        case 2:
            o->d38 += 0x20;
            if (o->d38 > 0x38e)
                o->d38 = 0x38e;
            o->d8c = o->d38 & 0xfff;
            if (((unsigned char*)&GCD.bceae)[0] == 4) {
                o->d8c = 0;
                o->velH = 0;
                o->d38 = 0;
                o->step = 0;
                o->b69 = 0;
                o->b0c = 0;
            }
            break;
        }
        break;
    case 2:
        o->b04 = b + 1;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}

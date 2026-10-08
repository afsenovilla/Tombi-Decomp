// FUNC 8011da50 484 X010
// MATCHING 8011da50 484
#include "TOBJ.H"

extern unsigned char D_8009C940[];
extern unsigned char D_8009C941;
extern void *D_80131CE0;
extern int D_1F8002D4[];
extern unsigned char D_8009CD9A, D_8009CD9E;
extern unsigned char D_800B146E, D_800B1476, D_800B1472;
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int rcos(int);
extern void FUN_8005a9a4(int, int);
extern void func_8004D620(int, int);
extern void FUN_800188e0(TObj *);

void func_8011DA50(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009C940[0] != 0) {
            if (D_8009C941 != 0x7e) break;
            D_8009C940[0] = 0;
            o->box0 = 8;
            o->box1 = 0x10;
            o->box3 = 0x10;
            o->box2 = 8;
            o->active = 1;
            o->w1e = 0xc;
            o->b69 = 0;
            o->b0a = 0;
            o->w08 = 0x7acc;
            o->b0d = 1;
            o->anim = D_80131CE0;
            o->d3c = D_1F8002D4[0];
            o->d38 = o->b.p.whole;
            o->w22 = 0;
            AnimLoadDuration(o);
            o->b04++;
        } else {
            o->active = 2;
        }
        break;
    case 1:
        if (ObjCullRegister(o)) AnimAdvance(o);
        o->b.p.whole = o->d38 + (rcos(o->w22 & 0xfff) >> 7);
        if ((o->w22 & 0xfff) > 0x800) o->animFrame = 0;
        else o->animFrame = 1;
        o->w22 += 0x10;
        break;
    case 2:
        FUN_8005a9a4(0x77, 0);
        func_8004D620(0x32, 2);
        D_8009CD9A = 9;
        D_8009CD9E = 0x3c;
        D_800B146E = 0x3c;
        D_800B1476 = 1;
        D_800B1472 = 0x1e;
        o->b04++;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}

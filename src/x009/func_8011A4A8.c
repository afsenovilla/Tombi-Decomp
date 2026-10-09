// FUNC 8011a4a8 504 X009
// MATCHING 8011a4a8 504
#include "TOBJ.H"
extern TObj D_800A6038;
extern unsigned char D_8009CF06[], D_8009D006[], D_8009C990, D_8009D2B3;
extern unsigned long *D_8012B1A0, *D_8012B1B4;
extern unsigned long *D_8012B190[], *D_8012B1A4[];
extern int D_1F8002E0[];
extern void *D_8013105C;
extern void loadImageRect(unsigned long *, short, short, short, short);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void func_80119F5C(TObj *);
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

void func_8011A4A8(TObj *o)
{
    switch (o->b04) {
    case 0:
        D_800A6038.active = 5;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b0a = 0;
        o->b0f = 0;
        o->animFrame = 0;
        *(int *)&o->w5c = o->a.p.whole;
        o->d60 = o->y.p.whole;
        o->d64 = o->b.p.whole;
        if (D_8009CF06[0]) {
            loadImageRect(D_8012B1A0, 0x90, 0x1f0, 0x10, 1);
            loadImageRect(D_8012B1B4, 0x90, 0x1f1, 0x10, 1);
        } else if (D_8009D006[0]) {
            loadImageRect(D_8012B1A0, 0x90, 0x1f0, 0x10, 1);
            loadImageRect(D_8012B1B4, 0x90, 0x1f1, 0x10, 1);
        } else {
            loadImageRect(D_8012B190[D_8009C990], 0x90, 0x1f0, 0x10, 1);
            loadImageRect(D_8012B1A4[D_8009D2B3], 0x90, 0x1f1, 0x10, 1);
        }
        o->w1e = 1;
        o->b0d = 0x80;
        o->b04++;
        o->d3c = D_1F8002E0[0];
        o->anim = D_8013105C;
        AnimLoadDuration(o);
        break;
    case 1:
        func_80119F5C(o);
        AnimAdvance(o);
        o->visible = 1;
        FUN_80018ca4(o);
        break;
    case 2:
    case 3:
        FUN_800187e4(o);
        break;
    }
}

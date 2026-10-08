// FUNC 8011e064 116 X009
// MATCHING 8011e064 116
#include "TOBJ.H"
extern char D_80010C50[];
extern unsigned char D_801152E8[];
extern void SfxPlay3(int, int);
extern void AnimJump(TObj *, int);

void func_8011E064(TObj *o)
{
    SfxPlay3(0x1c, 0x7f);
    o->b9c = 0;
    o->anim = D_80010C50;
    AnimJump(o, 4);
    *(unsigned char *)&o->waa = 0;
    o->d88 = 0;
    o->d8c = D_801152E8[o->wb0];
    o->state++;
}

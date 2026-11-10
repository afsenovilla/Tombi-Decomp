// VERSION BETA_980203 x00 800F12CC 248
#if defined(VERSION_BETA_980203)
// Beta version: the X00 initWeaponTrailSprite (PAL x013 initWeaponTrailSprite.c; the boot-exe copy is 80031A24): w1e 0xa3 / 0x33 by graph type, b6b 0x40.
#include "TOBJ.H"
extern int SPR_DATA[];
extern char D_8011846C[], D_80118470[];  /* x00 weapon-trail defs, 12 bytes each: +0 sprite index, +4 anim table */
extern int func_8005D190_beta980203(void);  /* GetGraphType */
extern void AnimLoadDuration(TObj *);

void initWeaponTrailSprite(TObj *o)
{
    short v;
    if (func_8005D190_beta980203() == 1 || func_8005D190_beta980203() == 2) v = 0xa3;
    else v = 0x33;
    o->w1e = v;
    o->d3c = SPR_DATA[*(short *)(D_8011846C + o->subtype * 12)];
    o->anim = *(void **)(*(int *)(D_80118470 + o->subtype * 12) + o->b0c * 4);
    AnimLoadDuration(o);
    o->b0a = 13;
    o->b0d = 0x80;
    o->b6b = 0x40;
    *(signed char *)&o->b0f = -7;
    o->d8c = 0;
    o->category |= 0x80;
    o->b04++;
}
#endif

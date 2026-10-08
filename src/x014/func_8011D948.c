// FUNC 8011d948 136 X014
// MATCHING 8011d948 136
#include "TOBJ.H"
extern int D_80125CF8[];
extern void AnimLoadDuration(TObj *);

static __inline__ void setbox(TObj *o, short k)
{
    unsigned char *p = (unsigned char *)o->d90 + k * 4;
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    o->box3 = *p;
    o->wac = k;
    o->anim = ((void **)*(int *)&o->wa8)[k];
    AnimLoadDuration(o);
}

void func_8011D948(TObj *o)
{
    setbox(o, D_80125CF8[(o->wb8 >> 1) & 7]);
}

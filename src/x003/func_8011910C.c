// FUNC 8011910c 184 X003
// MATCHING 8011910c 184
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_80138EC8[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void func_8011910C(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 9;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        o->anim = D_80138EC8[o->b0c];
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

// FUNC 8011910c 184 X003
// MATCHING 8011910c 184
#include "spad.h"
#include "TOBJ.H"

extern char D_801391E0[];
extern char D_801391E8[];
extern char D_80139158[];
extern char D_80139150[];
static void *D_80138EC8[4] = {
    D_801391E0, D_801391E8, D_80139158, D_80139150,
};
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void updateAnimDecorSprite(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 9;
        o->b0d = 0;
        o->d3c = ((int*)&SPAD.i2d4)[0];
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

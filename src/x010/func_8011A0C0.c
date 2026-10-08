// FUNC 8011a0c0 280 X010
// MATCHING 8011a0c0 280
#include "TOBJ.H"
typedef void (*Fn)(TObj *);

extern void *D_80131C8C[];
extern Fn D_8012F298[];
extern int D_1F8002D4[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjListPush_1F800224(TObj *);
extern void ObjFreeDup(TObj *);

void func_8011A0C0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b69 = 0;
        o->w1e = 0xc;
        o->b0d = 0;
        o->anim = D_80131C8C[o->b0c];
        o->d3c = D_1F8002D4[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->subtype == 3) {
            o->visible = 1;
            ObjListPush_1F800224(o);
        } else {
            ObjCullRegister(o);
        }
        D_8012F298[o->subtype](o);
        o->b69 = 0;
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

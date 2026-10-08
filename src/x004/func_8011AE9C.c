// FUNC 8011ae9c 268 X004
// MATCHING 8011ae9c 268
#include "TOBJ.H"

extern void *D_80134D60[];
extern int D_1F8002D4[];
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void func_8011AE9C(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 9;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        if (o->subtype == 0) {
            o->anim = D_80134D60[0];
        } else {
            o->anim = D_80134D60[o->subtype];
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->subtype != 0) {
            switch (o->step) {
            case 0:
                o->timer = 0x1a;
                break;
            case 1:
                break;
            case 2:
                break;
            }
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

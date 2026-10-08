// FUNC 80118ff0 152 X002
// MATCHING 80118ff0 152
#include "TOBJ.H"

extern void *D_8011FB60[];
extern unsigned char D_8009CFEA;
extern void AnimLoadDuration(TObj *);

void func_80118FF0(TObj *o)
{
    switch (o->state) {
    case 0:
        o->anim = D_8011FB60[o->subtype];
        AnimLoadDuration(o);
        o->state++;
        break;
    case 1:
        if (D_8009CFEA == 2) {
            o->step = 2;
            o->state = 0;
        }
        break;
    }
}

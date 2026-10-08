// FUNC 8011f1d0 184 X000
// MATCHING 8011f1d0 184
#include "TOBJ.H"
extern int D_1F800334;
extern unsigned char D_8009CDAC;
extern void ObjListPush_1F800224(void);
extern void ObjFreeDup(void);

void func_8011F1D0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->active = 2;
        if (D_8009CDAC == 0xff) {
            int *pp = &D_1F800334;
            pp = (int *)(*pp + ((int *)*(int *volatile *)pp)[12]);
            o->da0 = (int)pp;
        }
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800224();
        break;
    case 2:
    case 3:
        ObjFreeDup();
        break;
    }
}

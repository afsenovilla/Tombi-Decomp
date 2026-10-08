// FUNC 8011f1d0 184 X000
// wip: only the final addu differs (game addu v1,v1,v0 -> sw v1; ours addu v0,v0,v1)
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
            int *volatile *pg = (int *volatile *)&D_1F800334; o->da0 = (*pg)[12] + (unsigned int)*pg;
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

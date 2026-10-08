// FUNC 8011b7f0 208 X000
#include "TOBJ.H"
extern unsigned char D_8009CDAC;
extern int D_1F8002D4;
extern void *D_8013B168[];
extern void ObjCullRegister(o);
extern void ObjFree(o);

void func_8011B7F0(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        if (D_8009CDAC != 0xff) {
            o->b04 = 3;
        } else {
            o->b04 = s + 1;
            o->w1e = 8;
            o->b0d = 0;
            o->d3c = D_1F8002D4;
            o->anim = D_8013B168[o->b0c];
        }
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

// FUNC 8011afc8 240 X003
// MATCHING 8011afc8 240
#include "TOBJ.H"
typedef struct { unsigned char a, b, c; } T3;
extern T3 D_80135A6C[];
extern int ObjCullRegister(TObj *);
extern void func_8011ADD4(TObj *);
extern void ObjFreeDup(TObj *);

void func_8011AFC8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = D_80135A6C[o->subtype].a;
        o->box1 = D_80135A6C[o->subtype].b;
        o->box3 = o->box2 = D_80135A6C[o->subtype].c;
        break;
    case 1:
        ObjCullRegister(o);
        func_8011ADD4(o);
        o->b69 = 0;
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

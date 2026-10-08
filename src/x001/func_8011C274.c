// FUNC 8011c274 224 X001
// MATCHING 8011c274 224
#include "TOBJ.H"
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

void func_8011C274(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        if (o->subtype == 0) {
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 0x44;
            o->box3 = 0x60;
        } else {
            o->box0 = 0x18;
            o->box1 = 0x30;
            o->box2 = 0;
            o->box3 = 0x50;
        }
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

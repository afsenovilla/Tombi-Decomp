// FUNC 8011c158 284 X001
// MATCHING 8011c158 284
#include "TOBJ.H"

extern unsigned short D_8013C484[], D_8013C486[], D_8013C488[], D_8013C48A[];
extern unsigned char D_8009CE48[];
extern unsigned char D_8009D081;
extern void FUN_80020078(TObj *, int);
extern void ObjFreeDup(TObj *);

void func_8011C158(TObj *o)
{

    switch (o->b04) {
    case 0:
        o->box0 = -D_8013C484[0];
        o->box1 = o->box0 + D_8013C486[0];
        o->box2 = -D_8013C488[0];
        o->box3 = o->box2 + D_8013C48A[0];
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        if (D_8009CE48[0] != 0) {
            o->b04 = 3;
        } else {
            o->b04 = 1;
        }
        break;
    case 1:
        if (D_8009D081 >= 4) {
            o->b04 = 3;
        } else {
            FUN_80020078(o, 0x92);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

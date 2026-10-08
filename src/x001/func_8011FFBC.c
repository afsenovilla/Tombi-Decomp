// FUNC 8011ffbc 304 X001
// MATCHING 8011ffbc 304
#include "TOBJ.H"
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CE48;
extern unsigned char D_8009D081;
extern void FUN_800201ac(TObj *, int);
extern void ObjFreeDup(TObj *);

void func_8011FFBC(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (!(D_8009D2C3 & 1)) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->active = 2;
        if (D_8009CE48) {
            o->step = 1;
        } else {
            o->step = 0;
        }
        break;
    case 1:
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0: {
                unsigned char *c = &D_8009D081;
                if (*c >= 4) {
                    *c = 5;
                    o->state++;
                }
                break;
            }
            case 1:
                FUN_800201ac(o, 0x90);
                break;
            }
            break;
        case 1:
            FUN_800201ac(o, 0x90);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

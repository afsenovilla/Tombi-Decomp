// FUNC 8011ab44 392 X009
// MATCHING 8011ab44 392
#include "TOBJ.H"

extern void ObjCullRegister(TObj *o);
extern void ObjFreeDup(TObj *o);
extern void func_8011AA04(TObj *o);

void func_8011AB44(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            func_8011AA04(o);
            break;
        case 1:
            switch (o->state) {
            case 0:
                o->state = ((TObj *)o->d90)->state;
                if (o->state == 3) o->d88 = 0x600;
                break;
            case 1:
                o->d88 = (o->d88 + 0x20) & 0xfff;
                if (o->d88 >= 0x600) o->state++;
                break;
            case 2:
                break;
            case 3:
                o->d88 = (o->d88 - 0x20) & 0xfff;
                if (o->d88 == 0) o->state = 0;
                break;
            }
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

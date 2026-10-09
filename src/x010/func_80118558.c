// FUNC 80118558 392 X010
// MATCHING 80118558 392
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern void func_80118418(TObj *);

void func_80118558(TObj *o)
{
    unsigned char s = o->b04;

    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            func_80118418(o);
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

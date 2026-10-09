// FUNC 80118b40 404 X010
// MATCHING 80118b40 404
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

void func_80118B40(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0x18;
        o->d88 = 0;
        o->d8c = o->animFrame << 4;
        switch (o->subtype) {
        case 0: case 3: case 4: case 9: case 10: case 11:
            o->box0 = 0x14;
            o->box1 = 0x28;
            o->box2 = 0x18;
            o->box3 = 0x30;
            break;
        case 1: case 7:
            o->box0 = 0x24;
            o->box1 = 0x36;
            o->box2 = 0x26;
            o->box3 = 0x46;
            break;
        case 2:
            o->box0 = 0x10;
            o->box1 = 0x2c;
            o->box2 = 0x20;
            o->box3 = 0x3e;
            break;
        case 5:
            o->box0 = 0x10;
            o->box1 = 0x28;
            o->box2 = 0x20;
            o->box3 = 0x40;
            break;
        case 6: case 8:
            o->box0 = 0x16;
            o->box1 = 0x2c;
            o->box2 = 0x14;
            o->box3 = 0x28;
            break;
        case 12:
            o->box0 = 0x1c;
            o->box1 = 0x26;
            o->box2 = 0x20;
            o->box3 = 0x36;
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

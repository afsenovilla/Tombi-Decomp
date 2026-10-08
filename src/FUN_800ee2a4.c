// FUNC 800ee2a4 304 X000
// MATCHING 800ee2a4 304
#include "TOBJ.H"
extern void FUN_800201ac(TObj *, int);
extern void FUN_80018838(void);

void FUN_800ee2a4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        if (o->b0c == 0) {
            o->box0 = 4;
            o->box1 = 8;
            o->box2 = 0xaa;
            o->box3 = 0x154;
            o->subtype = 1;
        } else if (o->b0c == 1) {
            o->box0 = 6;
            o->box1 = 8;
            o->box2 = 0xaa;
            o->box3 = 0x154;
        } else if (o->b0c == 2) {
            o->box0 = 6;
            o->box1 = 8;
            o->box2 = 0x28;
            o->box3 = 0xb4;
        } else if (o->b0c == 3) {
            o->box0 = 6;
            o->box1 = 8;
            o->box2 = 0x64;
            o->box3 = 0xc8;
        }
        *(int *)((char *)o + 0xa0) = 0;
        break;
    case 1:
        FUN_800201ac(o, 0x40);
        break;
    case 2:
        break;
    case 3:
        FUN_80018838();
        break;
    }
}

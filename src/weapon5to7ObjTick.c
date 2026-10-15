// FUNC 80034f58 324 MAIN0
// MATCHING 80034f58 324
#include "plx.h"
#include "gv2.h"
#include "TOBJ.H"
extern signed char D_800A611B;
void weapon5to7Init(TObj *o, int n);
void ObjCullRegister(TObj *o);
void weapon5to7Swing(TObj *o);
void freeObjectLayer1(TObj *o);

void weapon5to7ObjTick(TObj *o)
{
    unsigned char st;
    if (PLX.b60ff != 0) {
        o->b04 = 2;
    }
    st = o->b04;
    switch (st) {
    case 0:
        if (o->state == 0) {
            o->w22 = 0;
            weapon5to7Init(o, ((unsigned char*)&GV2.bb2)[0] - 5);
        }
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            weapon5to7Swing(o);
            break;
        case 1:
            weapon5to7Swing(o);
            break;
        case 2:
            weapon5to7Swing(o);
            break;
        }
        break;
    case 2:
        if (--D_800A611B < 0) {
            D_800A611B = 0;
        }
        o->b04 = 3;
        break;
    case 3:
        freeObjectLayer1(o);
        break;
    }
}

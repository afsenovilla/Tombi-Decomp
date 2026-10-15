// FUNC 80034f58 324 MAIN0
// MATCHING 80034f58 324
// VERSION SLPM_80165 slpm80165 80032E84 296
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80034F58 (func_80034F58.c): steps 0 and 1 run the handler, step 2 does nothing.
#include "plx.h"
#include "gv2.h"
#include "TOBJ.H"
extern signed char D_800A611B;
void FUN_8003509c(TObj *o, int n);
void ObjCullRegister(TObj *o);
void func_80033B44(TObj *o);
void freeObjectLayer1(TObj *o);

void func_80034F58(TObj *o)
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
            FUN_8003509c(o, ((unsigned char*)&GV2.bb2)[0] - 5);
        }
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_80033B44(o);
            break;
        case 1:
            func_80033B44(o);
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
#else
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
#endif

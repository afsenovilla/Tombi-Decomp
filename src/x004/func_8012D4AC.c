// FUNC 8012d4ac 732 X004
// MATCHING 8012d4ac 732
#include "TOBJ.H"

extern void *D_80135748[], *D_8013577C[], *D_801357CC[], *D_801357D8[];
extern int D_1F8002DC;
extern int GetClut(int, int);
extern void AnimLoadDuration(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void FUN_80018790(TObj *o);
extern void func_8012C004(TObj *o);
extern void func_8012C1A8(TObj *o);
extern void func_8012C3D0(TObj *o);
extern void func_8012C574(TObj *o);
extern void func_8012C718(TObj *o);
extern void func_8012C884(TObj *o);
extern void func_8012C9F0(TObj *o);
extern void func_8012D198(TObj *o);

void func_8012D4AC(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->active = 3;
        switch (o->b0c) {
        case 0:
            o->box0 = 6;
            o->box1 = 0xc;
            o->box2 = 0x18;
            o->box3 = 0x20;
            o->w1e = 1;
            o->b0d = 1;
            o->w08 = GetClut(0x120, 0x1e0);
            o->wac = 0;
            o->anim = D_80135748[0];
            break;
        case 1:
            o->box0 = 6;
            o->box1 = 0xc;
            o->box2 = 8;
            o->box3 = 0x10;
            o->w1e = 2;
            o->b0d = 1;
            o->w08 = GetClut(0x120, 0x1e1);
            o->wac = 0;
            o->anim = D_8013577C[0];
            break;
        case 2:
            o->box0 = 0xa;
            o->box1 = 0x14;
            o->box2 = 0x1a;
            o->box3 = 0x22;
            o->w1e = 3;
            o->b0d = 1;
            o->w08 = GetClut(0x120, 0x1e2);
            o->wac = 7;
            o->anim = D_801357CC[0];
            break;
        case 3:
            o->box0 = 0x10;
            o->box1 = 0x20;
            o->box2 = 0x18;
            o->box3 = 0x30;
            o->w1e = 4;
            o->b0d = 1;
            o->w08 = GetClut(0x120, 0x1e3);
            o->wac = 0;
            o->anim = D_801357D8[0];
            break;
        }
        o->d3c = D_1F8002DC;
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0: func_8012C004(o); break;
        case 1: func_8012C1A8(o); break;
        case 2: func_8012C3D0(o); break;
        case 3: func_8012C574(o); break;
        case 4: func_8012C718(o); break;
        case 5: func_8012C884(o); break;
        case 6: func_8012C9F0(o); break;
        case 7: func_8012D198(o); break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

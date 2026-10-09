// FUNC 801307b0 480 X003
// MATCHING 801307b0 480
#include "TOBJ.H"

extern unsigned char D_8009D2C3, D_8009CDD9;
extern int D_1F8002D0[];
extern int D_1F8002F8[];
extern void *D_8013A368[];
extern void *D_8013A3A8[];
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);
extern void func_801301C0(TObj *);
extern void func_8013034C(TObj *);
extern void func_801304B8(TObj *);
extern void func_80130644(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_801307B0(TObj *o)
{
    unsigned char t = o->b04;
    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->active = 3;
        *(signed char *)&o->b0f = -4;
        if (!(D_8009D2C3 & 0x10)) {
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 0xc;
            o->box3 = 0x1c;
            o->w1e = 1;
            o->b0d = 0;
            o->d3c = D_1F8002D0[0];
            o->wac = 0;
            setAnim(o, D_8013A368[0]);
        } else {
            o->box0 = 10;
            o->box1 = 0x14;
            o->box2 = 0x18;
            o->box3 = 0x2e;
            o->w1e = 10;
            o->b0d = 1;
            o->w08 = FUN_8005e420(0xd0, 0x1e2);
            o->d3c = D_1F8002F8[0];
            o->wac = 0;
            setAnim(o, D_8013A3A8[0]);
        }
        break;
    case 1:
        AnimAdvance(o);
        ObjCullRegister(o);
        if (!(D_8009D2C3 & 0x10)) {
            if (D_8009CDD9 == 0) func_801301C0(o);
            else func_8013034C(o);
        } else {
            if (D_8009CDD9 == 0) func_801304B8(o);
            else func_80130644(o);
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

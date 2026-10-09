// FUNC 8013649c 704 X001
// MATCHING 8013649c 704
#include "TOBJ.H"

extern unsigned short D_8009C960[], D_8009C962[];
extern int DAT_1f8002d0[], DAT_1f8002e4[];
extern void *D_8013DE3C[], *D_8013DDE4[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_801356C0(TObj *);
extern void func_80135850(TObj *);
extern void func_80135164(TObj *);
extern void func_80135C90(TObj *);
extern void func_80136164(TObj *);
extern void FUN_80018790(TObj *);

void func_8013649C(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (o->subtype == 0) {
        o->box0 = 8;
        o->box1 = 0x10;
        o->box3 = 0x10;
        o->box2 = 8;
        o->w1e = 1;
        o->b0d = 0;
        if (D_8009C960[0] == 1 && D_8009C962[0] < 2) {
            o->d3c = DAT_1f8002d0[0];
            o->anim = D_8013DE3C[0];
        } else {
            o->d3c = DAT_1f8002e4[0];
            o->anim = D_8013DDE4[0];
        }
        o->category |= 0x80;
        FUN_8001fe6c(o);
        o->b04++;
        } else {
            func_801356C0(o);
        }
        break;
    case 1:
        if (FUN_800202b4(o) == 0)
            break;
        if (o->subtype == 0) {
            func_80135850(o);
            break;
        }
        if (*(int *)D_8009C960 == 0x20001)
            o->w1e = 2;
        else
            o->w1e = 1;
        switch (o->subtype) {
        case 1:
        case 4:
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            break;
        case 2:
        case 3:
            FUN_8001fec0(o);
            break;
        }
        break;
    case 2:
        FUN_800202b4(o);
        if (*(int *)D_8009C960 == 0x20001)
            o->w1e = 2;
        else
            o->w1e = 1;
        switch (o->subtype) {
        case 0:
            switch (o->step) {
            case 0:
                o->step = 1;
                break;
            case 1:
                FUN_8001fec0(o);
                break;
            case 2:
                o->b04 = 3;
                break;
            case 3:
                func_80135164(o);
                break;
            case 4:
                if (o->visible == 0)
                    o->b04 = 3;
                break;
            }
            break;
        case 1:
        case 2:
            func_80135C90(o);
            break;
        case 3:
        case 4:
            func_80136164(o);
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

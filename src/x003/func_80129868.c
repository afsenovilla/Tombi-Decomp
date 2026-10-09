// FUNC 80129868 452 X003
// MATCHING 80129868 452
#include "TOBJ.H"

extern unsigned char D_8009D2C3;
extern short *D_800A6078;
extern void func_80128A1C(TObj *);
extern void func_80128DF8(TObj *);
extern void func_80128C08(TObj *);
extern void func_80128FA0(TObj *);
extern void func_80129240(TObj *);
extern void func_80129430(TObj *);
extern void func_8012962C(TObj *);
extern int FUN_8001fec0(TObj *);

void func_80129868(TObj *o)
{
    switch ((unsigned short)o->wba) {
    case 0:
        switch (o->step) {
        case 0:
            func_80128A1C(o);
            break;
        case 1:
            goto anim;
        case 2:
            func_80128DF8(o);
            break;
        }
        break;
    case 1:
        switch (o->step) {
        case 0:
            func_80128C08(o);
            break;
        case 1:
            goto anim;
        case 2:
            func_80128FA0(o);
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            goto anim;
        case 1:
            func_80129240(o);
            break;
        case 2:
            func_80129430(o);
            break;
        }
        break;
    case 3:
        switch (o->step) {
        case 0:
            goto anim;
        case 1:
            func_8012962C(o);
            break;
        }
        break;
    case 4:
        if (!(D_8009D2C3 & 4)) {
            if (D_800A6078[1] > 0xa0) D_800A6078[1] = 0xa0;
        }
    anim:
        FUN_8001fec0(o);
        break;
    }
}

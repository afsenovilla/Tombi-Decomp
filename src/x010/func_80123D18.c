// FUNC 80123d18 436 X010
// MATCHING 80123d18 436
#include "TOBJ.H"

extern void *D_80131DC0;
extern unsigned char D_8009C959;
extern unsigned short D_8009C960;
extern unsigned char D_8009CF0A;
extern unsigned char D_8009CFC8;
void ObjCullRegister(TObj *o);
void AnimLoadDuration(TObj *o);
void addItemToInventory(int a, int b, int c);
void FUN_80018790(TObj *o);
void func_80122F48(TObj *o);
void func_801230D0(TObj *o);
void func_80123330(TObj *o);
void func_801234C8(TObj *o);
void func_80123708(TObj *o);

void func_80123D18(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80122F48(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->step == 0) {
            func_801230D0(o);
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_80123330(o);
            break;
        case 1:
        case 2:
            if (o->state == 0) {
                o->anim = D_80131DC0;
                AnimLoadDuration(o);
                D_8009C959--;
                addItemToInventory(0x10, 1, 1);
                if (D_8009C960 == 1) {
                    D_8009CF0A++;
                    o->b04 = 3;
                    o->step = 0;
                    o->state = 0;
                } else {
                    D_8009CFC8++;
                    o->b04 = 3;
                    o->step = 0;
                    o->state = 0;
                }
            }
            break;
        case 3:
            func_801234C8(o);
            break;
        case 4:
            func_80123708(o);
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

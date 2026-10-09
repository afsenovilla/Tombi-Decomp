// FUNC 8013721c 676 X001
// MATCHING 8013721c 676
#include "TOBJ.H"

extern unsigned char D_8009CEAB, D_8009CEAD;
extern int D_1F8002D4[];
extern void *D_8013E570[], *D_8013E700;
extern unsigned short *D_800A6078;
extern short D_800A604E;
void AnimLoadDuration(TObj *o);
void AnimAdvance(TObj *o);
int ObjCullRegister(TObj *o);
int Rand(void);
void FUN_8001e5f4(int a, int b);
void func_80136B4C(TObj *o);
void addItemToInventory(int a, int b, int c);
void FUN_80018790(TObj *o);

void func_8013721C(TObj *o)
{
    int v;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if (D_8009CEAB == 0 || D_8009CEAB >= 3) {
                o->b04 = 3;
                break;
            }
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 0x18;
            o->box3 = 0x20;
            o->d8c = 0;
            o->w1e = 0xa;
            o->b0d = 0;
            o->category |= 0x80;
            o->d3c = D_1F8002D4[0];
            o->anim = D_8013E570[0];
            o->b0a = 2;
            AnimLoadDuration(o);
            *(signed char *)&o->b0f = -10;
            o->b04 = 1;
            break;
        case 1:
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 0x18;
            o->box3 = 0x20;
            o->d8c = 0;
            o->w1e = 0xa;
            o->b0d = 0;
            o->category |= 0x80;
            o->d3c = D_1F8002D4[0];
            o->anim = D_8013E700;
            o->b0a = 2;
            AnimLoadDuration(o);
            o->b04 = 1;
            if (D_8009CEAD) {
                o->b04 = 3;
            }
            break;
        }
        break;
    case 1:
        if ((unsigned short)(D_800A6078[1] - 0x8ae) < 0x200 && (Rand() & 0x3f) == 0) {
            if (D_800A604E < -0x190) {
                v = 0x7f;
            } else if (D_800A604E < -0xc8) {
                v = 0x70;
            } else {
                v = 0x60;
            }
            FUN_8001e5f4(0x41, v);
        }
        if (ObjCullRegister(o) && o->subtype == 0) {
            AnimAdvance(o);
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            func_80136B4C(o);
            break;
        case 1:
            addItemToInventory(10, 1, 1);
            D_8009CEAD = 1;
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

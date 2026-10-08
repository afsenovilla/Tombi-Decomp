/* score 4: only scheduling in case 0: game loads lbu b04 after the two volatile D_1F800334 reads and stores d84 before b69/b04;
   tried: hill-climbed statement order, raw-offset stores, b04 increment forms, pp forms, do-while/asm barriers. Includes csv piece 80116D50. */
// FUNC 80116d14 320 X011
#include "TOBJ.H"
extern unsigned char D_8009D100;
extern unsigned char D_8009CE3A;
extern int D_1F800334;
extern int ObjCullRegister(TObj *);
extern void addItemToInventory(int, int, int);
extern void FUN_800188e0(TObj *);

void func_80116D14(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D100 != 0 || D_8009CE3A == 0) {
            o->b04 = 3;
            break;
        }
        {
            volatile int *pp = &D_1F800334;
            int n, base;
            n = *(int *)(*pp + 0x20);
            base = *pp;
            o->box1 = 0x10;
            o->box3 = 0x10;
            o->ba4 = 0;
            o->box0 = 8;
            o->active = 1;
            o->box2 = 8;
            o->b69 = 0;
            o->b04++;
            o->d84 = 0x80;
            o->da0 = base + n;
        }
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        if (D_8009D100 == 0) addItemToInventory(0x5c, 1, 1);
        o->b04++;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}

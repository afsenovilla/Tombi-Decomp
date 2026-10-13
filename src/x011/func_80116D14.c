// FUNC 80116d14 320 X011
// MATCHING 80116d14 320
#include "TOBJ.H"
extern unsigned char D_8009D100;
extern unsigned char D_8009CE3A;
extern int D_1F800334;
extern int ObjCullRegister(TObj *);
extern void addItemToInventory(int, int, int);
extern void freeObjectLayer5(TObj *);

/* Same shape as x001 func_80123F24 (volatile pointer local + q offset, sum into v). Includes csv piece 80116D50. */
void func_80116D14(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D100 != 0 || D_8009CE3A == 0) {
            o->b04 = 3;
            break;
        }
        {
            volatile int *g = &D_1F800334;
            int *q = (int *)*g;
            int v;
            unsigned char t;
            q += 8;
            v = *g + *q;
            o->ba4 = 0;
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 8;
            o->box3 = 0x10;
            o->active = 1;
            o->d84 = 0x80;
            o->b69 = 0;
            t = *(volatile unsigned char *)&o->b04;
            t++;
            o->b04 = t;
            o->da0 = v;
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
        freeObjectLayer5(o);
        break;
    }
}

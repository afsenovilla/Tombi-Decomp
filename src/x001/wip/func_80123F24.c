// FUNC 80123f24 308 X001
/* score 4: case 0: second read of D_1F800334 lands after lbu b04 (game: both lw (v1) adjacent); tried store-order hill climb, temps/volatile pointer variants; also 3000 random store orders with and without the q/v reads movable, setbox inline, b04 temp, TObj*/int*volatile* casts, split v=w+q[9] (all >= 4) */
#include "TOBJ.H"

extern unsigned char D_8009CDBA;
extern int D_1F800334;
extern int ObjCullRegister(TObj *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_80026c50(int, int, int);
extern void FUN_800188e0(TObj *);

void func_80123F24(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009CDBA == 0) {
            volatile int *g = &D_1F800334;
            int *q = (int *)*g;
            int v = q[9] + *g;
            o->ba4 = 0;
            o->box0 = 8;
            o->box1 = 0x10;
            o->box3 = 0x10;
            o->box2 = 8;
            o->active = 1;
            o->b69 = 0;
            o->b04++;
            o->da0 = v;
        } else {
            o->b04 = 3;
        }
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        if (D_8009CDBA == 0) {
            FUN_8005a8a8(0x16, 0, 0);
            FUN_80026c50(0xb, 1, 1);
        }
        o->b04++;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}

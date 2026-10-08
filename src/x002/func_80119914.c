// FUNC 80119914 264 X002
// MATCHING 80119914 264
#include "TOBJ.H"
typedef struct { short xf, x, yf, y, zf, z; } V6;
extern unsigned int D_8009C96C, D_8009C98C;
extern unsigned char D_8009CDCB, D_8009CFE4;
extern TObj *ObjSpawnType0(int, int, V6 *);
extern void func_80119788(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_80119914(TObj *o)
{
    V6 v;

    switch (o->b04) {
    case 0:
        if (D_8009C96C >= D_8009C98C && D_8009CDCB != 0 && D_8009CFE4 == 0) {
            v.x = 0xb0;
            v.y = -0x38;
            v.z = 0;
            ObjSpawnType0(0x5f, 0, &v);
        }
        o->b04++;
        break;
    case 1:
        func_80119788(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}

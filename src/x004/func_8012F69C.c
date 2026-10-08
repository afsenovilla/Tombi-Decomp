// FUNC 8012f69c 192 X004
// MATCHING 8012f69c 192
#include "TOBJ.H"
extern unsigned char D_8009D2C3;
extern void func_8012F508(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8012F69C(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D2C3 & 8) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        if (o->step == 0) func_8012F508(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}

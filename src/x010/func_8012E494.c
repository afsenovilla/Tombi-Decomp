// FUNC 8012e494 188 X010
// MATCHING 8012e494 188
#include "TOBJ.H"

extern unsigned char D_8009C93A;
extern void func_8012E114(TObj *);
extern void func_8012E328(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8012E494(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012E114(o);
        o->b04++;
        break;
    case 1:
        if (D_8009C93A) func_8012E328(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}

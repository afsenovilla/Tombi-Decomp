// FUNC 8012d858 140 X010
// MATCHING 8012d858 140
#include "TOBJ.H"
extern void func_8012D678(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8012D858(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        func_8012D678(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}

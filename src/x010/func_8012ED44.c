// FUNC 8012ed44 212 X010
// MATCHING 8012ed44 212
#include "TOBJ.H"
extern unsigned char D_8009CE1F;
extern void func_8012EBA0(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8012ED44(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (D_8009CE1F == 0) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            func_8012EBA0(o);
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}

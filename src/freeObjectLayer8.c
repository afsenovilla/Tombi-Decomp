// FUNC 8001888c 84 MAIN0
// MATCHING 8001888c 84
#include "TOBJ.H"
extern unsigned short FREE_OBJECT_COUNT_LAYER8;
extern int *FREE_OBJECTS_LAYER8;

void freeObjectLayer8(TObj *o)
{
    ((int *)o)[0] = 0;
    ((int *)o)[1] = 0;
    ((int *)o)[2] = 0;
    ((int *)o)[3] = 0;
    o->b9c = 0;
    o->b9d = 0;
    o->category &= 0x7f;
    FREE_OBJECT_COUNT_LAYER8++;
    FREE_OBJECTS_LAYER8--;
    *FREE_OBJECTS_LAYER8 = (int)o;
}

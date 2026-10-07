// FUNC 8001faf4 44 MAIN0
// MATCHING 8001faf4 44
#include "TOBJ.H"
void FUN_8001faf4(TObj *o)
{
    o->h->raw += (*(short *)((char *)o->movetab + (o->animFrame << 2))) << 8;
}

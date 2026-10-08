// FUNC 800ef460 48 X014
// MATCHING 800ef460 48
#include "TOBJ.H"

void func_800EF460(TObj *o)
{
    o->d->raw += o->velH << 8;
    o->y.raw += o->velV << 8;
}

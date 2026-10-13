// FUNC 8002cce4 60 MAIN0
// MATCHING 8002cce4 60
#include "TOBJ.H"
extern void (*D_80079D44[])(TObj *);

void dispatchMsgBoxHandler(TObj *o)
{
    D_80079D44[o->subtype](o);
}

// FUNC 8002cce4 60 MAIN0
// MATCHING 8002cce4 60
#include "TOBJ.H"
extern void (*MSG_BOX_HANDLERS[])(TObj *);

void dispatchMsgBoxHandler(TObj *o)
{
    MSG_BOX_HANDLERS[o->subtype](o);
}

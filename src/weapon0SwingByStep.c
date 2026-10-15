// FUNC 80032fec 84 MAIN0
// MATCHING 80032fec 84
#include "TOBJ.H"
extern void weapon0Swing(TObj *);

void weapon0SwingByStep(TObj *o)
{
    switch (o->step) {
    case 0:
        weapon0Swing(o);
        break;
    case 1:
        weapon0Swing(o);
        break;
    case 2:
        weapon0Swing(o);
        break;
    }
}

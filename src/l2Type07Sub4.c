// FUNC 801302a0 116 X000
// MATCHING 801302a0 116
#include "TOBJ.H"
extern void l2Type07Sub4Step(TObj *);

void l2Type07Sub4(TObj *o)
{
    TObj *c;
    int i;
    if (o->subtype == 0) {
        i = 1;
        l2Type07Sub4Step(o);
        c = (TObj *)o->d94;
        do {
            l2Type07Sub4Step(c);
            c->state = o->state;
            c = (TObj *)c->d94;
            i++;
        } while (i < 3);
    }
}

// FUNC 80129f10 184 X004
// MATCHING 80129f10 184
#include "TOBJ.H"
extern void paintingSlotHintTalk(TObj *);
extern void paintingSlotHintNpcMoveTo(TObj *);
extern void AnimAdvance(TObj *);

void stepPaintingSlotHintTalk(TObj *o)
{
    switch (o->subtype) {
    case 0:
        switch (o->step) {
        case 0:
            if (o->b68) o->step++;
            break;
        case 1:
            paintingSlotHintTalk(o);
            break;
        case 3:
            AnimAdvance(o);
            break;
        }
        break;
    case 1:
        paintingSlotHintNpcMoveTo(o);
        break;
    }
}

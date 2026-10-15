// FUNC 8011faa8 104 X001
// MATCHING 8011faa8 104
#include "TOBJ.H"
extern void bucketFillWithWater(TObj *);
extern void bucketThrowWater(TObj *);

void stepBucket(TObj *o)
{
    if (o->visible == 0) return;
    switch (o->step) {
    case 0:
        break;
    case 1:
        bucketFillWithWater(o);
        break;
    case 2:
        bucketThrowWater(o);
        break;
    }
}

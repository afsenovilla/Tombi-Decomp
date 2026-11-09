// FUNC 8011fe38 144 X003
// MATCHING 8011fe38 144
#include "TOBJ.H"
extern short probeGrabPointInBoxA(void);
extern short GRAB_POINT_OFFSET;
extern short LIST_COUNTER;
extern TObj *TOUCHED_OBJECT;

void playerGrabCat4Type02(TObj *a, TObj *b)
{
    if (b->d94 == 0 && probeGrabPointInBoxA() != 0) {
        a->b9e = 4;
        a->wba = -3;
        a->velY = 0;
        LIST_COUNTER = 0;
        a->wb8 = GRAB_POINT_OFFSET;
        b->b68 = 2;
        TOUCHED_OBJECT = b;
        b->animFrame = a->animFrame & 1;
    }
}

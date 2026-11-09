// FUNC 80121254 112 X009
// MATCHING 80121254 112
#include "TOBJ.H"
extern short probeGrabPointInBoxA(void);
extern short GRAB_POINT_OFFSET;
extern short LIST_COUNTER;
extern TObj *TOUCHED_OBJECT;

void jellyfishGrabProbe(TObj *a, TObj *b)
{
    if (probeGrabPointInBoxA() != 0) {
        a->b9e = 4;
        a->wba = 0;
        a->velY = 0;
        LIST_COUNTER = 0;
        a->wb8 = GRAB_POINT_OFFSET;
        b->ba7 = 1;
        TOUCHED_OBJECT = b;
    }
}

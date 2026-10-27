// FUNC 80121254 112 X009
// MATCHING 80121254 112
#include "TOBJ.H"
extern short probeGrabPointInBoxA(void);
extern short GRAB_POINT_OFFSET;
extern short DAT_1f80019e;
extern TObj *DAT_1f8003c0;

void jellyfishGrabProbe(TObj *a, TObj *b)
{
    if (probeGrabPointInBoxA() != 0) {
        a->b9e = 4;
        a->wba = 0;
        a->velY = 0;
        DAT_1f80019e = 0;
        a->wb8 = GRAB_POINT_OFFSET;
        b->ba7 = 1;
        DAT_1f8003c0 = b;
    }
}

// FUNC 80027124 56 MAIN0
// MATCHING 80027124 56
#include "camera.h"
extern void updateCameraController(CameraScroll *);
void updateInventoryOverlay(void)
{
    if (CAMERA_SCROLL.active != 0)
        updateCameraController(&CAMERA_SCROLL);
}

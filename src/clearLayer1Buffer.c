// FUNC 80018128 44 MAIN0
// MATCHING 80018128 44
extern void memfill(void *p, int v, int n);
#include "camera.h"

void clearLayer1Buffer(void)
{
    memfill(&CAMERA_SCROLL, 0, sizeof(CameraScroll));
}

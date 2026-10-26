// FUNC 80027124 56 MAIN0
// MATCHING 80027124 56
extern unsigned char CAMERA_SCROLL[];
extern void updateCameraController(unsigned char *);
void updateInventoryOverlay(void)
{
    if (CAMERA_SCROLL[0] != 0)
        updateCameraController(CAMERA_SCROLL);
}

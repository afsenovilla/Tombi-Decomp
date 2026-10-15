// FUNC 80027124 56 MAIN0
// MATCHING 80027124 56
extern unsigned char DAT_800a4550[];
extern void updateCameraController(unsigned char *);
void updateInventoryOverlay(void)
{
    if (DAT_800a4550[0] != 0)
        updateCameraController(DAT_800a4550);
}

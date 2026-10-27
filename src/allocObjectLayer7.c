// FUNC 80018678 128 MAIN0
// MATCHING 80018678 128
extern short FREE_OBJECT_COUNT_LAYER7;
extern int *FREE_OBJECTS_LAYER7;
extern unsigned short SPAWN_AXIS_SWAP;

int allocObjectLayer7(void)
{
    int r;
    int *p;
    if (FREE_OBJECT_COUNT_LAYER7 > 0) {
        FREE_OBJECT_COUNT_LAYER7 = FREE_OBJECT_COUNT_LAYER7 - 1;
        p = FREE_OBJECTS_LAYER7;
        FREE_OBJECTS_LAYER7 = p + 1;
        r = *p;
        if ((SPAWN_AXIS_SWAP & 1) == 0) {
            *(int *)(r + 0x40) = r + 0x10;
            *(int *)(r + 0x44) = r + 0x18;
        } else {
            *(int *)(r + 0x44) = r + 0x10;
            *(int *)(r + 0x40) = r + 0x18;
        }
        return r;
    }
    return 0;
}

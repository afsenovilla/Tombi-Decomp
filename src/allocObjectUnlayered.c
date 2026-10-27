// FUNC 800186f8 76 MAIN0
// MATCHING 800186f8 76
extern short FREE_OBJECT_COUNT_UNLAYERED;
extern int *FREE_OBJECTS_UNLAYERED;

int allocObjectUnlayered(void)
{
    int r;
    if (FREE_OBJECT_COUNT_UNLAYERED < 1) {
        r = 0;
    } else {
        int *p = FREE_OBJECTS_UNLAYERED;
        FREE_OBJECT_COUNT_UNLAYERED = FREE_OBJECT_COUNT_UNLAYERED - 1;
        FREE_OBJECTS_UNLAYERED = p + 1;
        r = *p;
    }
    return r;
}

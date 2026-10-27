// FUNC 800182ac 128 MAIN0
// MATCHING 800182ac 128
extern short FREE_OBJECT_COUNT_LAYER1;
extern int **DAT_1f800204;
extern unsigned short SPAWN_AXIS_SWAP;

int *allocObjectLayer1(void)
{
    int *o;
    short n = FREE_OBJECT_COUNT_LAYER1;
    if (n > 0) {
        FREE_OBJECT_COUNT_LAYER1 = n - 1;
        o = *DAT_1f800204++;
        if ((SPAWN_AXIS_SWAP & 1) == 0) {
            o[0x10] = (int)(o + 4);
            o[0x11] = (int)(o + 6);
        } else {
            o[0x11] = (int)(o + 4);
            o[0x10] = (int)(o + 6);
        }
        return o;
    }
    return 0;
}

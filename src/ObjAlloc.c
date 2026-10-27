// FUNC 80018448 144 MAIN0
// MATCHING 80018448 144
// FLAGS -O2 -G0 -fno-schedule-insns2
extern short FREE_OBJECT_COUNT;
extern int **FREE_OBJECTS;
extern unsigned short SPAWN_AXIS_SWAP;

int *ObjAlloc(void)
{
    char pad;
    int *o;
    int **q;
    short n = FREE_OBJECT_COUNT;
    char c = 3;
    if (n > 0) {
        q = FREE_OBJECTS;
        FREE_OBJECT_COUNT = n - 1;
        FREE_OBJECTS = q + 1;
        o = *q;
        *((char *)o + 0x1c) = c;
        if ((SPAWN_AXIS_SWAP & 1) == 0) {
            o[16] = (int)(o + 4);
            o[17] = (int)((char *)o + 0x18);
        } else {
            o[17] = (int)(o + 4);
            o[16] = (int)((char *)o + 0x18);
        }
        return o;
    }
    return 0;
}

// FUNC 800183b8 144 MAIN0
// MATCHING 800183b8 144
extern short FREE_OBJECT_COUNT;
extern int *FREE_OBJECTS;
extern unsigned short SPAWN_AXIS_SWAP;
int allocObjectLayer2(void)
{
    int o;
    short n;
    char c;
    char pad;
    n = FREE_OBJECT_COUNT;
    c = 2;
    if (n > 0) {
        FREE_OBJECT_COUNT = n - 1;
        o = *FREE_OBJECTS++;
        *(char *)(o + 0x1c) = c;
        if ((SPAWN_AXIS_SWAP & 1) == 0) {
            *(int *)(o + 0x40) = o + 0x10;
            *(int *)(o + 0x44) = o + 0x18;
        } else {
            *(int *)(o + 0x44) = o + 0x10;
            *(int *)(o + 0x40) = o + 0x18;
        }
        return o;
    }
    return 0;
}

// FUNC 80018568 144 MAIN0
// MATCHING 80018568 144
extern short FREE_OBJECT_COUNT;
extern char **FREE_OBJECTS;
extern unsigned short SPAWN_AXIS_SWAP;

char *allocObjectLayer5(void)
{
    char *p;
    char c = 5;
    short n = FREE_OBJECT_COUNT;
    if (n > 0) {
        FREE_OBJECT_COUNT = n - 1;
        p = *FREE_OBJECTS++;
        p[0x1c] = c;
        if ((SPAWN_AXIS_SWAP & 1) == 0) {
            *(char **)(p + 0x40) = p + 0x10;
            *(char **)(p + 0x44) = p + 0x18;
        } else {
            *(char **)(p + 0x44) = p + 0x10;
            *(char **)(p + 0x40) = p + 0x18;
        }
        return p;
    }
    return 0;
}

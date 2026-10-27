// FUNC 8003bd80 148 MAIN0
// MATCHING 8003bd80 148
extern int UPDATE_OBJECT_INDEX;
extern char DAT_800b1478[];
extern void (*PTR_8007a10c[])(char *);
void updateObjectsLayer1(void)
{
    char *p = DAT_800b1478;
    UPDATE_OBJECT_INDEX = 0;
    do {
        if (*p != 0)
            PTR_8007a10c[(unsigned char)p[2]](p);
        UPDATE_OBJECT_INDEX = UPDATE_OBJECT_INDEX + 1;
        p += 0xec;
    } while (UPDATE_OBJECT_INDEX < 4);
}

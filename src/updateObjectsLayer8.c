// FUNC 8002b020 148 MAIN0
// MATCHING 8002b020 148
extern char DAT_800b1828[];
extern void (*PTR_DAT_80079b88[])(char *);
extern int UPDATE_OBJECT_INDEX;

void updateObjectsLayer8(void)
{
    char *p = DAT_800b1828;
    UPDATE_OBJECT_INDEX = 0;
    do {
        if (*p != 0)
            PTR_DAT_80079b88[(unsigned char)p[2]](p);
        UPDATE_OBJECT_INDEX++;
        p += 0xd4;
    } while (UPDATE_OBJECT_INDEX < 0x2d);
}

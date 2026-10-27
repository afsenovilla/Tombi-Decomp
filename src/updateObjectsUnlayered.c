// FUNC 8004c0dc 148 MAIN0
// MATCHING 8004c0dc 148
extern int UPDATE_OBJECT_INDEX;
extern unsigned char DAT_800a6268[];
extern void (*PTR_8007bf34[])(unsigned char *);

void updateObjectsUnlayered(void)
{
    unsigned char *p = DAT_800a6268;
    UPDATE_OBJECT_INDEX = 0;
    do {
        if (*p != 0) {
            PTR_8007bf34[p[2]](p);
        }
        UPDATE_OBJECT_INDEX = UPDATE_OBJECT_INDEX + 1;
        p += 0x3c;
    } while (UPDATE_OBJECT_INDEX < 10);
}

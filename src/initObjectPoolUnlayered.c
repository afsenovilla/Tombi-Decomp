// FUNC 80017d30 164 MAIN0
// MATCHING 80017d30 164
extern unsigned char DAT_800a6268[];
extern unsigned char DAT_800a6484[];
extern unsigned char DAT_800b11b8[];
extern unsigned char **FREE_OBJECTS_UNLAYERED;
extern short FREE_OBJECT_COUNT_UNLAYERED;
extern void *memset(void *, int, int);

void initObjectPoolUnlayered(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = DAT_800a6268;
    do {
        memset(p, 0, 0x3c);
        i++;
        p += 0x3c;
    } while (i < 10);
    q = DAT_800a6484;
    FREE_OBJECTS_UNLAYERED = (unsigned char **)DAT_800b11b8;
    i = 0;
    do {
        i++;
        *--FREE_OBJECTS_UNLAYERED = q;
        q -= 0x3c;
    } while (i < 10);
    FREE_OBJECT_COUNT_UNLAYERED = 10;
}

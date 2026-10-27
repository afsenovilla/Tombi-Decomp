// FUNC 80017ea8 168 MAIN0
// MATCHING 80017ea8 168
extern char DAT_800a6610[];
extern char DAT_800b0adc[];
extern char DAT_800a49a8[];
extern char *FREE_OBJECTS;
extern short FREE_OBJECT_COUNT;
extern void memset(void *, int, int);

void initObjectPool(void)
{
    int i;
    char *p;
    char *r;
    char **q;
    i = 0;
    p = DAT_800a6610;
    do {
        memset(p, 0, 0xd4);
        i++;
        p += 0xd4;
    } while (i < 200);
    r = DAT_800b0adc;
    FREE_OBJECTS = DAT_800a49a8;
    i = 0;
    do {
        r[0x1c] = 0;
        i++;
        q = (char **)(FREE_OBJECTS - 4);
        FREE_OBJECTS = FREE_OBJECTS - 4;
        *q = r;
        r -= 0xd4;
    } while (i < 200);
    FREE_OBJECT_COUNT = 200;
}

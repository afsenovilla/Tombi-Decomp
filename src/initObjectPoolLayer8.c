// FUNC 8001801c 220 MAIN0
// MATCHING 8001801c 220
extern char OBJECT_POOL_LAYER8[];
extern char DAT_800b3c98[];
extern char DAT_800a6264[];
extern char DAT_800b3e24[];
extern char *FREE_OBJECTS_LAYER8;
extern char *CAT8_OBJECT_LIST;
extern char *CAT8_OBJECT_LIST_SAVED;
extern short FREE_OBJECT_COUNT_LAYER8;
extern short CAT8_OBJECT_COUNT_SAVED;
extern short CAT8_OBJECT_COUNT;
extern void memset(char *, int, int);
extern void initSpriteSlots8(void);

void initObjectPoolLayer8(void)
{
    char *p;
    char *q;
    int i;
    char **sp;

    i = 0;
    p = OBJECT_POOL_LAYER8;
    do {
        memset(p, 0, 0xd4);
        i++;
        p += 0xd4;
    } while (i < 0x2d);
    q = DAT_800b3c98;
    FREE_OBJECTS_LAYER8 = DAT_800a6264;
    i = 0;
    do {
        q[0x1c] = 8;
        i++;
        sp = (char **)FREE_OBJECTS_LAYER8 - 1;
        FREE_OBJECTS_LAYER8 = (char *)sp;
        *sp = q;
        q -= 0xd4;
    } while (i < 0x2d);
    FREE_OBJECT_COUNT_LAYER8 = 0x2d;
    CAT8_OBJECT_LIST = DAT_800b3e24;
    CAT8_OBJECT_LIST_SAVED = DAT_800b3e24;
    CAT8_OBJECT_COUNT_SAVED = 0;
    CAT8_OBJECT_COUNT = 0;
    initSpriteSlots8();
}

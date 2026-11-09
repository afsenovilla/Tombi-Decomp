// FUNC 80017dd4 212 MAIN0
// MATCHING 80017dd4 212
extern unsigned char OBJECT_POOL_LAYER1[];
extern unsigned char DAT_800b173c[];
extern unsigned char CAMERA_SCROLL[];
extern unsigned char ONSCREEN_OBJECT_LIST_BUF[];
extern unsigned char **FREE_OBJECTS_LAYER1;
extern short FREE_OBJECT_COUNT_LAYER1;
extern unsigned char *ONSCREEN_OBJECT_LIST;
extern unsigned char *ONSCREEN_OBJECT_LIST_SAVED;
extern short ONSCREEN_OBJECT_COUNT_SAVED;
extern short ONSCREEN_OBJECT_COUNT;
extern void *memset(void *, int, int);

void initObjectPoolLayer1(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = OBJECT_POOL_LAYER1;
    do {
        memset(p, 0, 0xec);
        i++;
        p += 0xec;
    } while (i < 4);
    q = DAT_800b173c;
    FREE_OBJECTS_LAYER1 = (unsigned char **)CAMERA_SCROLL;
    i = 0;
    do {
        q[0x1c] = 1;
        i++;
        *--FREE_OBJECTS_LAYER1 = q;
        q -= 0xec;
    } while (i < 4);
    FREE_OBJECT_COUNT_LAYER1 = 4;
    ONSCREEN_OBJECT_LIST = ONSCREEN_OBJECT_LIST_BUF;
    ONSCREEN_OBJECT_LIST_SAVED = ONSCREEN_OBJECT_LIST_BUF;
    ONSCREEN_OBJECT_COUNT_SAVED = 0;
    ONSCREEN_OBJECT_COUNT = 0;
}

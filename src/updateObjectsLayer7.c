// FUNC 80030f38 148 MAIN0
// MATCHING 80030f38 148
extern void (*tab[])(void *);
extern char OBJECT_POOL_LAYER7[];
extern int UPDATE_OBJECT_INDEX;
void updateObjectsLayer7(void)
{
    char *p = OBJECT_POOL_LAYER7;
    UPDATE_OBJECT_INDEX = 0;
    do {
        if (*p) tab[(unsigned char)p[2]](p);
        UPDATE_OBJECT_INDEX++;
        p += 0x6c;
    } while (UPDATE_OBJECT_INDEX < 10);
}

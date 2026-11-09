// FUNC 8003bd80 148 MAIN0
// MATCHING 8003bd80 148
extern int UPDATE_OBJECT_INDEX;
extern char OBJECT_POOL_LAYER1[];
extern void (*PTR_8007a10c[])(char *);
void updateObjectsLayer1(void)
{
    char *p = OBJECT_POOL_LAYER1;
    UPDATE_OBJECT_INDEX = 0;
    do {
        if (*p != 0)
            PTR_8007a10c[(unsigned char)p[2]](p);
        UPDATE_OBJECT_INDEX = UPDATE_OBJECT_INDEX + 1;
#if defined(VERSION_PCPX_96084) /* PrePre 8 layer-1 objects are 0xE4 bytes */
        p += 0xe4;
#else
        p += 0xec;
#endif
    } while (UPDATE_OBJECT_INDEX < 4);
}

// VERSION SCUS_94236 scus94236 8005B668 260
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libapi, func_8005B4CC.c); licencia MIT del proyecto original.
typedef int s32;
typedef unsigned int u32;
extern int strcmp(const char *, const char *);
#define NULL 0

typedef struct {
    char* name;
    s32 pad[12];
    s32 (*firstfile)();
    s32 pad2[6];
} DevEntry;

extern s32 firstfile();
extern s32 (*D_8009B108)();
extern char D_8009B110[];

#define DEV_TABLE (*(DevEntry**)0x150)
#define DEV_SIZE (*(u32*)0x154)

s32 psyq_8005bc10(s32* fd, void* a1, void* a2)
{
    DevEntry* dev;
    DevEntry* end;
    DevEntry* tmp;
    s32 (*saved)();

    if (*fd == 0) {
        *fd = 1;
    }
    saved = D_8009B108;
    dev = DEV_TABLE;
    tmp = dev + DEV_SIZE / sizeof(DevEntry);
    if (dev < tmp) {
        end = tmp;
        do {
            if (dev->name != NULL && strcmp(dev->name, D_8009B110) == 0) {
                dev->firstfile = saved;
                break;
            }
            dev++;
        } while (dev < end);
    }
    return D_8009B108(fd, a1, a2);
}
__asm__("nop");
#endif

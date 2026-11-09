// VERSION SCUS_94236 scus94236 8005B4CC 412
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

static inline s32 hook_find(void)
{
    DevEntry* dev;
    DevEntry* end;
    DevEntry* tmp;

    dev = DEV_TABLE;
    tmp = dev + DEV_SIZE / sizeof(DevEntry);
    if (dev < tmp) {
        end = tmp;
        do {
            if (dev->name != NULL && strcmp(dev->name, D_8009B110) == 0) {
                D_8009B108 = dev->firstfile;
                return 1;
            }
            dev++;
        } while (dev < end);
    }
    return 0;
}

s32 psyq_8005bc10();

s32 psyq_8005ba74(signed char* name, void* entry)
{
    DevEntry* dev;
    DevEntry* end;
    DevEntry* tmp;
    signed char* s = name;
    char* d = D_8009B110;

    while (*s >= 0x3B) {
        *d++ = *s++;
    }
    *d = 0;
    if (hook_find() == 0) {
        return 0;
    }
    dev = DEV_TABLE;
    tmp = dev + DEV_SIZE / sizeof(DevEntry);
    if (dev < tmp) {
        end = tmp;
        do {
            if (dev->name != NULL && strcmp(dev->name, D_8009B110) == 0) {
                dev->firstfile = psyq_8005bc10;
                break;
            }
            dev++;
        } while (dev < end);
    }
    return firstfile(name, entry);
}

#endif

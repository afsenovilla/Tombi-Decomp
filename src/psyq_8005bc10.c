// FUNC 8005bc10 256 MAIN0
// MATCHING 8005bc10 256
// FLAGS -O2 -G0
// AS gnu
// Ported from psx_tomba (psyq/libapi, func_8005B4CC.c), MIT licence (THIRD_PARTY_NOTICES.md).
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
extern s32 (*D_8009BDA0)();
extern char D_8009BDA8[];

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
    saved = D_8009BDA0;
    dev = DEV_TABLE;
    tmp = dev + DEV_SIZE / sizeof(DevEntry);
    if (dev < tmp) {
        end = tmp;
        do {
            if (dev->name != NULL && strcmp(dev->name, D_8009BDA8) == 0) {
                dev->firstfile = saved;
                break;
            }
            dev++;
        } while (dev < end);
    }
    return D_8009BDA0(fd, a1, a2);
}

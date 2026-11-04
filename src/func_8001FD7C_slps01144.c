// VERSION SLPS_01144 slps01144 8001FD7C 152
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
typedef struct { unsigned char pad[0x290]; unsigned char *p290; volatile int d294; } SP;
#define SPAD (*(SP *)0x1F800000)
extern int CdGetSector(void *, int);
extern void CdReadyCallback(void *);
extern int CdControlF(int, void *);
void func_8001FD7C_slps01144(unsigned char c)
{
    if (c == 1) {
        CdGetSector(SPAD.p290, 512);
        SPAD.d294 = SPAD.d294 - 1;
        SPAD.p290 += 2048;
        if (SPAD.d294 != 0) return;
    } else {
        SPAD.d294 = -1;
    }
    CdReadyCallback(0);
    CdControlF(9, 0);
}
#endif

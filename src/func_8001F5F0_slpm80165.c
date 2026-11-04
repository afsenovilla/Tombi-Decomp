// VERSION SLPM_80165 slpm80165 8001F5F0 152
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/func_8001FD7C_slps01144.c (func_8001FD7C_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { unsigned char pad[0x290]; unsigned char *p290; volatile int d294; } SP;
#define SPAD (*(SP *)0x1F800000)
extern int CdGetSector(void *, int);
extern void CdReadyCallback(void *);
extern int CdControlF(int, void *);
void func_8001F5F0_slpm80165(unsigned char c)
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

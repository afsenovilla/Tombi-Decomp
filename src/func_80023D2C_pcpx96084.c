// VERSION PCPX_96084 tomba 80023D2C 152
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of JP func_8001FD7C_slps01144: the sector pointer and count are at scratchpad 0x3DC / 0x3EC.
typedef struct { unsigned char pad[0x3dc]; unsigned char *p290; unsigned char pad2[0x3ec - 0x3e0]; volatile int d294; } SP;
#define SPAD (*(SP *)0x1F800000)
extern int CdGetSector(void *, int);
extern void CdReadyCallback(void *);
extern int CdControlF(int, void *);
void func_80023D2C(unsigned char c)
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

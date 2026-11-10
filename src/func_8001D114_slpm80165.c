// VERSION SLPM_80165 slpm80165 8001D114 196
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_80022830_pcpx96084 (src/func_80022830_pcpx96084.c): CD seek to one of five files (PrePre 8:
// two) of the 24-byte file table D_8006BA50, then a pause when the seek was queued.
extern short D_800700EC[], D_80070114[], D_8007009C[], D_800700C4[], D_8007013C[];
extern char D_8006BA50[];
extern int CdControl(int, void *, void *);
extern int CdControlF(int, void *);

void func_8001D114(short a)
{
    short *p;
    switch (a) {
    case 0:
        p = D_800700EC;
        break;
    case 1:
        p = D_80070114;
        break;
    case 2:
        p = D_8007009C;
        break;
    case 3:
        p = D_800700C4;
        break;
    case 4:
        p = D_8007013C;
        break;
    }
    if (CdControl(2, D_8006BA50 + *p * 24, 0)) CdControlF(0x15, 0);
}
#endif

// VERSION PCPX_96084 tomba 80022830 136
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_800AFDB0[];
extern short D_800AFD88[];
extern char D_800E5D40[];
extern int CdControl(int, void *, void *);
extern int CdControlF(int, void *);

void func_80022830_pcpx96084(short a)
{
    short *p;
    switch (a) {
    case 0:
        p = D_800AFDB0;
        break;
    case 2:
        p = D_800AFD88;
        break;
    }
    if (CdControl(2, D_800E5D40 + *p * 24, 0)) CdControlF(0x15, 0);
}
#endif

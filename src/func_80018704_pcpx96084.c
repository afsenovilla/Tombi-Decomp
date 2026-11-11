// VERSION PCPX_96084 tomba 80018704 124
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_1F80028A, D_1F800392;

void func_80018704_pcpx96084(unsigned char *p)
{
    unsigned char *o = p;
    if (o[0] != 0) {
        if ((unsigned short)(*(unsigned short *)(*(char **)(o + 64) + 2) - D_1F80028A + 160) >= 640
            || (unsigned short)(D_1F800392 - *(unsigned short *)(o + 22) + 160) >= 560) {
            o[4] = 3;
            o[0] = 2;
            o[1] = 0;
        }
    }
}
#endif

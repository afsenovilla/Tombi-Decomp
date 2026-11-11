// VERSION PCPX_96084 tomba 80042834 60
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern char *D_800CCCCC;

void func_80042834_pcpx96084(char *o)
{
    int x = *(unsigned short *)(D_800CCCCC + 32);
    if (x >= 0) {
        if (x < 13) *(short *)(o + 126) = -736;
        else if (x == 13) *(short *)(o + 126) = -816;
    }
}
#endif

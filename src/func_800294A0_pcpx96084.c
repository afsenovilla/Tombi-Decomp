// VERSION PCPX_96084 tomba 800294A0 40
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
unsigned char func_800294A0_pcpx96084(unsigned char a, unsigned char b)
{
    int s = a + b;
    unsigned char r = s;
    if (s > 0xff) r = 0xff;
    return r;
}
#endif

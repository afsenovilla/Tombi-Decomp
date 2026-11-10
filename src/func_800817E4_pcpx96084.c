// VERSION PCPX_96084 tomba 800817E4 76
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern int func_800180D8_pcpx96084(void *);

void func_800817E4_pcpx96084(unsigned char *o)
{
    switch (o[6]) {
    case 0:
        o[6]++;
    case 1:
        func_800180D8_pcpx96084(o);
        break;
    }
}
#endif

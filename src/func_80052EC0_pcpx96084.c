// VERSION PCPX_96084 tomba 80052EC0 124
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern void func_800420E8(unsigned char *);
extern int func_800180D8_pcpx96084(void *);

void func_80052EC0_pcpx96084(unsigned char *o)
{
    switch (o[6]) {
    case 0:
        o[6]++;
    case 1:
        func_800420E8(o);
        func_800180D8_pcpx96084(o);
        if (!(*(unsigned short *)(o + 46) & 8)) {
            o[5] = 1;
            o[6] = 0;
        }
        break;
    }
}
#endif

// VERSION PCPX_96084 tomba 80039CFC 88
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern void func_80039D54(void *);
extern void func_8003A254(void *);

void func_80039CFC_pcpx96084(unsigned char *o)
{
    if (o[12] == 0) {
        func_80039D54(o);
        func_8003A254(*(unsigned char **)(o + 0x94));
        (*(unsigned char **)(o + 0x94))[6] = o[6];
    }
}
#endif

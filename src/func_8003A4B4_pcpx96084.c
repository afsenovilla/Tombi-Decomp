// VERSION PCPX_96084 tomba 8003A4B4 88
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern void func_8003A50C(void *);
extern void func_8003A688(void *);

void func_8003A4B4_pcpx96084(unsigned char *o)
{
    if (o[12] == 0) {
        func_8003A50C(o);
        func_8003A688(*(unsigned char **)(o + 0x94));
        (*(unsigned char **)(o + 0x94))[6] = o[6];
    }
}
#endif

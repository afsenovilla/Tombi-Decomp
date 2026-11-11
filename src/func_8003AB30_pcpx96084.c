// VERSION PCPX_96084 tomba 8003AB30 88
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern void func_8003AB88(void *);
extern void func_8003ACD0(void *);

void func_8003AB30_pcpx96084(unsigned char *o)
{
    if (o[12] == 0) {
        func_8003AB88(o);
        func_8003ACD0(*(unsigned char **)(o + 0x94));
        (*(unsigned char **)(o + 0x94))[6] = o[6];
    }
}
#endif

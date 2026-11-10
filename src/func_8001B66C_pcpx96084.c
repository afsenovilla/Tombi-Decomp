// VERSION PCPX_96084 tomba 8001B66C 152
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short func_8001BEF0(char *o, short x, short y);

static __inline__ short chk(char *o, int dx)
{
    return func_8001BEF0(o, *(unsigned short *)(*(char **)(o + 0x40) + 2) + dx, *(unsigned short *)(o + 0x16) - 12);
}

short func_8001B66C_pcpx96084(char *o)
{
    short r;
    char *s = o;
    if ((r = chk(s, 8)) != 0) return r;
    if ((r = chk(s, -8)) != 0) return r;
    return 0;
}
#endif

// VERSION PCPX_96084 tomba 8001B5B0 188
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Probes 8 px right then left at y+16 (func_8001BC4C); on a hit stores the side (2 right, 3 left) in +0xA2.
extern short func_8001BC4C(char *o, short x, short y);

static __inline__ short chk(char *o, int dx)
{
    return func_8001BC4C(o, *(unsigned short *)(*(char **)(o + 0x40) + 2) + dx, *(unsigned short *)(o + 0x16) + 16);
}

short func_8001B5B0_pcpx96084(char *o)
{
    short r;
    char *s = o;
    if ((r = chk(s, 8)) != 0) {
        s[0xa2] = 2;
        return r;
    }
    if ((r = chk(s, -8)) != 0) {
        s[0xa2] = 3;
        return r;
    }
    return 0;
}
#endif

// VERSION PCPX_96084 tomba 80030FA0 228
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x000 80118D8C (src/spawnMushroomClusterDebris.c): +0xA is a halfword here.
extern char *func_8001A464_pcpx96084(void);

void func_80030FA0_pcpx96084(char *o, short b, short c, int d)
{
    int i = 0;
    int cc = c;
    int dd = d << 16;
    int x = b;
    char *n;
    unsigned short t;
    do {
        n = func_8001A464_pcpx96084();
        if (n != 0) {
            n[0] = 1;
            n[2] = 2;
            *(int *)(n + 0x10) = x << 16;
            *(int *)(n + 0x14) = (cc + (i % 2) * -8) * 0x10000;
            *(int *)(n + 0x18) = dd;
            t = *(unsigned short *)(o + 0x1e);
            n[0xd] = 0;
            *(short *)(n + 10) = 0;
            n[3] = i;
            *(signed char *)&n[0xf] = -2;
            *(unsigned short *)(n + 0x1e) = t;
            *(int *)(n + 0x3c) = *(int *)(o + 0x3c);
        }
        i++;
        x -= 2;
    } while (i < 6);
}
#endif

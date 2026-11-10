// VERSION PCPX_96084 tomba 800950F0 192
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
// SN host-file transfer in chunks of at most 0x8000 bytes through func_800951B0_pcpx96084; -1 on error,
// else the byte count (stops early on a short chunk).
extern int func_800951B0_pcpx96084();
int func_800950F0_pcpx96084(int fd, char *buf, unsigned int n)
{
    int total = 0;
    while (n != 0) {
        unsigned int c = n;
        int r;
        if (c > 0x8000) c = 0x8000;
        r = func_800951B0_pcpx96084(0, fd, c, buf);
        if (r == -1) return -1;
        total += r;
        buf += r;
        n -= r;
        if (r < (int)c) return total;
    }
    return total;
}

#endif

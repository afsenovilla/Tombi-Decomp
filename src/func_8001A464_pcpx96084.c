// VERSION PCPX_96084 tomba 8001A464 124
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 object allocator, the shape of PAL allocObjectLayer7 (src/allocObjectLayer7.c): count at scratchpad
// 0x2CA, free list pointer at 0x2F0; the axis swap flag is the halfword at 0x1F800128. The empty array gives the
// 8-byte frame of the original.
extern short D_1F8002CA;
extern int *D_1F8002F0;
extern short D_1F800128;

int func_8001A464_pcpx96084(void)
{
    char pad[0];
    int r;
    int *p;
    if (D_1F8002CA > 0) {
        D_1F8002CA = D_1F8002CA - 1;
        p = D_1F8002F0;
        D_1F8002F0 = p + 1;
        r = *p;
        if (D_1F800128 == 0) {
            *(int *)(r + 0x40) = r + 0x10;
            *(int *)(r + 0x44) = r + 0x18;
        } else {
            *(int *)(r + 0x44) = r + 0x10;
            *(int *)(r + 0x40) = r + 0x18;
        }
        return r;
    }
    return 0;
}
#endif

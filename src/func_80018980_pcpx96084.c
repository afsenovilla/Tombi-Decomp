// VERSION PCPX_96084 tomba 80018980 56
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL setObjectAxisPointers (src/setObjectAxisPointers.c): the axis flag is the whole halfword at
// scratchpad 0x1F800128, not bit 0 of 0x1F8001C8.
typedef unsigned char u8;

void func_80018980_pcpx96084(u8 *self)
{
    if (*(short *)0x1F800128 == 0) {
        *(u8 **)(self + 0x40) = self + 0x10;
        *(u8 **)(self + 0x44) = self + 0x18;
    } else {
        *(u8 **)(self + 0x44) = self + 0x10;
        *(u8 **)(self + 0x40) = self + 0x18;
    }
}
#endif

// VERSION PCPX_96084 tomba 800690F4 68
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
void func_800690F4_pcpx96084(unsigned char *o)
{
    o[0x9c] = 0;
    o[0] = 2;
    switch (o[2]) {
    case 0:
        *(short *)(o + 0xa0) = 0x500;
        break;
    case 1:
    case 2:
        o[4] = 2;
        o[5] = 0;
        o[6] = 0;
        break;
    }
}
#endif

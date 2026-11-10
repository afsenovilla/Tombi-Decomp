// VERSION PCPX_96084 tomba 8003E428 108
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
void func_8003E428_pcpx96084(unsigned char *o)
{
    switch (o[3]) {
    case 0:
        *(int *)(o + 0x8C) = (*(int *)(o + 0x8C) + 8) & 0xFFF;
        break;
    case 1:
        *(int *)(o + 0x8C) = (*(int *)(o + 0x8C) - 8) & 0xFFF;
        break;
    case 3:
        *(int *)(o + 0x8C) = (*(int *)(o + 0x8C) + 8) & 0xFFF;
        break;
    case 4:
        *(int *)(o + 0x8C) = (*(int *)(o + 0x8C) - 8) & 0xFFF;
        break;
    case 7:
        *(int *)(o + 0x8C) = (*(int *)(o + 0x8C) - 8) & 0xFFF;
        break;
    case 8:
        *(int *)(o + 0x8C) = (*(int *)(o + 0x8C) + 8) & 0xFFF;
        break;
    case 9:
        *(int *)(o + 0x8C) = (*(int *)(o + 0x8C) - 8) & 0xFFF;
        break;
    }
}
#endif

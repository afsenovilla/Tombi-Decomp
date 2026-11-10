// VERSION PCPX_96084 tomba 80054B00 160
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_800A7F98[];
extern void func_800531A4(unsigned char *o);
extern void func_80053674(unsigned char *o);
extern void func_80053BE4(unsigned char *o);

void func_80054B00_pcpx96084(unsigned char *o)
{
    switch (o[5]) {
    case 0:
        func_800531A4(o);
        *(int *)(o + 0x8C) = D_800A7F98[*(short *)(o + 0xA8)];
        break;
    case 1:
        func_80053674(o);
        *(int *)(o + 0x8C) = D_800A7F98[*(short *)(o + 0xA8)];
        break;
    case 2:
        func_80053BE4(o);
        break;
    }
}
#endif

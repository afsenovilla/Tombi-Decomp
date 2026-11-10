// VERSION PCPX_96084 tomba 80080058 176
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern void *D_80014CB0;
extern char D_800A18B0[];
extern void func_800180A0_pcpx96084(unsigned char *o);
extern void func_80018550(unsigned char *o);

void func_80080058_pcpx96084(unsigned char *o)
{
    switch (o[6]) {
    case 0:
        *(short *)(o + 0xA8) = 0;
        *(void **)(o + 0x24) = D_80014CB0;
        *(int *)(o + 0x30) = **(int **)(o + 0x40);
        *(int *)(o + 0x34) = *(int *)(o + 0x14);
        *(int *)(o + 0x38) = **(int **)(o + 0x44);
        *(char **)(o + 0x28) = D_800A18B0;
        *(short *)(o + 0x80) = 0x180;
        *(int *)(o + 0x84) = 0;
        *(int *)(o + 0x88) = 0;
        *(int *)(o + 0x8C) = 0;
        o[6]++;
        *(short *)(o + 0x20) = 8;
        func_800180A0_pcpx96084(o);
        break;
    case 1:
        func_80018550(o);
        break;
    case 2:
        break;
    }
}
#endif

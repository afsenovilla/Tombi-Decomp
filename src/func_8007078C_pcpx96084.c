// VERSION PCPX_96084 tomba 8007078C 96
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
typedef struct { unsigned short a, b, c, d; } Q;
extern Q D_800AE9A4[];

void func_8007078C_pcpx96084(char *o)
{
    unsigned short *p = &D_800AE9A4[*(unsigned short *)(*(char **)(o + 36) + 2)].a;
    *(short *)(o + 108) = *p++;
    *(short *)(o + 110) = *p++;
    *(short *)(o + 112) = *p;
    *(short *)(o + 114) = p[1];
    *(short *)(o + 44) = *(unsigned short *)(*(char **)(o + 36) + 6) & 0x3fff;
}
#endif

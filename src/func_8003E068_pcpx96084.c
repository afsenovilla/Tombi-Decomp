// VERSION PCPX_96084 tomba 8003E068 124
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned short D_800C826C[], D_800C826E[], D_800C8270[], D_800C8272[], D_800C8274[], D_800C8276[];

void func_8003E068_pcpx96084(int i, short *a, short *b)
{
    a[1] = D_800C826C[i * 16];
    a[3] = D_800C826E[i * 16];
    a[5] = D_800C8270[i * 16];
    b[1] = D_800C8272[i * 16];
    b[3] = D_800C8274[i * 16];
    b[5] = D_800C8276[i * 16];
}
#endif

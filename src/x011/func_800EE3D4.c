// FUNC 800ee3d4 84 X011
// MATCHING 800ee3d4 84
extern unsigned char *D_8009C330;
void func_800EE3D4(unsigned char *o)
{
    D_8009C330[8] = 0;
    D_8009C330[9] = 0;
    *(short *)(D_8009C330 + 0xc) = 0;
    *(unsigned short *)(D_8009C330 + 0x28) = 0xffff;
    *(unsigned short *)(D_8009C330 + 0x2a) = 0xffff;
    *(short *)(o + 0x80) = 0;
    *(short *)(o + 0x82) = 0;
    *(short *)(o + 0xb0) = 0;
    *(int *)(o + 0x84) = 0;
    *(int *)(o + 0x88) = 0;
    *(int *)(o + 0x8c) = 0;
}

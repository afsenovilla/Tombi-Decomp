// FUNC 8010d7f0 152 X014
// MATCHING 8010d7f0 152
void func_8010D7F0(unsigned char *o)
{
    switch (*(unsigned short *)(o + 0x76) & 7) {
    case 0: *(short *)(o + 0x7a) = 0; break;
    case 1: *(short *)(o + 0x7a) = 0x20; break;
    case 2: *(short *)(o + 0x7a) = 0x40; break;
    case 3: *(short *)(o + 0x7a) = 0x60; break;
    case 4: *(short *)(o + 0x7a) = 0x80; break;
    case 5: *(short *)(o + 0x7a) = 0xa0; break;
    case 6: *(short *)(o + 0x7a) = 0xc0; break;
    case 7: *(short *)(o + 0x7a) = 0xe0; break;
    }
    if (*(unsigned short *)(o + 0x76) & 8) *(short *)(o + 0x74) = 0;
    else *(short *)(o + 0x74) = 0x200;
}

// near-twin of setAngleFromDir8 (PAL)
// VERSION SLPS_01144 slps01144 80037D08 92
#if defined(VERSION_SLPS_01144)
void func_80037D08_slps01144(unsigned char *o)
{
    switch (*(unsigned short *)(o + 0x2e)) {
    case 0: case 1: case 2: case 3:
        *(int *)(o + 0x8c) = 0; break;
    case 4: *(int *)(o + 0x8c) = 0x20; break;
    case 5: *(int *)(o + 0x8c) = 0xe0; break;
    case 6: *(int *)(o + 0x8c) = 0x40; break;
    case 7: *(int *)(o + 0x8c) = 0xc0; break;
    }
}
#endif

// FUNC 80036ebc 116 MAIN0
// MATCHING 80036ebc 116
void func_80036EBC(unsigned char *o)
{
    if ((unsigned char)(o[6] - 6) < 2) return;
    switch (*(unsigned short *)(o + 0x2e)) {
    case 0: case 1: case 2: case 3:
        *(int *)(o + 0x8c) = 0; break;
    case 4: *(int *)(o + 0x8c) = 0x20; break;
    case 5: *(int *)(o + 0x8c) = 0xe0; break;
    case 6: *(int *)(o + 0x8c) = 0x40; break;
    case 7: *(int *)(o + 0x8c) = 0xc0; break;
    }
}

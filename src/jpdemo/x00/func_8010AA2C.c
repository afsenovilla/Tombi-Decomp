// JPDEMO x00 8010AA2C 148
// Demo-only helper: sets the vertical speed (+0x7e) from the D_8010F060 table by player animation (0..12, 13), level and kind.
extern struct { char p[0x20]; unsigned short w20; } *D_800893C8;
extern unsigned char D_8009B163;
extern unsigned short D_8010F060[][10];
void func_8010AA2C(unsigned char *o)
{
    int i = D_8009B163 + o[0xc1] * 3;
    switch (D_800893C8->w20) {
    case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12:
        *(short *)(o + 0x7e) = D_8010F060[i][0];
        break;
    case 13:
        *(short *)(o + 0x7e) = D_8010F060[i][-1];
        break;
    }
}

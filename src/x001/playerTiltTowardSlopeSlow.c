// FUNC 800ee51c 68 X001
// MATCHING 800ee51c 68
extern unsigned char SLOPE_TILT_TABLE_SLOW[];
void playerTiltTowardSlopeSlow(unsigned char *o)
{
    int t = (unsigned char)(SLOPE_TILT_TABLE_SLOW[*(short *)(o + 0xb0)] - *(int *)(o + 0x8c));
    unsigned char d = t;
    if (t != 0) {
        if (d < 0x81) (*(int *)(o + 0x8c))--;
        else (*(int *)(o + 0x8c))++;
    }
}

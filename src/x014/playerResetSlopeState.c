// FUNC 80103774 100 X014
// MATCHING 80103774 100
extern char *DAT_8009d2e8;
extern void playerSetSlopeTilt(void *, int);

void playerResetSlopeState(char *o)
{
    o[0x69] = 0;
    o[0xac] = 3;
    o[0x9c] = 0;
    *(short *)(o + 0x7c) = 0;
    *(short *)(o + 0x7e) = 0;
    *(short *)(o + 0x80) = 0;
    *(short *)(o + 0x82) = 0;
    *(short *)(o + 0xb2) = 0;
    playerSetSlopeTilt(o, 0);
    *(unsigned int *)(DAT_8009d2e8 + 0x8c) = *(unsigned char *)(o + 0x8c);
}

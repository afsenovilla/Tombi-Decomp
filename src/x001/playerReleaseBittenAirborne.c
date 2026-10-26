// FUNC 800fc02c 216 X001
// MATCHING 800fc02c 216
extern unsigned char *DAT_800a611c;
extern unsigned char *BITTEN_OBJECT;
extern int DAT_8009c934;
extern unsigned char *PLAYER_EXT;
extern unsigned char *GRABBED_OBJECT;

void playerReleaseBittenAirborne(unsigned char *o)
{
    unsigned char *p;
    o[0x9c] = 1;
    if (o[0xac] >= 2) {
        BITTEN_OBJECT = DAT_800a611c;
        DAT_800a611c[4] = 2;
        BITTEN_OBJECT[5] = 2;
        BITTEN_OBJECT[6] = 0;
    }
    o[0xac] = 0;
    DAT_8009c934 = 0;
    o[0xc7] = 1;
    o[0x9d] = 0;
    o[0xc6] = 0;
    o[0xe3] = 0;
    *PLAYER_EXT = 0;
    *(int *)(o + 0x8c) = 0;
    if (o[0x9e] != 0)
        GRABBED_OBJECT[0x6a] = 0;
    o[0x9e] = 0;
    o[0xaa] = 0;
    o[0xa7] = 0;
    p = PLAYER_EXT;
    *(short *)(o + 0xb0) = 0;
    p[8] = 0;
    *(signed char *)(o + 0xf) = -8;
    o[0xa4] = 0;
    o[0x69] = 0;
    *(short *)(o + 0x82) = 0;
    o[6]++;
}

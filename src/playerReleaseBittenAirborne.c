// FUNC 800fc02c 216 X000
// MATCHING 800fc02c 216
// VERSION SLPM_80165 x00 80104800 212
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800FC02C (FUN_800fc02c.c): sets active 2, clears +0x9c, no DAT_8009c934 reset, flag at
// +0xcf (PAL +0xe3).
extern unsigned char *DAT_800a611c;
extern unsigned char *BITTEN_OBJECT;
extern int DAT_8009c934;
extern unsigned char *PLAYER_EXT;
extern unsigned char *GRABBED_OBJECT;

void FUN_800fc02c(unsigned char *o)
{
    unsigned char *p;
    o[0] = 2;
    o[0x9c] = 0;
    if (o[0xac] >= 2) {
        BITTEN_OBJECT = DAT_800a611c;
        DAT_800a611c[4] = 2;
        BITTEN_OBJECT[5] = 2;
        BITTEN_OBJECT[6] = 0;
    }
    o[0xac] = 0;
    o[0xc7] = 1;
    o[0x9d] = 0;
    o[0xc6] = 0;
    o[0xcf] = 0;
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
#else
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
#endif

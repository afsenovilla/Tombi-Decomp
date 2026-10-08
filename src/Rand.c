// FUNC 8001f9e0 64 MAIN0
// MATCHING 8001f9e0 64
extern int DAT_1f800200;
unsigned Rand(void)
{
    unsigned u;
    int k = 0x41c64e6d;
    DAT_1f800200 = DAT_1f800200 * k + 0x3039;
    u = DAT_1f800200;
    if ((int)u < 0) u += 0xffff;
    return u >> 16;
}

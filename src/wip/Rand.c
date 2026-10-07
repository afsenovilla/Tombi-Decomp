// FUNC 8001f9e0 64 MAIN0
extern int DAT_1f800200;
unsigned Rand(void)
{
    unsigned u;
    DAT_1f800200 = DAT_1f800200 * 0x41c64e6d + 0x3039;
    u = DAT_1f800200;
    if ((int)u < 0) u += 0xffff;
    return u >> 16;
}

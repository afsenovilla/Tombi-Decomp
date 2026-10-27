// FUNC 8001f9e0 64 MAIN0
// MATCHING 8001f9e0 64
extern int RAND_SEED;
unsigned Rand(void)
{
    unsigned u;
    int k = 0x41c64e6d;
    RAND_SEED = RAND_SEED * k + 0x3039;
    u = RAND_SEED;
    if ((int)u < 0) u += 0xffff;
    return u >> 16;
}

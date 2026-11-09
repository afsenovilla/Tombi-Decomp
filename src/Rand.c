// FUNC 8001f9e0 64 MAIN0
// MATCHING 8001f9e0 64
extern int GAME_RAND_SEED;
unsigned Rand(void)
{
    unsigned u;
    int k = 0x41c64e6d;
    GAME_RAND_SEED = GAME_RAND_SEED * k + 0x3039;
    u = GAME_RAND_SEED;
    if ((int)u < 0) u += 0xffff;
    return u >> 16;
}
